/* 
*  Kevin Meergans, SquadAI, 2014
*  DistractionBaseAttack.cpp
*  A team manoeuvre that orders most participating entities to perform a distraction attack
*  on the enemy base while a single participant attempts to steal the flag by entering the
*  base from another direction while the defenders are ideally busy with fighting off the
*  distraction attack.
*/

// Includes
#include "DistractionBaseAttack.h"
#include "MultiflagCTFTeamAI.h"
#include "Entity.h"
#include "TestEnvironment.h"

DistractionBaseAttack::DistractionBaseAttack(unsigned int minNumberParticipants, unsigned int maxNumberParticipants, MultiflagCTFTeamAI* pTeamAI, unsigned int numberOfSneakers, float waitForParticipantsInterval)
	: TeamManoeuvre(CoordinatedBaseAttackManoeuvre, AttackEnemyFlagCategory, minNumberParticipants, maxNumberParticipants),
	  m_pTeamAI(pTeamAI),
	  m_waitForParticipantsInterval(waitForParticipantsInterval),
	  m_timer(0.0f),
	  m_currentPhase(AssemblePhase),
	  m_distractionAssemblyPoint(0.0f, 0.0f),            
	  m_sneakAssemblyPoint(0.0f, 0.0f),
	  m_numberOfSneakers(numberOfSneakers)
{
}

DistractionBaseAttack::~DistractionBaseAttack(void)
{
}


