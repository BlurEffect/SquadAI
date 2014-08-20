/* 
*  Kevin Meergans, SquadAI, 2014
*  MultiflagCTFTeamAI.cpp
*  Team AI that controls its entities to play Multiflag Capture-the-Flag matches.
*/

#include "MultiflagCTFTeamAI.h"
#include "TestEnvironment.h"

MultiflagCTFTeamAI::MultiflagCTFTeamAI(void) : TeamAI()
{
}

MultiflagCTFTeamAI::~MultiflagCTFTeamAI(void)
{
}

//--------------------------------------------------------------------------------------
// Initialises the multiflag CTF AI.
// Param1: The team that this Ai will control.
// Param2: A pointer to the test environment, in which the matches take place.
//--------------------------------------------------------------------------------------
bool MultiflagCTFTeamAI::Initialise(EntityTeam team, TestEnvironment* pEnvironment, TeamAICharacteristic characteristic)
{
	if(TeamAI::Initialise(team, pEnvironment, characteristic))
	{
		m_pBehaviour = BehaviourFactory::CreateBehaviourTree(SimpleTeamMultiflagCTFTree, this);
		if(!m_pBehaviour)
		{
			return false;
		}

		return true;
	}

	return false;
}

//--------------------------------------------------------------------------------------
// Processes all inbox messages that the team AI received.
// Param1: A pointer to the message to process.
//--------------------------------------------------------------------------------------
void MultiflagCTFTeamAI::ProcessMessage(Message* pMessage)
{
	switch(pMessage->GetType())
	{
	case FlagPickedUpMessageType:
		{
		FlagPickedUpMessage* pMsg = reinterpret_cast<FlagPickedUpMessage*>(pMessage);
		m_flagData[pMsg->GetData().m_flagOwner].m_state     = Stolen;
		m_flagData[pMsg->GetData().m_flagOwner].m_carrierId = pMsg->GetData().m_carrierId;
		break;
		}
	case FlagDroppedMessageType:
		{
		FlagDroppedMessage* pMsg = reinterpret_cast<FlagDroppedMessage*>(pMessage);
		m_flagData[pMsg->GetData().m_flagOwner].m_state     = Dropped;
		m_flagData[pMsg->GetData().m_flagOwner].m_position  = pMsg->GetData().m_dropPosition;
		m_flagData[pMsg->GetData().m_flagOwner].m_carrierId = 0;
		break;
		}
	case FlagReturnedMessageType:
		{
		FlagReturnedMessage* pMsg = reinterpret_cast<FlagReturnedMessage*>(pMessage);
		m_flagData[pMsg->GetData().m_flagOwner].m_state     = InBase;
		m_flagData[pMsg->GetData().m_flagOwner].m_position  = m_flagData[pMsg->GetData().m_flagOwner].m_basePosition;
		m_flagData[pMsg->GetData().m_flagOwner].m_carrierId = 0;
		break;
		}
	default:
		// Forward other messages to the base class implementation of the function
		TeamAI::ProcessMessage(pMessage);
	}
}

//--------------------------------------------------------------------------------------
// Process a given event. Default implementation.
// Param1: The type of event.
// Param2: A pointer to the event data.
//--------------------------------------------------------------------------------------
void MultiflagCTFTeamAI::ProcessEvent(EventType type, void* pEventData)
{
	// Not expecting any events, forward to the base class
	TeamAI::ProcessEvent(type, pEventData);
}

//--------------------------------------------------------------------------------------
// Processes a message sent to the team AI.
// Param1: A pointer to the message to process.
//--------------------------------------------------------------------------------------
void MultiflagCTFTeamAI::InitiateManoeuvre(TeamManoeuvreType manoeuvre)
{
	switch(manoeuvre)
	{
	case TestAllAttackManoeuvre:
		break;
	case TestAllMoveManoeuvre:
		break;
	case TestAllDefendManoeuvre:
		break;
	default:
		TeamAI::InitiateManoeuvre(manoeuvre);
	}
}

//--------------------------------------------------------------------------------------
// Processes a message sent to the team AI.
// Param1: A pointer to the message to process.
// Param2: The time passed since the last update.
// Returns a behaviour status code representing the current state of the manoeuvre.
//--------------------------------------------------------------------------------------
BehaviourStatus MultiflagCTFTeamAI::UpdateManoeuvre(TeamManoeuvreType manoeuvre, float deltaTime)
{
	switch(manoeuvre)
	{
	case TestAllAttackManoeuvre:
		return AllAttack(deltaTime);
		break;
	case TestAllMoveManoeuvre:
		return AllMove(deltaTime);
		break;
	case TestAllDefendManoeuvre:
		return AllDefend(deltaTime);
		break;
	default:
		return TeamAI::UpdateManoeuvre(manoeuvre, deltaTime);
	}
}

