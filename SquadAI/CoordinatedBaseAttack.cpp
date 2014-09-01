/* 
*  Kevin Meergans, SquadAI, 2014
*  CoordinatedBaseAttack.cpp
*  A team manoeuvre that splits the participating entities into equally strong groups
*  that will attack the enemy base from different directions.
*/

// Includes
#include "CoordinatedBaseAttack.h"
#include "MultiflagCTFTeamAI.h"
#include "Entity.h"
#include "TestEnvironment.h"

CoordinatedBaseAttack::CoordinatedBaseAttack(unsigned int minNumberParticipants, unsigned int maxNumberParticipants, MultiflagCTFTeamAI* pTeamAI, unsigned int numberOfGroups, float waitForParticipantsInterval)
	: TeamManoeuvre(CoordinatedBaseAttackManoeuvre, AttackEnemyFlagCategory, minNumberParticipants, maxNumberParticipants),
	  m_pTeamAI(pTeamAI),
	  m_waitForParticipantsInterval(waitForParticipantsInterval),
	  m_timer(0.0f),
	  m_currentPhase(AssemblePhase),
	  m_numberOfGroups(numberOfGroups)
{
}

CoordinatedBaseAttack::~CoordinatedBaseAttack(void)
{
}


//--------------------------------------------------------------------------------------
// Processes an inbox message that the manoeuvre received.
// Param1: A pointer to the message to process.
//--------------------------------------------------------------------------------------
void CoordinatedBaseAttack::ProcessMessage(Message* pMessage)
{

	// add steal flag message -> success
	// add participant killed -> release
	// check if all dead -> terminate/fail

	switch(pMessage->GetType())
	{
	case FlagPickedUpMessageType:
	{
		FlagPickedUpMessage* pMsg = reinterpret_cast<FlagPickedUpMessage*>(pMessage);
		if(pMsg->GetData().m_flagOwner != GetTeamAI()->GetTeam())
		{
			// The enemy flag was picked up, the manoeuvre succeeded
			// Note: At the moment it is not distinguished whether an actual participant of this manoeuvre stole the 
			//       flag or if another team member did so.
			SetSucceeded(true);
		}
		break;
	}
	case EntityKilledMessageType:
	{
		EntityKilledMessage* pMsg = reinterpret_cast<EntityKilledMessage*>(pMessage);
		if(IsParticipant(pMsg->GetData().m_id) && pMsg->GetData().m_team == GetTeamAI()->GetTeam())
		{
			// Participants that get killed, drop out of the manoeuvre
			m_pTeamAI->ReleaseEntityFromManoeuvre(pMsg->GetData().m_id);

			m_entityGroupMap.erase(pMsg->GetData().m_id);

			if(m_currentPhase == AssemblePhase)
			{
				std::set<unsigned long>::iterator foundId = m_arrivedEntities.find(pMsg->GetData().m_id);
				if(foundId != m_arrivedEntities.end())
				{
					// The entity already arrived, remove it from the set of arrived entities
					m_arrivedEntities.erase(foundId);
				}

				// Check if the attack manoeuvre should be started
				if(m_arrivedEntities.size() >= GetNumberOfParticipants())
				{
					StartAttack();
				}
			}
		}
		break;
	}
	case UpdateOrderStateMessageType:
	{
	// Cancel old order, Send Follow-Up Orders, finish manoeuvre etc
	UpdateOrderStateMessage* pMsg = reinterpret_cast<UpdateOrderStateMessage*>(pMessage);
	if(IsParticipant(pMsg->GetData().m_entityId))
	{
		if(pMsg->GetData().m_orderState == SucceededOrderState)
		{
			// Officially cancel the old order that was fulfilled and delete it.
			CancelOrder(pMsg->GetData().m_entityId);
			m_activeOrders.erase(m_activeOrders.find(pMsg->GetData().m_entityId));

			if(m_currentPhase == AssemblePhase)
			{
				// Register the entity as having reached its assembly point
				m_arrivedEntities.insert(pMsg->GetData().m_entityId);
			
				// Send a new defend order to let the entity wait for other entities
				// It's a low priority order as it is sufficient if the entities are in the same area when the actual rush attack begins.
				Order* pNewOrder = new DefendOrder(pMsg->GetData().m_entityId, DefendPositionOrder, MediumPriority, XMFLOAT2(m_assemblyPoints[m_entityGroupMap[pMsg->GetData().m_entityId]]), XMFLOAT2(0.0f,0.0f));
		
				if(!pNewOrder)
				{
					SetFailed(true);
				}
		
				// Find the participant
				std::vector<Entity*>::iterator foundIt = std::find_if(m_participants.begin(), m_participants.end(), Entity::FindEntityById(pMsg->GetData().m_entityId));

				FollowOrderMessageData data(pNewOrder);
				SendMessage(*foundIt, FollowOrderMessageType, &data);

				m_activeOrders.insert(std::pair<unsigned long, Order*>(pMsg->GetData().m_entityId, pNewOrder));

				if(m_arrivedEntities.size() >= GetNumberOfParticipants())
				{
					// All participants arrived at the assembly point, no point in waiting for the interval to
					// expire -> attack now
					StartAttack();
				}
			}
		
		}else if(pMsg->GetData().m_orderState == FailedOrderState)
		{
			// Entities executing a defend manoeuvre won't send success messages concerning the order state
			// as the defend order is a passive behaviour that is unlimited in time and thus cannot succeed.
			// It is thus sufficient to check for failure.

			// The order failed -> release the entity from the manoeuvre
			m_pTeamAI->ReleaseEntityFromManoeuvre(pMsg->GetData().m_entityId);
		}
	}
	break;
	}
	default:
		TeamManoeuvre::ProcessMessage(pMessage);
	}
}

