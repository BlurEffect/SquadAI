/* 
*  Kevin Meergans, SquadAI, 2014
*  PickUpDroppedFlag.cpp
*  A team manoeuvre that orders the participating entities to move onto the position
*  of the dropped enemy flag with the goal to pick it up again in order to capture it.
*/


// Includes
#include "PickUpDroppedFlag.h"
#include "MultiflagCTFTeamAI.h"
#include "Entity.h"
#include "TestEnvironment.h"

PickUpDroppedFlag::PickUpDroppedFlag(unsigned int minNumberParticipants, unsigned int maxNumberParticipants, MultiflagCTFTeamAI* pTeamAI)
	: TeamManoeuvre(PickUpDroppedFlagManoeuvre, AttackEnemyFlagCategory, minNumberParticipants, maxNumberParticipants),
	  m_pTeamAI(pTeamAI)
{
}

PickUpDroppedFlag::~PickUpDroppedFlag(void)
{
}


//--------------------------------------------------------------------------------------
// Processes an inbox message that the manoeuvre received.
// Param1: A pointer to the message to process.
//--------------------------------------------------------------------------------------
void PickUpDroppedFlag::ProcessMessage(Message* pMessage)
{
	switch(pMessage->GetType())
	{
	case FlagReturnedMessageType:
	{
	FlagReturnedMessage* pMsg = reinterpret_cast<FlagReturnedMessage*>(pMessage);
	if(pMsg->GetData().m_flagOwner != GetTeamAI()->GetTeam())
	{
		// The flag enemy was returned -> the manoeuvre failed
		SetFailed(true);
	}
	break;
	}
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
BehaviourStatus PickUpDroppedFlag::Initiate(void)
{
	// Determine the target for the attack
	XMFLOAT2 target(0.0f, 0.0f);

	if(GetTeamAI()->GetTeam() == TeamRed)
	{
		target = GetTeamAI()->GetFlagData(TeamBlue).m_position;
	}else
	{
		target = GetTeamAI()->GetFlagData(TeamRed).m_position;
	}

	for(std::vector<Entity*>::iterator it = m_participants.begin(); it != m_participants.end(); ++it)
	{
		// Hurry to the position, where the flag was dropped. The manoeuvre send out a mixture of high priority and
		// medium priority orders.
		Order* pNewOrder = nullptr;
		
		// At the moment it is randomly decided whether the participants run to the drop position ignoring enemies
		// or engage in fights along the way.

		if(rand() % 2 == 0)
		{
			pNewOrder = new MoveOrder((*it)->GetId(), MoveToPositionOrder, MediumPriority, target);
		}else
		{
			pNewOrder = new MoveOrder((*it)->GetId(), MoveToPositionOrder, HighPriority, target);
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
BehaviourStatus PickUpDroppedFlag::Update(float deltaTime)
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
void PickUpDroppedFlag::Terminate(void)
{
	TeamManoeuvre::Terminate();
}

//--------------------------------------------------------------------------------------
// Resets the manoeuvre. Should only be called to reset the manoeuver in between game rounds.
//--------------------------------------------------------------------------------------
void PickUpDroppedFlag::Reset(void)
{
	TeamManoeuvre::Reset();
}

const MultiflagCTFTeamAI* PickUpDroppedFlag::GetTeamAI(void) const
{
	return m_pTeamAI;
}

void PickUpDroppedFlag::SetTeamAI(MultiflagCTFTeamAI* pTeamAI)
{
	if(pTeamAI)
	{
		m_pTeamAI = pTeamAI;
	}
}

