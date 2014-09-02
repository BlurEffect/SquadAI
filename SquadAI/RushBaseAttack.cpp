/* 
*  Kevin Meergans, SquadAI, 2014
*  RushBaseAttack.cpp
*  A team manoeuvre that orders the participating entities to assemble and perform an
*  all-out attack on the enemy flag trying to steal it by brute force/sheer number.
*/

// Includes
#include "RushBaseAttack.h"
#include "MultiflagCTFTeamAI.h"
#include "Entity.h"
#include "TestEnvironment.h"

RushBaseAttack::RushBaseAttack(unsigned int minNumberParticipants, unsigned int maxNumberParticipants, MultiflagCTFTeamAI* pTeamAI, float waitForParticipantsInterval)
	: TeamManoeuvre(RushBaseAttackManoeuvre, AttackEnemyFlagCategory, minNumberParticipants, maxNumberParticipants),
	  m_pTeamAI(pTeamAI),
	  m_assemblyPoint(0.0f, 0.0f),
	  m_waitForParticipantsInterval(waitForParticipantsInterval),
	  m_timer(0.0f),
	  m_currentPhase(AssemblePhase)
{
}

RushBaseAttack::~RushBaseAttack(void)
{
}


//--------------------------------------------------------------------------------------
// Processes an inbox message that the manoeuvre received.
// Param1: A pointer to the message to process.
//--------------------------------------------------------------------------------------
void RushBaseAttack::ProcessMessage(Message* pMessage)
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
				// Register the entity as having reached the assembly point
				m_arrivedEntities.insert(pMsg->GetData().m_entityId);
			
				//
				// Send a new defend order to let the entity wait for other entities
				// It's a low priority order as it is sufficient if the entities are in the same area when the actual rush attack begins.
				Order* pNewOrder = new DefendOrder(pMsg->GetData().m_entityId, DefendPositionOrder, LowPriority, XMFLOAT2(m_assemblyPoint), XMFLOAT2(0.0f,0.0f));
		
				if(!pNewOrder)
				{
					SetFailed(true);
				}
		
				// Find the participant
				std::vector<Entity*>::iterator foundIt = std::find_if(m_participants.begin(), m_participants.end(), Entity::FindEntityById(pMsg->GetData().m_entityId));

				FollowOrderMessageData data(pNewOrder);
				SendMessage(*foundIt, FollowOrderMessageType, &data);

				m_activeOrders.insert(std::pair<unsigned long, Order*>(pMsg->GetData().m_entityId, pNewOrder));
				//
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
void RushBaseAttack::StartAttack(void)
{
	// Cancel all old orders
	for(std::vector<Entity*>::iterator it = m_participants.begin(); it != m_participants.end(); ++it)
	{
		CancelOrder((*it)->GetId());
		m_activeOrders.erase(m_activeOrders.find((*it)->GetId()));
	}

	// Clear all current orders
	//ClearOrders();

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
// Determines the point at which to assemble the participants before sending them out
// to attack the enemy base.
//--------------------------------------------------------------------------------------
void RushBaseAttack::DetermineAssemblyPoint(void)
{
	// Randomly choose one of the attack positions set in edit mode as assembly point for the
	// rush attack.

	unsigned int randIndex = rand() % m_pTeamAI->GetTestEnvironment()->GetAttackPositions(GetTeamAI()->GetTeam()).size();
	std::unordered_map<Direction, std::vector<XMFLOAT2>>::const_iterator it = m_pTeamAI->GetTestEnvironment()->GetAttackPositions(GetTeamAI()->GetTeam()).begin();
	std::advance(it, randIndex);

	m_assemblyPoint = it->second.at(rand() % it->second.size());

	/*
	// Get the enemy flag base position
	XMFLOAT2 redBasePosition = GetTeamAI()->GetFlagData(TeamRed).m_basePosition;
	XMFLOAT2 blueBasePosition = GetTeamAI()->GetFlagData(TeamBlue).m_basePosition;
	
	XMFLOAT2 friendlyBasePos(0.0f, 0.0f);
	XMFLOAT2 enemyBasePos(0.0f, 0.0f);

	if(GetTeamAI()->GetTeam() == TeamRed)
	{
		friendlyBasePos = GetTeamAI()->GetFlagData(TeamRed).m_basePosition;
		enemyBasePos    = GetTeamAI()->GetFlagData(TeamBlue).m_basePosition;
	}else
	{
		friendlyBasePos = GetTeamAI()->GetFlagData(TeamBlue).m_basePosition;
		enemyBasePos    = GetTeamAI()->GetFlagData(TeamRed).m_basePosition;
	}


	XMVECTOR baseToBaseVector = XMLoadFloat2(&enemyBasePos) - XMLoadFloat2(&friendlyBasePos);

	float gridWidth = GetTeamAI()->GetTestEnvironment()->GetGridSize();

	XMFLOAT2 normOrthoVector(0.0f, 0.0f);
	XMStoreFloat2(&normOrthoVector, XMVector2Orthogonal(XMVector2Normalize(baseToBaseVector)));

	XMFLOAT2 finalDirection(0.0f, 0.0f);

	do
	{
		float randomAngle = XMConvertToRadians(static_cast<float>(rand() % 180));
		
		float sine = sin(randomAngle);
		float cosine = cos(randomAngle);

		// rotate point
		finalDirection.x = normOrthoVector.x * cosine - normOrthoVector.y * sine;
		finalDirection.y = normOrthoVector.x * sine + normOrthoVector.y * cosine;

		//finalDirection.x = normOrthoVector.x * cosine + normOrthoVector.y * sine;
		//finalDirection.y = -normOrthoVector.x * sine + normOrthoVector.y * cosine;

		// Add a bit of randomisation
		XMStoreFloat2(&m_assemblyPoint, XMLoadFloat2(&enemyBasePos) + XMLoadFloat2(&finalDirection) * (m_assemblyPointDistance + ((rand() % 200 - 100) * 0.01f) * GetTeamAI()->GetTestEnvironment()->GetGridSpacing() * 4.0f)); 

	}while(GetTeamAI()->GetTestEnvironment()->IsBlocked(m_assemblyPoint));
	*/

	/*
	// Get the enemy flag base position
	XMFLOAT2 redBasePosition = GetTeamAI()->GetFlagData(TeamRed).m_basePosition;
	XMFLOAT2 blueBasePosition = GetTeamAI()->GetFlagData(TeamBlue).m_basePosition;
	
	XMVECTOR baseToBaseVector;
	
	if(GetTeamAI()->GetTeam() == TeamRed)
	{
		baseToBaseVector = XMLoadFloat2(&redBasePosition) - XMLoadFloat2(&blueBasePosition);
	}else
	{
		baseToBaseVector = XMLoadFloat2(&blueBasePosition) - XMLoadFloat2(&redBasePosition);
	}
	
	XMFLOAT2 basePoint(0.0f, 0.0f);
	XMStoreFloat2(&basePoint, XMVector2Normalize(baseToBaseVector) * m_assemblyPointDistance);

	float gridWidth = GetTeamAI()->GetTestEnvironment()->GetGridSize();

	XMVECTOR orthoVector = XMVector2Normalize(XMVector2Orthogonal(XMLoadFloat2(&basePoint)));

	do
	{
		XMStoreFloat2(&m_assemblyPoint, XMLoadFloat2(&basePoint) + (orthoVector * (((rand() % 200 - 100) * 0.01) * gridWidth * 0.5)));
	}while(GetTeamAI()->GetTestEnvironment()->IsBlocked(m_assemblyPoint));
	*/

	//m_assemblyPointDistance

	// Note: Another approach to this could include annotations placed in the node graph to
	//       mark good assembly points that are close to an enemy base but well covered.

	/*
	if(GetTeamAI()->GetTeam() == TeamRed)
	{
		m_assemblyPoint = XMFLOAT2(-20.0f, 20.0f);
	}else
	{
		m_assemblyPoint = XMFLOAT2(20.0f, -20.0f);
	}
	*/
}

//--------------------------------------------------------------------------------------
// Initiates the manoeuvre. This mostly consists of sending initial orders to all
// participating entities and everything that is involved in that process, such as 
// determining targets etc.
// Returns a behaviour status code representing the current state of the initiation of the manoeuvre.
//--------------------------------------------------------------------------------------
BehaviourStatus RushBaseAttack::Initiate(void)
{
	DetermineAssemblyPoint();

	for(std::vector<Entity*>::iterator it = m_participants.begin(); it != m_participants.end(); ++it)
	{
		Order* pNewOrder = new MoveOrder((*it)->GetId(), MoveToPositionOrder, MediumPriority, m_assemblyPoint);
			
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
BehaviourStatus RushBaseAttack::Update(float deltaTime)
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
void RushBaseAttack::Terminate(void)
{
	m_timer = 0.0f;
	m_arrivedEntities.clear();
	m_currentPhase = AssemblePhase;

	TeamManoeuvre::Terminate();
}

//--------------------------------------------------------------------------------------
// Resets the manoeuvre. Should only be called to reset the manoeuver in between game rounds.
//--------------------------------------------------------------------------------------
void RushBaseAttack::Reset(void)
{
	m_timer = 0.0f;
	m_arrivedEntities.clear();
	m_currentPhase = AssemblePhase;

	TeamManoeuvre::Reset();
}

const MultiflagCTFTeamAI* RushBaseAttack::GetTeamAI(void) const
{
	return m_pTeamAI;
}

float RushBaseAttack::GetWaitForParticipantsInterval(void) const
{
	return m_waitForParticipantsInterval;
}

const XMFLOAT2& RushBaseAttack::GetAssemblyPoint(void) const
{
	return m_assemblyPoint;
}

void RushBaseAttack::SetTeamAI(MultiflagCTFTeamAI* pTeamAI)
{
	if(pTeamAI)
	{
		m_pTeamAI = pTeamAI;
	}
}

void RushBaseAttack::SetWaitForParticipantsInterval(float interval)
{
	m_waitForParticipantsInterval = interval;
}

void RushBaseAttack::SetAssemblyPoint(const XMFLOAT2& assemblyPoint)
{
	m_assemblyPoint = assemblyPoint;
}
