/* 
*  Kevin Meergans, SquadAI, 2014
*  ActiveBaseDefence.cpp
*  A team manoeuvre that orders the participating entities to patrol the area around the
*  base and attack any enemies that might be spotted. When one of the defenders is killed,
*  the other ones will investigate the position, where he died (if they are not involved in
*  fights themselves).
*/


// Includes
#include "ActiveBaseDefence.h"
#include "MultiflagCTFTeamAI.h"
#include "Entity.h"
#include "TestEnvironment.h"

ActiveBaseDefence::ActiveBaseDefence(unsigned int minNumberParticipants, unsigned int maxNumberParticipants, MultiflagCTFTeamAI* pTeamAI, float patrolRadius)
	: TeamManoeuvre(ActiveBaseDefenceManoeuvre, ProtectOwnFlagCategory, minNumberParticipants, maxNumberParticipants),
	  m_pTeamAI(pTeamAI),
	  m_patrolRadius(patrolRadius)
{
}

ActiveBaseDefence::~ActiveBaseDefence(void)
{
}


//--------------------------------------------------------------------------------------
// Processes an inbox message that the manoeuvre received.
// Param1: A pointer to the message to process.
//--------------------------------------------------------------------------------------
void ActiveBaseDefence::ProcessMessage(Message* pMessage)
{
	switch(pMessage->GetType())
	{
	case FlagPickedUpMessageType:
	{
		FlagPickedUpMessage* pMsg = reinterpret_cast<FlagPickedUpMessage*>(pMessage);
		if(pMsg->GetData().m_flagOwner == GetTeamAI()->GetTeam())
		{
			// The team's flag was picked up, the manoeuvre failed
			SetFailed(true);
		}
		break;
	}
	case EntityKilledMessageType:
	{
	EntityKilledMessage* pMsg = reinterpret_cast<EntityKilledMessage*>(pMessage);
	if(IsParticipant(pMsg->GetData().m_id) && pMsg->GetData().m_team == GetTeamAI()->GetTeam())
	{
		XMFLOAT2 deathPosition = GetParticipant(pMsg->GetData().m_id)->GetPosition();

		// Participants that get killed, drop out of the manoeuvre
		m_pTeamAI->ReleaseEntityFromManoeuvre(pMsg->GetData().m_id);

		// Send other participants to investigate the position, where the entity died
		InvestigatePosition(deathPosition);
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
			XMFLOAT2 movePosition(0.0f, 0.0f);
			if(!m_pTeamAI->GetTestEnvironment()->GetRandomUnblockedTargetInArea(GetTeamAI()->GetFlagData(GetTeamAI()->GetTeam()).m_basePosition, m_patrolRadius, movePosition))
			{
				m_pTeamAI->ReleaseEntityFromManoeuvre(pMsg->GetData().m_entityId);
			}else
			{
				// Set the next patrol target
				reinterpret_cast<MoveOrder*>(m_activeOrders[pMsg->GetData().m_entityId])->SetTargetPosition(movePosition);
			}
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
BehaviourStatus ActiveBaseDefence::Initiate(void)
{
	for(std::vector<Entity*>::iterator it = m_participants.begin(); it != m_participants.end(); ++it)
	{
		// Randomly pick a grid field near the base and patrol to it.

		XMFLOAT2 movePosition(0.0f, 0.0f);
		if(!m_pTeamAI->GetTestEnvironment()->GetRandomUnblockedTargetInArea(GetTeamAI()->GetFlagData(GetTeamAI()->GetTeam()).m_basePosition, m_patrolRadius, movePosition))
		{
			return StatusFailure;
		}

		Order* pNewOrder = new MoveOrder((*it)->GetId(), MoveToPositionOrder, MediumPriority, movePosition);

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
BehaviourStatus ActiveBaseDefence::Update(float deltaTime)
{
	// Keep track of who has reached the target and let those defend until all have arrived
	SortOutProcessedMessages();
	ProcessMessages();

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
void ActiveBaseDefence::Terminate(void)
{
	TeamManoeuvre::Terminate();
}

//--------------------------------------------------------------------------------------
// Resets the manoeuvre. Should only be called to reset the manoeuver in between game rounds.
//--------------------------------------------------------------------------------------
void ActiveBaseDefence::Reset(void)
{
	TeamManoeuvre::Reset();
}

//--------------------------------------------------------------------------------------
// Sends all idle participants to the provided position. Participants are consiered idle
// if they are currently not fighting any enemies.
// Param1: The position to investigate.
//--------------------------------------------------------------------------------------
void ActiveBaseDefence::InvestigatePosition(const XMFLOAT2& position)
{
	for(std::vector<Entity*>::iterator it = m_participants.begin(); it != m_participants.end(); ++it)
	{
		if(GetTeamAI()->GetSpottedEnemies().at((*it)->GetId()).empty())
		{
			// Change the movement target in the orders for the entities that should investigate the
			// provided position

			reinterpret_cast<MoveOrder*>(m_activeOrders[(*it)->GetId()])->SetTargetPosition(position);
		}
	}
}

// Data access functions

const MultiflagCTFTeamAI* ActiveBaseDefence::GetTeamAI(void) const
{
	return m_pTeamAI;
}

float ActiveBaseDefence::GetPatrolRadius(void) const
{
	return m_patrolRadius;
}

void ActiveBaseDefence::SetTeamAI(MultiflagCTFTeamAI* pTeamAI)
{
	if(pTeamAI)
	{
		m_pTeamAI = pTeamAI;
	}
}

void ActiveBaseDefence::SetPatrolRadius(float radius)
{
	m_patrolRadius = radius;
}
