/* 
*  Kevin Meergans, SquadAI, 2014
*  RunTheFlagHome.cpp
*  A flag carrier behaviour that has the corresponding entity hurrying to its home base
*  in order to capture the flag.
*/

// Includes
#include "RunTheFlagHome.h"
#include "MultiflagCTFTeamAI.h"
#include "Entity.h"
#include "TestEnvironment.h"

RunTheFlagHome::RunTheFlagHome(unsigned int minNumberParticipants, unsigned int maxNumberParticipants, MultiflagCTFTeamAI* pTeamAI)
	: TeamManoeuvre(RunTheFlagHomeManoeuvre, minNumberParticipants, maxNumberParticipants),
	  m_pTeamAI(pTeamAI)
{
}

RunTheFlagHome::~RunTheFlagHome(void)
{
}


//--------------------------------------------------------------------------------------
// Processes an inbox message that the manoeuvre received.
// Param1: A pointer to the message to process.
//--------------------------------------------------------------------------------------
void RunTheFlagHome::ProcessMessage(Message* pMessage)
{
	switch(pMessage->GetType())
	{
	case ScoreUpdateMessageType:
	{
	ScoreUpdateMessage* pMsg = reinterpret_cast<ScoreUpdateMessage*>(pMessage);
	if(pMsg->GetData().m_team == GetTeamAI()->GetTeam())
	{
		// The flag was captured -> the manoeuvre succeeded
		SetActive(false);
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
		
			// The entity arrived at the home base, take on a defense position until the flag is captured
			// (might have to wait for the own flag to be returned)

			Order* pNewOrder = new DefendOrder(pMsg->GetData().m_entityId, DefendPositionOrder, MediumPriority, XMFLOAT2(GetTeamAI()->GetFlagData(GetTeamAI()->GetTeam()).m_basePosition), XMFLOAT2(0.0f, 0.0f));
			
			if(!pNewOrder)
			{
				SetActive(false);
			}else
			{
				// Find the entity
				std::vector<Entity*>::iterator foundIt = std::find_if(m_participants.begin(), m_participants.end(), Entity::FindEntityById(pMsg->GetData().m_entityId));

				FollowOrderMessageData data(pNewOrder);
				SendMessage(*foundIt, FollowOrderMessageType, &data);

				m_activeOrders.insert(std::pair<unsigned long, Order*>(pMsg->GetData().m_entityId, pNewOrder));
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
// Initiates the manoeuvre. This mostly consists of sending initial orders to all
// participating entities and everything that is involved in that process, such as 
// determining targets etc.
//--------------------------------------------------------------------------------------
void RunTheFlagHome::Initiate(void)
{
	for(std::vector<Entity*>::iterator it = m_participants.begin(); it != m_participants.end(); ++it)
	{
		// Run the flag to the home base ignoring all enemies
		// Note: In future versions the manoeuvre should send the carrier on a path, where he is less likely to
		//       encounter enemies.
		Order* pNewOrder = new MoveOrder((*it)->GetId(), MoveToPositionOrder, HighPriority, XMFLOAT2(GetTeamAI()->GetFlagData(GetTeamAI()->GetTeam()).m_basePosition));
			
		if(!pNewOrder)
		{
			SetActive(false);
			return;
		}

		FollowOrderMessageData data(pNewOrder);
		SendMessage(*it, FollowOrderMessageType, &data);

		m_activeOrders.insert(std::pair<unsigned long, Order*>((*it)->GetId(), pNewOrder));
	}
	
	SetActive(true);
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
BehaviourStatus RunTheFlagHome::Update(float deltaTime)
{
	// Keep track of who has reached the target and let those defend until all have arrived
	SortOutProcessedMessages();
	ProcessMessages();

	if((!IsActive() && !HasSucceeded()) || (GetNumberOfParticipants() < GetMinNumberOfParticipants()))
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
void RunTheFlagHome::Terminate(void)
{
	TeamManoeuvre::Terminate();
}

//--------------------------------------------------------------------------------------
// Resets the manoeuvre. Should only be called to reset the manoeuver in between game rounds.
//--------------------------------------------------------------------------------------
void RunTheFlagHome::Reset(void)
{
	TeamManoeuvre::Reset();
}

const MultiflagCTFTeamAI* RunTheFlagHome::GetTeamAI(void) const
{
	return m_pTeamAI;
}

void RunTheFlagHome::SetTeamAI(MultiflagCTFTeamAI* pTeamAI)
{
	if(pTeamAI)
	{
		m_pTeamAI = pTeamAI;
	}
}

