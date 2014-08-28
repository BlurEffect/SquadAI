/* 
*  Kevin Meergans, SquadAI, 2014
*  TestManoeuvre.cpp
*  Just for testing.
*/

// Includes
#include "TestManoeuvre.h"
#include "Entity.h"
#include "Order.h"
#include "TeamAI.h"


TestManoeuvre::TestManoeuvre(unsigned int minNumberParticipants, unsigned int maxNumberParticipants, TeamAI* pTeamAI)
	: TeamManoeuvre(TestAllMoveManoeuvre, AttackEnemyFlagCategory, minNumberParticipants, maxNumberParticipants),
	  m_pTeamAI(pTeamAI),
	  m_entitiesReachedDestinationCount(0)
{
}

TestManoeuvre::~TestManoeuvre(void)
{
}


//--------------------------------------------------------------------------------------
// Processes an inbox message that the manoeuvre received.
// Param1: A pointer to the message to process.
//--------------------------------------------------------------------------------------
void TestManoeuvre::ProcessMessage(Message* pMessage)
{
	switch(pMessage->GetType())
	{
	case UpdateOrderStateMessageType:
	{
	// Cancel old order, Send Follow-Up Orders, finish manoeuvre etc
	UpdateOrderStateMessage* pMsg = reinterpret_cast<UpdateOrderStateMessage*>(pMessage);
	if(pMsg->GetData().m_orderState == SucceededOrderState)
	{
		// Officially cancel the old order that was fulfilled and delete it.
		CancelOrder(pMsg->GetData().m_entityId);
		m_activeOrders.erase(m_activeOrders.find(pMsg->GetData().m_entityId));

		if(m_pTeamAI->GetTeam() == TeamRed)
		{
			// The entity reached its destination, let it wait until all entities have arrived.
			++m_entitiesReachedDestinationCount;
			Order* pNewOrder = new DefendOrder(pMsg->GetData().m_entityId, DefendPositionOrder, MediumPriority, XMFLOAT2(20.0f, 20.0f), XMFLOAT2(0.0f,0.0f));
		
			if(!pNewOrder)
			{
				SetActive(false);
			}
		
			// Find the participant
			std::vector<Entity*>::iterator foundIt = std::find_if(m_participants.begin(), m_participants.end(), Entity::FindEntityById(pMsg->GetData().m_entityId));

			FollowOrderMessageData data(pNewOrder);
			SendMessage(*foundIt, FollowOrderMessageType, &data);

			m_activeOrders.insert(std::pair<unsigned long, Order*>(pMsg->GetData().m_entityId, pNewOrder));
		}
		if(m_entitiesReachedDestinationCount == GetNumberOfParticipants())
		{
			// All entities reached the destination, initiate termination of the manoeuvre
			SetActive(false);
			SetSucceeded(true);
		}

	}else if(pMsg->GetData().m_orderState == FailedOrderState)
	{
		// The order failed -> release the entity from the manoeuvre
		m_pTeamAI->ReleaseEntityFromManoeuvre(pMsg->GetData().m_entityId);
	}
	break;
	}
	default:
		TeamManoeuvre::ProcessMessage(pMessage);
	}

	// Update any active attack orders?
	// Send cancel if order update received?
}

//--------------------------------------------------------------------------------------
// Initiates the manoeuvre. This mostly consists of sending initial orders to all
// participating entities and everything that is involved in that process, such as 
// determining targets etc.
// Returns a behaviour status code representing the current state of the initiation of the manoeuvre.
//--------------------------------------------------------------------------------------
BehaviourStatus TestManoeuvre::Initiate(void)
{
	if(m_pTeamAI->GetTeam() == TeamRed)
	{
		for(std::vector<Entity*>::iterator it = m_participants.begin(); it != m_participants.end(); ++it)
		{
			Order* pNewOrder = new MoveOrder((*it)->GetId(), MoveToPositionOrder, MediumPriority, XMFLOAT2(20.0f, 20.0f));
			
			if(!pNewOrder)
			{
				return StatusFailure;
			}

			FollowOrderMessageData data(pNewOrder);
			SendMessage(*it, FollowOrderMessageType, &data);

			m_activeOrders.insert(std::pair<unsigned long, Order*>((*it)->GetId(), pNewOrder));
		}
	}else
	{
		for(std::vector<Entity*>::iterator it = m_participants.begin(); it != m_participants.end(); ++it)
		{
			Order* pNewOrder = new MoveOrder((*it)->GetId(), MoveToPositionOrder, HighPriority, XMFLOAT2(-20.0f, -20.0f));
			
			if(!pNewOrder)
			{
				return StatusFailure;
			}

			FollowOrderMessageData data(pNewOrder);
			SendMessage(*it, FollowOrderMessageType, &data);

			m_activeOrders.insert(std::pair<unsigned long, Order*>((*it)->GetId(), pNewOrder));
			
		}
		
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
BehaviourStatus TestManoeuvre::Update(float deltaTime)
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
	//TestManoeuvre::Update(deltaTime);
}

//--------------------------------------------------------------------------------------
// Terminates the manoeuvre. This mostly consists of cancelling all active orders and
// removing all participating entities.
//--------------------------------------------------------------------------------------
void TestManoeuvre::Terminate(void)
{
	m_entitiesReachedDestinationCount = 0;
	TeamManoeuvre::Terminate();
}
