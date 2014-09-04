/* 
*  Kevin Meergans, SquadAI, 2014
*  InterceptFlagCarrier.cpp
*  A team manoeuvre that orders the participating entities prevent the enemy flag 
*  carrier from reaching his destination and capturing the flag of the team.
*/

// Includes
#include "InterceptFlagCarrier.h"
#include "MultiflagCTFTeamAI.h"
#include "Entity.h"
#include "TestEnvironment.h"

InterceptFlagCarrier::InterceptFlagCarrier(unsigned int minNumberParticipants, unsigned int maxNumberParticipants, MultiflagCTFTeamAI* pTeamAI, float searchRadius, float updateCarrierPositionInterval)
	: TeamManoeuvre(InterceptFlagCarrierManoeuvre, ProtectOwnFlagCategory, minNumberParticipants, maxNumberParticipants),
	  m_currentPhase(HuntPhase),  
      m_pTeamAI(pTeamAI),
	  m_timer(0.0f),
	  m_enemyFlagCarrierId(0),
	  m_searchRadius(searchRadius),
	  m_updateCarrierPositionInterval(updateCarrierPositionInterval)
{
}

InterceptFlagCarrier::~InterceptFlagCarrier(void)
{
}


//--------------------------------------------------------------------------------------
// Processes an inbox message that the manoeuvre received.
// Param1: A pointer to the message to process.
//--------------------------------------------------------------------------------------
void InterceptFlagCarrier::ProcessMessage(Message* pMessage)
{
	switch(pMessage->GetType())
	{
	case ScoreUpdateMessageType:
	{
		ScoreUpdateMessage* pMsg = reinterpret_cast<ScoreUpdateMessage*>(pMessage);
		if(pMsg->GetData().m_team != GetTeamAI()->GetTeam())
		{
			// The flag was captured by the enemy -> the manoeuvre failed
			SetFailed(true);
		}
		break;
	}
	case FlagDroppedMessageType:
	{
		FlagDroppedMessage* pMsg = reinterpret_cast<FlagDroppedMessage*>(pMessage);
		if(pMsg->GetData().m_flagOwner == GetTeamAI()->GetTeam())
		{
			// The flag was dropped (enemy flag carrier killed) -> the manoeuvre succeeded
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
	}
	break;
	}
	case EnemySpottedMessageType:
	{
		EnemySpottedMessage* pMsg = reinterpret_cast<EnemySpottedMessage*>(pMessage);
		if(pMsg->GetData().m_enemyId == m_enemyFlagCarrierId)
		{
			// The enemy flag carrier was spotted, get back to hunt him if the contact was lost before
			if(m_currentPhase == SearchPhase)
			{
				m_currentPhase = HuntPhase;
				UpdateAttackOrders(pMsg->GetData().m_enemyPosition, HighPriority);
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
		if((m_currentPhase = HuntPhase) || (pMsg->GetData().m_orderState == FailedOrderState))
		{
			// An attack order failed, that means that the target was not at the last known position and is
			// now at an unknown position. Send the entities to the enemy base in an attempt to find the 
			// carrier along the way and intercept him before he reaches his goal.
			
			EntityTeam enemyTeam = (GetTeamAI()->GetTeam() == TeamRed) ? (TeamBlue) : (TeamRed);
			
			m_currentPhase = SearchPhase;
			UpdateAttackOrders(GetTeamAI()->GetFlagData(enemyTeam).m_basePosition, HighPriority);
		}else if((m_currentPhase = SearchPhase) || (pMsg->GetData().m_orderState == FailedOrderState))
		{
			// Arrived at the enemy base without spotting the enemy carrier.
			// Patrol the area and look for the carrier, but also fight other enemies if they get in the way.
			
			EntityTeam enemyTeam = (GetTeamAI()->GetTeam() == TeamRed) ? (TeamBlue) : (TeamRed);
			// Get a random position close to the base and patrol it.
			XMFLOAT2 patrolPosition(0.0f, 0.0f);
			if(!(m_pTeamAI->GetTestEnvironment()->GetRandomUnblockedTargetInArea(GetTeamAI()->GetFlagData(enemyTeam).m_basePosition, m_searchRadius, patrolPosition)))
			{
				// An error occurred
				SetFailed(true);
			}else
			{
				UpdateAttackOrders(patrolPosition, MediumPriority);
			}

		}else if((pMsg->GetData().m_orderState == SucceededOrderState) || (pMsg->GetData().m_orderState == FailedOrderState))
		{
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
// Initiates the manoeuvre. This mostly consists of sending initial orders to all
// participating entities and everything that is involved in that process, such as 
// determining targets etc.
// Returns a behaviour status code representing the current state of the initiation of the manoeuvre.
//--------------------------------------------------------------------------------------
BehaviourStatus InterceptFlagCarrier::Initiate(void)
{
	m_enemyFlagCarrierId = GetTeamAI()->GetFlagData(GetTeamAI()->GetTeam()).m_carrierId;
	
	if(!SendOutAttackOrders(m_pTeamAI->GetFlagData(GetTeamAI()->GetTeam()).m_position))
	{
		return StatusFailure;
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
BehaviourStatus InterceptFlagCarrier::Update(float deltaTime)
{
	// Keep track of who has reached the target and let those defend until all have arrived
	SortOutProcessedMessages();
	ProcessMessages();

	m_timer += deltaTime;

	if(m_updateCarrierPositionInterval != 0.0f && m_timer >= m_updateCarrierPositionInterval)
	{
		if(m_currentPhase == HuntPhase)
		{
			UpdateAttackOrders(GetTeamAI()->GetFlagData(GetTeamAI()->GetTeam()).m_position, HighPriority);
		}
		
		m_timer = 0.0f;
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
// Issues attack orders to command all participants to attack the enemy flag carrier
// at the provided position.
// Param1: The position, at which the flag carrier was seen last.
// Returns true if all attack order were sent out successfully, flase otherwise.
//--------------------------------------------------------------------------------------
bool InterceptFlagCarrier::SendOutAttackOrders(const XMFLOAT2& attackPosition)
{
	m_currentPhase = HuntPhase;

	for(std::vector<Entity*>::iterator it = m_participants.begin(); it != m_participants.end(); ++it)
	{
		Order* pNewOrder = new AttackOrder((*it)->GetId(), AttackEnemyOrder, HighPriority, m_enemyFlagCarrierId, attackPosition);

		if(!pNewOrder)
		{
			return false;
		}

		FollowOrderMessageData data(pNewOrder);
		SendMessage(*it, FollowOrderMessageType, &data);

		m_activeOrders.insert(std::pair<unsigned long, Order*>((*it)->GetId(), pNewOrder));
	}

	return true;
}

//--------------------------------------------------------------------------------------
// Updates the attack orders for all participants with a new last known position of the
// enemy flag carrier.
// Param1: The position, at which the flag carrier was seen last.
// Param2: The priority to set.
//--------------------------------------------------------------------------------------
void InterceptFlagCarrier::UpdateAttackOrders(const XMFLOAT2& attackPosition, OrderPriority priority)
{
	for(std::vector<Entity*>::iterator it = m_participants.begin(); it != m_participants.end(); ++it)
	{
		reinterpret_cast<AttackOrder*>(m_activeOrders[(*it)->GetId()])->SetOrderPriority(priority);
		reinterpret_cast<AttackOrder*>(m_activeOrders[(*it)->GetId()])->SetEnemyPosition(attackPosition);
	}
}

//--------------------------------------------------------------------------------------
// Terminates the manoeuvre. This mostly consists of cancelling all active orders and
// removing all participating entities.
//--------------------------------------------------------------------------------------
void InterceptFlagCarrier::Terminate(void)
{
	m_timer = 0.0f;
	m_currentPhase = HuntPhase;
	m_enemyFlagCarrierId = 0;
	TeamManoeuvre::Terminate();
}

//--------------------------------------------------------------------------------------
// Resets the manoeuvre. Should only be called to reset the manoeuver in between game rounds.
//--------------------------------------------------------------------------------------
void InterceptFlagCarrier::Reset(void)
{
	m_timer = 0.0f;
	m_currentPhase = HuntPhase;
	m_enemyFlagCarrierId = 0;
	TeamManoeuvre::Reset();
}

const MultiflagCTFTeamAI* InterceptFlagCarrier::GetTeamAI(void) const
{
	return m_pTeamAI;
}

float InterceptFlagCarrier::GetSearchRadius(void) const
{
	return m_searchRadius;
}

float InterceptFlagCarrier::GetUpdateCarrierPositionInterval(void) const
{
	return m_updateCarrierPositionInterval;
}

void InterceptFlagCarrier::SetTeamAI(MultiflagCTFTeamAI* pTeamAI)
{
	if(pTeamAI)
	{
		m_pTeamAI = pTeamAI;
	}
}

void InterceptFlagCarrier::SetSearchRadius(float radius)
{
	m_searchRadius = radius;
}
	
void InterceptFlagCarrier::SetUpdateCarrierPositionInterval(float interval)
{
	m_updateCarrierPositionInterval = interval;
}

