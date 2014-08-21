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
	: TeamManoeuvre(TestAllMoveManoeuvre, minNumberParticipants, maxNumberParticipants),
	  m_pTeamAI(pTeamAI)
{
}

TestManoeuvre::~TestManoeuvre(void)
{
}


//--------------------------------------------------------------------------------------
// Processes an inbox message that the team AI received.
// Param1: A pointer to the message to process.
// Returns true if this was the final communicator to process the message, false if the
// message was forwarded to another one.
//--------------------------------------------------------------------------------------
bool TestManoeuvre::ProcessMessage(Message* pMessage)
{
	
	return TeamManoeuvre::ProcessMessage(pMessage);
	// Update any active attack orders?
	// Send cancel if order update received?
}

//--------------------------------------------------------------------------------------
// Initiates the manoeuvre. This mostly consists of sending initial orders to all
// participating entities and everything that is involved in that process, such as 
// determining targets etc.
//--------------------------------------------------------------------------------------
void TestManoeuvre::Initiate(void)
{
	if(m_pTeamAI->GetTeam() == TeamRed)
	{
		for(std::vector<Entity*>::iterator it = m_participants.begin(); it != m_participants.end(); ++it)
		{
			Order* pNewOrder = new MoveOrder((*it)->GetId(), MoveToPositionOrder, MediumPriority, XMFLOAT2(20.0f, 20.0f));
			
			if(!pNewOrder)
			{
				SetActive(false);
				return;
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
				SetActive(false);
				return;
			}

			FollowOrderMessageData data(pNewOrder);
			SendMessage(*it, FollowOrderMessageType, &data);

			m_activeOrders.insert(std::pair<unsigned long, Order*>((*it)->GetId(), pNewOrder));
			
		}
		
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
BehaviourStatus TestManoeuvre::Update(float deltaTime)
{
	// Keep track of who has reached the target and let those defend until all have arrived
	SortOutProcessedMessages();
	ProcessMessages();

	if(!IsActive())
	{
		// The manoeuvre will become fail if something failed during the initiation or if it wasn't
		// initiated at all.
		return StatusFailure;
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
	TeamManoeuvre::Terminate();
}
