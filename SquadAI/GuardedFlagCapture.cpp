/* 
*  Kevin Meergans, SquadAI, 2014
*  GuardedFlagCapture.cpp
*  A team manoeuvre that orders the flag-carrying participant to run to the home base in 
*  order to capture the flag while the rest of the participating entities attempts to 
*  protect the flag carrier from any enemies trying to intercept him.
*/


// Includes
#include "GuardedFlagCapture.h"
#include "MultiflagCTFTeamAI.h"
#include "Entity.h"
#include "TestEnvironment.h"

GuardedFlagCapture::GuardedFlagCapture(unsigned int minNumberParticipants, unsigned int maxNumberParticipants, MultiflagCTFTeamAI* pTeamAI, float guardRadius, float updateMovementTargetsInterval)
	: TeamManoeuvre(GuardedFlagCaptureManoeuvre, AttackEnemyFlagCategory, minNumberParticipants, maxNumberParticipants),
	  m_pTeamAI(pTeamAI),
	  m_guardRadius(guardRadius),
	  m_updateMovementTargetsInterval(updateMovementTargetsInterval)
{
}

GuardedFlagCapture::~GuardedFlagCapture(void)
{
}


//--------------------------------------------------------------------------------------
// Processes an inbox message that the manoeuvre received.
// Param1: A pointer to the message to process.
//--------------------------------------------------------------------------------------
void GuardedFlagCapture::ProcessMessage(Message* pMessage)
{
	switch(pMessage->GetType())
	{
	case FlagDroppedMessageType:
	{
		FlagDroppedMessage* pMsg = reinterpret_cast<FlagDroppedMessage*>(pMessage);
		if(pMsg->GetData().m_flagOwner != GetTeamAI()->GetTeam())
		{
			// The flag was dropped, the manoeuvre failed
			SetFailed(true);
		}
		break;
	}
	case ScoreUpdateMessageType:
	{
		ScoreUpdateMessage* pMsg = reinterpret_cast<ScoreUpdateMessage*>(pMessage);
		if(pMsg->GetData().m_team == GetTeamAI()->GetTeam())
		{
			// The flag was captured -> the manoeuvre succeeded
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
	case AttackedByEnemyMessageType:
	{
		AttackedByEnemyMessage* pMsg = reinterpret_cast<AttackedByEnemyMessage*>(pMessage);

		if(pMsg->GetData().m_entityId == m_flagCarrierId)
		{
			// The flag carrier is being attacked, protect him.

			// Get the attack direction
			XMFLOAT2 viewDirection(0.0f, 0.0f);
			XMStoreFloat2(&viewDirection, XMLoadFloat2(&pMsg->GetData().m_attackPosition) - XMLoadFloat2(&GetParticipant(pMsg->GetData().m_entityId)->GetPosition()));

			// Send all protectors to the position of the attacker to protect the carrier
			for(std::vector<Entity*>::iterator it = m_participants.begin(); it != m_participants.end(); ++it)
			{
				if((*it)->GetId() != m_flagCarrierId)
				{
					// Change the movement target in the orders for the entities
					reinterpret_cast<MoveOrder*>(m_activeOrders[(*it)->GetId()])->SetTargetPosition(pMsg->GetData().m_attackPosition);
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
			// Let the entity wait for the next movement update (switch to defend?)
		}else if(pMsg->GetData().m_orderState == FailedOrderState)
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
BehaviourStatus GuardedFlagCapture::Initiate(void)
{
	EntityTeam enemyTeam = (GetTeamAI()->GetTeam() == TeamRed) ? (TeamBlue) : (TeamRed);

	m_flagCarrierId = GetTeamAI()->GetFlagData(enemyTeam).m_carrierId;

	XMFLOAT2 flagCarrierPosition = GetParticipant(m_flagCarrierId)->GetPosition();

	for(std::vector<Entity*>::iterator it = m_participants.begin(); it != m_participants.end(); ++it)
	{
		Order* pNewOrder = nullptr;

		if((*it)->GetId() != m_flagCarrierId)
		{
			// Orders for the protectors

			XMFLOAT2 movementTarget(0.0f, 0.0f);
			if(!m_pTeamAI->GetTestEnvironment()->GetRandomUnblockedTargetInArea(flagCarrierPosition, m_guardRadius, movementTarget))
			{
				return StatusFailure;
			}

			pNewOrder = new MoveOrder((*it)->GetId(), MoveToPositionOrder, MediumPriority, movementTarget);
			
		}else
		{
			// Orders for the flag carrier

			// Move to the home base but attack enemies if they get in the way
			// Note: In future versions the manoeuvre should send the carrier on a path, where he is less likely to
			//       encounter enemies.
			pNewOrder = new MoveOrder((*it)->GetId(), MoveToPositionOrder, MediumPriority, GetTeamAI()->GetFlagData(GetTeamAI()->GetTeam()).m_basePosition);
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
BehaviourStatus GuardedFlagCapture::Update(float deltaTime)
{
	// Keep track of who has reached the target and let those defend until all have arrived
	SortOutProcessedMessages();
	ProcessMessages();

	m_timer += deltaTime;

	if(m_updateMovementTargetsInterval != 0.0f && m_timer >= m_updateMovementTargetsInterval)
	{
		UpdateMovementTargets();
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
// Terminates the manoeuvre. This mostly consists of cancelling all active orders and
// removing all participating entities.
//--------------------------------------------------------------------------------------
void GuardedFlagCapture::Terminate(void)
{
	m_timer = 0.0f;
	TeamManoeuvre::Terminate();
}

//--------------------------------------------------------------------------------------
// Resets the manoeuvre. Should only be called to reset the manoeuver in between game rounds.
//--------------------------------------------------------------------------------------
void GuardedFlagCapture::Reset(void)
{
	m_timer = 0.0f;
	TeamManoeuvre::Reset();
}

//--------------------------------------------------------------------------------------
// Updates the movement targets for the protectors to move to new guard positions closer
// to the current position of the flag carrier.
//--------------------------------------------------------------------------------------
void GuardedFlagCapture::UpdateMovementTargets(void)
{
	XMFLOAT2 flagCarrierPosition = GetParticipant(m_flagCarrierId)->GetPosition();

	for(std::vector<Entity*>::iterator it = m_participants.begin(); it != m_participants.end(); ++it)
	{
		if((*it)->GetId() != m_flagCarrierId)
		{
			XMFLOAT2 newMovementTarget(0.0f, 0.0f);
			
			if(!m_pTeamAI->GetTestEnvironment()->GetRandomUnblockedTargetInArea(flagCarrierPosition, m_guardRadius, newMovementTarget))
			{
				m_pTeamAI->ReleaseEntityFromManoeuvre((*it)->GetId());
			}

			// Change the movement target in the orders for the entities
			reinterpret_cast<MoveOrder*>(m_activeOrders[(*it)->GetId()])->SetTargetPosition(newMovementTarget);
		}
	}
}

// Data access functions

const MultiflagCTFTeamAI* GuardedFlagCapture::GetTeamAI(void) const
{
	return m_pTeamAI;
}

float GuardedFlagCapture::GetGuardRadius(void) const
{
	return m_guardRadius;
}

float GuardedFlagCapture::GetUpdateMovementTargetsInterval(void) const
{
	return m_updateMovementTargetsInterval;
}

void GuardedFlagCapture::SetTeamAI(MultiflagCTFTeamAI* pTeamAI)
{
	if(pTeamAI)
	{
		m_pTeamAI = pTeamAI;
	}
}

void GuardedFlagCapture::SetGuardRadius(float radius)
{
	m_guardRadius = radius;
}

void GuardedFlagCapture::SetUpdateMovementTargetsInterval(float interval)
{
	m_updateMovementTargetsInterval = interval;
}