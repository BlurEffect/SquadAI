/* 
*  Kevin Meergans, SquadAI, 2014
*  SimpleBaseDefence.cpp
*  A team manoeuvre that orders the participating entities to defend their base. This is a 
*  basic small-scale behaviour without any actual coordination of the participants. It was 
*  mostly implemented as a fallback manoeuvre if other defense manoeuvres are currently not available.
*/

// Includes
#include "SimpleBaseDefence.h"
#include "MultiflagCTFTeamAI.h"
#include "Entity.h"
#include "TestEnvironment.h"

SimpleBaseDefence::SimpleBaseDefence(unsigned int minNumberParticipants, unsigned int maxNumberParticipants, MultiflagCTFTeamAI* pTeamAI)
	: TeamManoeuvre(SimpleBaseDefenceManoeuvre, ProtectOwnFlagCategory, minNumberParticipants, maxNumberParticipants),
	  m_pTeamAI(pTeamAI)
{
}

SimpleBaseDefence::~SimpleBaseDefence(void)
{
}


//--------------------------------------------------------------------------------------
// Processes an inbox message that the manoeuvre received.
// Param1: A pointer to the message to process.
//--------------------------------------------------------------------------------------
void SimpleBaseDefence::ProcessMessage(Message* pMessage)
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
		if((pMsg->GetData().m_orderState == SucceededOrderState) || (pMsg->GetData().m_orderState == FailedOrderState))
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
BehaviourStatus SimpleBaseDefence::Initiate(void)
{
	if(GetTeamAI()->GetTestEnvironment()->GetBaseFieldPositions(GetTeamAI()->GetTeam()).empty())
	{
		return StatusFailure;
	}


	for(std::vector<Entity*>::iterator it = m_participants.begin(); it != m_participants.end(); ++it)
	{
		// Randomly pick a grid field that belongs to the team's base and guard it.

		XMFLOAT2 defendPosition = GetTeamAI()->GetTestEnvironment()->GetBaseFieldPositions(GetTeamAI()->GetTeam()).at(rand() % GetTeamAI()->GetTestEnvironment()->GetBaseFieldPositions(GetTeamAI()->GetTeam()).size());

		Order* pNewOrder = new DefendOrder((*it)->GetId(), DefendPositionOrder, MediumPriority, defendPosition, XMFLOAT2(0.0f, 0.0f));

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
BehaviourStatus SimpleBaseDefence::Update(float deltaTime)
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
void SimpleBaseDefence::Terminate(void)
{
	TeamManoeuvre::Terminate();
}

//--------------------------------------------------------------------------------------
// Resets the manoeuvre. Should only be called to reset the manoeuver in between game rounds.
//--------------------------------------------------------------------------------------
void SimpleBaseDefence::Reset(void)
{
	TeamManoeuvre::Reset();
}

MultiflagCTFTeamAI* SimpleBaseDefence::GetTeamAI(void)
{
	return m_pTeamAI;
}

void SimpleBaseDefence::SetTeamAI(MultiflagCTFTeamAI* pTeamAI)
{
	if(pTeamAI)
	{
		m_pTeamAI = pTeamAI;
	}
}