//--------------------------------------------------------------------------------------
// Starts the actual attack on the enemy base in order to steal the flag.
//--------------------------------------------------------------------------------------
void CoordinatedBaseAttack::StartAttack(void)
{
	// Clear all current orders
	ClearOrders();

	m_currentPhase = AttackPhase;

	// Determine the target for the attack
	XMFLOAT2 target(0.0f, 0.0f);

	if(GetTeamAI()->GetTeam() == TeamRed)
	{
		target = GetTeamAI()->GetFlagData(TeamBlue).m_basePosition;
	}else
	{
		target = GetTeamAI()->GetFlagData(TeamRed).m_basePosition;
	}

	for(std::vector<Entity*>::iterator it = m_participants.begin(); it != m_participants.end(); ++it)
	{
		// Send out the new move orders to attack the enemy flag/base
		Order* pNewOrder = new MoveOrder((*it)->GetId(), MoveToPositionOrder, MediumPriority, XMFLOAT2(target));
		
		if(!pNewOrder)
		{
			SetFailed(true);
		}
		
		FollowOrderMessageData data(pNewOrder);
		SendMessage(*it, FollowOrderMessageType, &data);

		m_activeOrders.insert(std::pair<unsigned long, Order*>((*it)->GetId(), pNewOrder));
	}
}

//--------------------------------------------------------------------------------------
// Distributes the participants over the number of groups that was specified upon creation
// of the manoeuvre.
//--------------------------------------------------------------------------------------
void CoordinatedBaseAttack::SetupGroups(void)
{
	unsigned int groupNumber = 0;

	for(std::vector<Entity*>::iterator it = m_participants.begin(); it != m_participants.end(); ++it)
	{
		m_entityGroupMap.insert(std::pair<unsigned long, unsigned int>((*it)->GetId(), groupNumber));
		groupNumber = (groupNumber + 1) % m_numberOfGroups;
	}
}

//--------------------------------------------------------------------------------------
// Determines the points at which to assemble the participants of the different groups
// before sending them out to attack the enemy base.
//--------------------------------------------------------------------------------------
void CoordinatedBaseAttack::DetermineAssemblyPoints(void)
{
	unsigned int index = rand() % GetTeamAI()->GetTestEnvironment()->GetAttackPositions(GetTeamAI()->GetTeam()).size();
	std::unordered_map<Direction, std::vector<XMFLOAT2>>::const_iterator it = GetTeamAI()->GetTestEnvironment()->GetAttackPositions(GetTeamAI()->GetTeam()).begin();
	std::advance(it, index);

	for(unsigned int i = 0; i < m_numberOfGroups; ++i)
	{
		m_assemblyPoints.push_back(it->second.at(rand() % it->second.size()));

		++it;
		if(it == GetTeamAI()->GetTestEnvironment()->GetAttackPositions(GetTeamAI()->GetTeam()).end())
		{
			it = GetTeamAI()->GetTestEnvironment()->GetAttackPositions(GetTeamAI()->GetTeam()).begin();
		}
	}
}