//--------------------------------------------------------------------------------------
// Processes a message sent to the team AI.
// Param1: A pointer to the message to process.
//--------------------------------------------------------------------------------------
void MultiflagCTFTeamAI::TerminateManoeuvre(TeamManoeuvreType manoeuvre)
{
	switch(manoeuvre)
	{
	case TestAllAttackManoeuvre:
		break;
	case TestAllMoveManoeuvre:
		break;
	case TestAllDefendManoeuvre:
		break;
	default:
		TeamAI::TerminateManoeuvre(manoeuvre);
	}
}

//--------------------------------------------------------------------------------------
// Sends all team members to attack a certain enemy.
// Param1: The time in seconds passed since the last frame.
// Returns the current state of the action.
//--------------------------------------------------------------------------------------
BehaviourStatus MultiflagCTFTeamAI::AllAttack(float deltaTime)
{
	if(!GetEnemyRecords().empty())
	{
		for(std::vector<Entity*>::iterator it = GetTeamMembers().begin(); it != GetTeamMembers().end(); ++it)
		{
			// Check if there currently is another order for the entity
			std::unordered_map<unsigned long, Order*>::iterator foundIt = GetActiveOrders().find((*it)->GetId());
			if(foundIt != GetActiveOrders().end())
			{
				break;
			}
			
			Order* pNewOrder = new AttackOrder((*it)->GetId(), AttackEnemyOrder, HighPriority, GetEnemyRecords().begin()->first, GetEnemyRecords().begin()->second.m_lastKnownPosition);
			
			if(!pNewOrder)
			{
				return StatusFailure;
			}

			FollowOrderMessageData data(pNewOrder);
			SendMessage(*it, FollowOrderMessageType, &data);

			GetActiveOrders().insert(std::pair<unsigned long, Order*>((*it)->GetId(), pNewOrder));
		}
	}

	return StatusSuccess;
}

//--------------------------------------------------------------------------------------
// Sends all team members to defend a certain position.
// Param1: The time in seconds passed since the last frame.
// Returns the current state of the action.
//--------------------------------------------------------------------------------------
BehaviourStatus MultiflagCTFTeamAI::AllDefend(float deltaTime)
{
	if(GetTeam() == TeamRed)
	{
		for(std::vector<Entity*>::iterator it = GetTeamMembers().begin(); it != GetTeamMembers().end(); ++it)
		{
			// Check if there currently is another order for the entity
			std::unordered_map<unsigned long, Order*>::iterator foundIt = GetActiveOrders().find((*it)->GetId());
			if(foundIt != GetActiveOrders().end())
			{
				break;
			}

			Order* pNewOrder = new DefendOrder((*it)->GetId(), DefendPositionOrder, MediumPriority, XMFLOAT2(0.0f, 0.0f), XMFLOAT2(-1.0f, 0.0f));
			
			if(!pNewOrder)
			{
				return StatusFailure;
			}

			FollowOrderMessageData data(pNewOrder);
			SendMessage(*it, FollowOrderMessageType, &data);

			GetActiveOrders().insert(std::pair<unsigned long, Order*>((*it)->GetId(), pNewOrder));
		}
	}else
	{
		for(std::vector<Entity*>::iterator it = GetTeamMembers().begin(); it != GetTeamMembers().end(); ++it)
		{
			// Check if there currently is another order for the entity
			std::unordered_map<unsigned long, Order*>::iterator foundIt = GetActiveOrders().find((*it)->GetId());
			if(foundIt != GetActiveOrders().end())
			{
				break;
			}
			/*
			Order* pNewOrder = new DefendOrder((*it)->GetId(), DefendPositionOrder, MediumPriority, XMFLOAT2(-20.0f, -20.0f));
			
			if(!pNewOrder)
			{
				return StatusFailure;
			}

			FollowOrderMessageData data(pNewOrder);
			SendMessage(*it, FollowOrderMessageType, &data);

			GetActiveOrders().insert(std::pair<unsigned long, Order*>((*it)->GetId(), pNewOrder));
			*/
		}
	}

	return StatusSuccess;
}

bool blueDone = false;
bool redDone = false;