//--------------------------------------------------------------------------------------
// Processes an inbox message that the manoeuvre received.
// Param1: A pointer to the message to process.
//--------------------------------------------------------------------------------------
void DistractionBaseAttack::ProcessMessage(Message* pMessage)
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
	case AttackedByEnemyMessageType:
	{
		if(m_currentPhase == DistractionAttackPhase)
		{
			AttackedByEnemyMessage* pMsg = reinterpret_cast<AttackedByEnemyMessage*>(pMessage);

			if(GetTeamAI()->GetTestEnvironment()->GetTerritoryOwner(pMsg->GetData().m_attackPosition))
			{
				StartSneakAttack();
			}
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

			if(m_sneakParticipants.find(pMsg->GetData().m_id) != m_sneakParticipants.end())
			{
				m_sneakParticipants.erase(pMsg->GetData().m_id);
			}else
			{
				m_distractionParticipants.erase(pMsg->GetData().m_id);
			}

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
					StartDistractionAttack();
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
					
				Order* pNewOrder = nullptr;

				if(m_sneakParticipants.find(pMsg->GetData().m_entityId) != m_sneakParticipants.end())
				{
					pNewOrder = new DefendOrder(pMsg->GetData().m_entityId, DefendPositionOrder, MediumPriority, m_sneakAssemblyPoint, XMFLOAT2(0.0f, 0.0f));
				}else
				{
					pNewOrder = new DefendOrder(pMsg->GetData().m_entityId, DefendPositionOrder, MediumPriority, m_distractionAssemblyPoint, XMFLOAT2(0.0f, 0.0f));
				}

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
					StartDistractionAttack();
				}
			}else
			{
				m_pTeamAI->ReleaseEntityFromManoeuvre(pMsg->GetData().m_entityId);
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
// Starts the distraction attack on the enemy base.
//--------------------------------------------------------------------------------------
void DistractionBaseAttack::StartDistractionAttack(void)
{
	m_currentPhase = DistractionAttackPhase;

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
		if(m_distractionParticipants.find((*it)->GetId()) != m_distractionParticipants.end())
		{
			// Cancel the old order and delete it.
			CancelOrder((*it)->GetId());
			m_activeOrders.erase(m_activeOrders.find((*it)->GetId()));

			// Send out the new move order to attack the enemy flag/base
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
}

//--------------------------------------------------------------------------------------
// Starts the sneak attack on the enemy base.
//--------------------------------------------------------------------------------------
void DistractionBaseAttack::StartSneakAttack(void)
{
	m_currentPhase = SneakAttackPhase;

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
		if(m_sneakParticipants.find((*it)->GetId()) != m_sneakParticipants.end())
		{
			// Cancel the old order and delete it.
			CancelOrder((*it)->GetId());
			m_activeOrders.erase(m_activeOrders.find((*it)->GetId()));

			// Send out the new move order to attack the enemy flag/base
			Order* pNewOrder = new MoveOrder((*it)->GetId(), MoveToPositionOrder, HighPriority, XMFLOAT2(target));
		
			if(!pNewOrder)
			{
				SetFailed(true);
			}
		
			FollowOrderMessageData data(pNewOrder);
			SendMessage(*it, FollowOrderMessageType, &data);

			m_activeOrders.insert(std::pair<unsigned long, Order*>((*it)->GetId(), pNewOrder));
		}
	}
}

//--------------------------------------------------------------------------------------
// Splits the participants into distraction and sneak groups.
//--------------------------------------------------------------------------------------
void DistractionBaseAttack::SetupGroups(void)
{
	unsigned int sneakCount = 0;

	for(std::vector<Entity*>::iterator it = m_participants.begin(); it != m_participants.end(); ++it)
	{
		if(sneakCount < m_numberOfSneakers)
		{
			m_sneakParticipants.insert((*it)->GetId());
			++sneakCount;
		}else
		{
			m_distractionParticipants.insert((*it)->GetId());
		}
	}
}

//--------------------------------------------------------------------------------------
// Determines the points at which to assemble the participants of the different groups
// before sending them out to attack the enemy base.
// Returns true if suitable assembly points could be determined, false otherwise.
//--------------------------------------------------------------------------------------
bool DistractionBaseAttack::DetermineAssemblyPoints(void)
{
	if(GetTeamAI()->GetTestEnvironment()->GetAttackPositions(GetTeamAI()->GetTeam()).size() < 2)
	{
		return false;
	}

	unsigned int firstIndex = rand() % GetTeamAI()->GetTestEnvironment()->GetAttackPositions(GetTeamAI()->GetTeam()).size();
	std::unordered_map<Direction, std::vector<XMFLOAT2>>::const_iterator it1 = GetTeamAI()->GetTestEnvironment()->GetAttackPositions(GetTeamAI()->GetTeam()).begin();
	std::advance(it1, firstIndex);

	unsigned int secondIndex = firstIndex;
	do
	{
		secondIndex = rand() % GetTeamAI()->GetTestEnvironment()->GetAttackPositions(GetTeamAI()->GetTeam()).size();
	}while(secondIndex == firstIndex);

	std::unordered_map<Direction, std::vector<XMFLOAT2>>::const_iterator it2 = GetTeamAI()->GetTestEnvironment()->GetAttackPositions(GetTeamAI()->GetTeam()).begin();
	std::advance(it2, secondIndex);

	m_distractionAssemblyPoint = it1->second.at(rand() % it1->second.size());
	m_sneakAssemblyPoint	   = it2->second.at(rand() % it2->second.size());

	return true;
}

//--------------------------------------------------------------------------------------
// Initiates the manoeuvre. This mostly consists of sending initial orders to all
// participating entities and everything that is involved in that process, such as 
// determining targets etc.
// Returns a behaviour status code representing the current state of the initiation of the manoeuvre.
//--------------------------------------------------------------------------------------
BehaviourStatus DistractionBaseAttack::Initiate(void)
{
	SetupGroups();
	
	if(!DetermineAssemblyPoints())
	{
		return StatusFailure;
	}

	for(std::vector<Entity*>::iterator it = m_participants.begin(); it != m_participants.end(); ++it)
	{
		Order* pNewOrder = nullptr;

		if(m_sneakParticipants.find((*it)->GetId()) != m_sneakParticipants.end())
		{
			pNewOrder = new MoveOrder((*it)->GetId(), MoveToPositionOrder, MediumPriority, m_sneakAssemblyPoint);
		}else
		{
			pNewOrder = new MoveOrder((*it)->GetId(), MoveToPositionOrder, MediumPriority, m_distractionAssemblyPoint);
		}

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
BehaviourStatus DistractionBaseAttack::Update(float deltaTime)
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
			StartDistractionAttack();
		}
	}

	if(!IsActive() || HasFailed() || (GetNumberOfParticipants() < GetMinNumberOfParticipants()) || m_sneakParticipants.empty() || m_distractionParticipants.empty())
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
void DistractionBaseAttack::Terminate(void)
{
	m_timer = 0.0f;
	m_arrivedEntities.clear();
	m_currentPhase = AssemblePhase;
	m_distractionParticipants.clear();
	m_sneakParticipants.clear();

	TeamManoeuvre::Terminate();
}

//--------------------------------------------------------------------------------------
// Resets the manoeuvre. Should only be called to reset the manoeuver in between game rounds.
//--------------------------------------------------------------------------------------
void DistractionBaseAttack::Reset(void)
{
	m_timer = 0.0f;
	m_arrivedEntities.clear();
	m_currentPhase = AssemblePhase;
	m_distractionParticipants.clear();
	m_sneakParticipants.clear();

	TeamManoeuvre::Reset();
}

MultiflagCTFTeamAI* DistractionBaseAttack::GetTeamAI(void)
{
	return m_pTeamAI;
}

float DistractionBaseAttack::GetWaitForParticipantsInterval(void) const
{
	return m_waitForParticipantsInterval;
}

const XMFLOAT2& DistractionBaseAttack::GetDistractionAssemblyPoint(void) const
{
	return m_distractionAssemblyPoint;
}

const XMFLOAT2& DistractionBaseAttack::GetSneakAssemblyPoint(void) const
{
	return m_sneakAssemblyPoint;
}

unsigned int DistractionBaseAttack::GetNumberOfSneakers(void) const
{
	return m_numberOfSneakers;
}

void DistractionBaseAttack::SetTeamAI(MultiflagCTFTeamAI* pTeamAI)
{
	if(pTeamAI)
	{
		m_pTeamAI = pTeamAI;
	}
}

void DistractionBaseAttack::SetWaitForParticipantsInterval(float interval)
{
	m_waitForParticipantsInterval = interval;
}

void DistractionBaseAttack::SetDistractionAssemblyPoint(const XMFLOAT2& assemblyPoint)
{
	m_distractionAssemblyPoint = assemblyPoint;
}

void DistractionBaseAttack::SetSneakAssemblyPoint(const XMFLOAT2& assemblyPoint)
{
	m_sneakAssemblyPoint = assemblyPoint;
}