//--------------------------------------------------------------------------------------
// Initiates the manoeuvre. This mostly consists of sending initial orders to all
// participating entities and everything that is involved in that process, such as 
// determining targets etc.
// Returns a behaviour status code representing the current state of the initiation of the manoeuvre.
//--------------------------------------------------------------------------------------
BehaviourStatus CoordinatedBaseAttack::Initiate(void)
{
	SetupGroups();
	DetermineAssemblyPoints();

	for(std::vector<Entity*>::iterator it = m_participants.begin(); it != m_participants.end(); ++it)
	{
		Order* pNewOrder = new MoveOrder((*it)->GetId(), MoveToPositionOrder, MediumPriority, m_assemblyPoints[m_entityGroupMap[(*it)->GetId()]]);
			
		if(!pNewOrder)
		{
			return StatusFailure;
		}

		FollowOrderMessageData data(pNewOrder);
		SendMessage(*it, FollowOrderMessageType, &data);

		m_activeOrders.insert(std::pair<unsigned long, Order*>((*it)->GetId(), pNewOrder));
	}
	
	return StatusSuccess;
}

//--------------------------------------------------------------------------------------
// Updates the manoeuvre. This mostly consists of processing any messages received from
// the participating entities and reacting accordingly. This can include the sending
// out of follow-up orders for entities that have completed a stage of the manoeuvre. Eventually
// the function should also check whether the goal of the manoeuvre has been achieved in 
// order to initiate termination.
// Param1: The time passed since the last update.
// Returns a behaviour status representing the state of the manoeuvre.
//--------------------------------------------------------------------------------------
BehaviourStatus CoordinatedBaseAttack::Update(float deltaTime)
{
	// Keep track of who has reached the target and let those defend until all have arrived
	SortOutProcessedMessages();
	ProcessMessages();

	if(m_currentPhase == AssemblePhase)
	{
		m_timer += deltaTime;

		if(m_timer >= m_waitForParticipantsInterval)
		{
			// Waited long enough, start the attack
			StartAttack();
		}
	}

	if(!IsActive() || HasFailed() || (GetNumberOfParticipants() < GetMinNumberOfParticipants()))
	{
		// The manoeuvre will fail if something failed during the initiation or if it wasn't
		// initiated at all.
		return StatusFailure;
	}else if(HasSucceeded())
	{
		return StatusSuccess;
	}

	return StatusRunning;
}

//--------------------------------------------------------------------------------------
// Terminates the manoeuvre. This mostly consists of cancelling all active orders and
// removing all participating entities.
//--------------------------------------------------------------------------------------
void CoordinatedBaseAttack::Terminate(void)
{
	m_timer = 0.0f;
	m_arrivedEntities.clear();
	m_currentPhase = AssemblePhase;
	m_assemblyPoints.clear();
	m_entityGroupMap.clear();

	TeamManoeuvre::Terminate();
}

//--------------------------------------------------------------------------------------
// Resets the manoeuvre. Should only be called to reset the manoeuver in between game rounds.
//--------------------------------------------------------------------------------------
void CoordinatedBaseAttack::Reset(void)
{
	m_timer = 0.0f;
	m_arrivedEntities.clear();
	m_currentPhase = AssemblePhase;
	m_assemblyPoints.clear();
	m_entityGroupMap.clear();

	TeamManoeuvre::Reset();
}

MultiflagCTFTeamAI* CoordinatedBaseAttack::GetTeamAI(void)
{
	return m_pTeamAI;
}

float CoordinatedBaseAttack::GetWaitForParticipantsInterval(void) const
{
	return m_waitForParticipantsInterval;
}

const XMFLOAT2& CoordinatedBaseAttack::GetAssemblyPoint(unsigned int group) const
{
	return m_assemblyPoints[group];
}

unsigned int CoordinatedBaseAttack::GetNumberOfGroups(void) const
{
	return m_numberOfGroups;
}

void CoordinatedBaseAttack::SetTeamAI(MultiflagCTFTeamAI* pTeamAI)
{
	if(pTeamAI)
	{
		m_pTeamAI = pTeamAI;
	}
}

void CoordinatedBaseAttack::SetWaitForParticipantsInterval(float interval)
{
	m_waitForParticipantsInterval = interval;
}

void CoordinatedBaseAttack::SetAssemblyPoint(unsigned int group, const XMFLOAT2& assemblyPoint)
{
	m_assemblyPoints[group] = assemblyPoint;
}