//--------------------------------------------------------------------------------------
// Sends all team members to move to a certain position.
// Param1: The time in seconds passed since the last frame.
// Returns the current state of the action.
//--------------------------------------------------------------------------------------
BehaviourStatus MultiflagCTFTeamAI::AllMove(float deltaTime)
{
	if(GetTeam() == TeamRed)
	{
		if(redDone)
			return StatusSuccess;
		for(std::vector<Entity*>::iterator it = GetTeamMembers().begin(); it != GetTeamMembers().end(); ++it)
		{
			// Check if there currently is another order for the entity
			std::unordered_map<unsigned long, Order*>::iterator foundIt = GetActiveOrders().find((*it)->GetId());
			if(foundIt != GetActiveOrders().end())
			{
				break;
			}

			Order* pNewOrder = new MoveOrder((*it)->GetId(), MoveToPositionOrder, MediumPriority, XMFLOAT2(20.0f, 20.0f));
			
			if(!pNewOrder)
			{
				return StatusFailure;
			}

			FollowOrderMessageData data(pNewOrder);
			SendMessage(*it, FollowOrderMessageType, &data);

			GetActiveOrders().insert(std::pair<unsigned long, Order*>((*it)->GetId(), pNewOrder));
		}
		redDone = true;
	}else
	{
		if(blueDone)
			return StatusSuccess;
		for(std::vector<Entity*>::iterator it = GetTeamMembers().begin(); it != GetTeamMembers().end(); ++it)
		{
			// Check if there currently is another order for the entity
			std::unordered_map<unsigned long, Order*>::iterator foundIt = GetActiveOrders().find((*it)->GetId());
			if(foundIt != GetActiveOrders().end())
			{
				break;
			}
			
			Order* pNewOrder = new MoveOrder((*it)->GetId(), MoveToPositionOrder, HighPriority, XMFLOAT2(-20.0f, -20.0f));
			
			if(!pNewOrder)
			{
				return StatusFailure;
			}

			FollowOrderMessageData data(pNewOrder);
			SendMessage(*it, FollowOrderMessageType, &data);

			GetActiveOrders().insert(std::pair<unsigned long, Order*>((*it)->GetId(), pNewOrder));
			
		}
		blueDone = true;
	}
	
	return StatusSuccess;
}


/*
//--------------------------------------------------------------------------------------
// Processes a message sent to the team AI.
// Param1: A pointer to the message to process.
//--------------------------------------------------------------------------------------
void MultiflagCTFTeamAI::ProcessMessage(Message* pMessage)
{
	switch(pMessage->GetType())
	{
	case FlagPickedUpMessageType:
		{
		FlagPickedUpMessage* pMsg = reinterpret_cast<FlagPickedUpMessage*>(pMessage);
		m_flagData[pMsg->GetFlagOwner()].m_state     = Stolen;
		m_flagData[pMsg->GetFlagOwner()].m_carrierId = pMsg->GetCarrierId();
		break;
		}
	case FlagDroppedMessageType:
		{
		FlagDroppedMessage* pMsg = reinterpret_cast<FlagDroppedMessage*>(pMessage);
		m_flagData[pMsg->GetFlagOwner()].m_state     = Dropped;
		m_flagData[pMsg->GetFlagOwner()].m_position  = pMsg->GetDropPosition();
		m_flagData[pMsg->GetFlagOwner()].m_carrierId = 0;
		break;
		}
	case FlagReturnedMessageType:
		{
		FlagReturnedMessage* pMsg = reinterpret_cast<FlagReturnedMessage*>(pMessage);
		m_flagData[pMsg->GetFlagOwner()].m_state     = InBase;
		m_flagData[pMsg->GetFlagOwner()].m_position  = m_flagData[pMsg->GetFlagOwner()].m_basePosition;
		m_flagData[pMsg->GetFlagOwner()].m_carrierId = 0;
		break;
		}
	default:
		// Forward other messages to the base class implementation of the function
		TeamAI::ProcessMessage(pMessage);
	}
}
*/




//--------------------------------------------------------------------------------------
// Resets the team AI after a completed match.
//--------------------------------------------------------------------------------------
void MultiflagCTFTeamAI::Reset(void)
{
	for(unsigned int i = 0; i < NumberOfTeams-1; ++i)
	{
		m_flagData[i].m_state     = InBase;
		m_flagData[i].m_position  = m_flagData[i].m_basePosition;
		m_flagData[i].m_carrierId = 0;
	}

	ClearOrders();

	TeamAI::Reset();
}