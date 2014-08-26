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

		reinterpret_cast<TeamBehaviour*>(m_pBehaviour)->CalculateCharacteristicValues();

		return true;
	}

	return false;
}

//--------------------------------------------------------------------------------------
// Initialises the available manoeuvres for this team AI.
// Returns true if all manouevres were initialised successfully, false otherwise.
//--------------------------------------------------------------------------------------
bool MultiflagCTFTeamAI::InitialiseManoeuvres(void)
{
	// Use the manoeuvre factory to create the manoeuvres for the team AI
	TeamManoeuvre* pTestManoeuvre = TeamManoeuvreFactory::CreateTeamManoeuvre(TestAllMoveManoeuvre, 8, 8, this, nullptr);
	
	if(!pTestManoeuvre)
	{
		return false;
	}

	m_manoeuvres.insert(std::pair<TeamManoeuvreType, TeamManoeuvre*>(TestAllMoveManoeuvre, pTestManoeuvre));
	
	// Defend base entrances manoeuvre
	DefendBaseEntrancesInitData defendEntrancesData(5.0f);
	TeamManoeuvre* pDefendBaseEntrancesManoeuvre = TeamManoeuvreFactory::CreateTeamManoeuvre(DefendBaseEntrancesManoeuvre, 1, 8, this, &defendEntrancesData);
	if(!pDefendBaseEntrancesManoeuvre)
	{
		return false;
	}
	m_manoeuvres.insert(std::pair<TeamManoeuvreType, TeamManoeuvre*>(DefendBaseEntrancesManoeuvre, pDefendBaseEntrancesManoeuvre));

	// Rush base attack manoeuvre
	RushBaseAttackInitData rushBaseData(10.0f, 15.0f);
	TeamManoeuvre* pRushBaseAttackManoeuvre = TeamManoeuvreFactory::CreateTeamManoeuvre(RushBaseAttackManoeuvre, 4, 8, this, &rushBaseData);
	if(!pRushBaseAttackManoeuvre)
	{
		return false;
	}
	m_manoeuvres.insert(std::pair<TeamManoeuvreType, TeamManoeuvre*>(RushBaseAttackManoeuvre, pRushBaseAttackManoeuvre));


	return true;
}

//--------------------------------------------------------------------------------------
// Prepares the team AI for the simulation.
//--------------------------------------------------------------------------------------
void MultiflagCTFTeamAI::PrepareForSimulation(void)
{
	// Create a list of all base entrances for both teams.
	
}

//--------------------------------------------------------------------------------------
// Processes all inbox messages that the team AI received.
// Param1: A pointer to the message to process.
// Returns true if this was the final communicator to process the message, false if the
// message was forwarded to another one.
//--------------------------------------------------------------------------------------
bool MultiflagCTFTeamAI::ProcessMessage(Message* pMessage)
{
	switch(pMessage->GetType())
	{
	case FlagPickedUpMessageType:
		{
		FlagPickedUpMessage* pMsg = reinterpret_cast<FlagPickedUpMessage*>(pMessage);
		m_flagData[pMsg->GetData().m_flagOwner].m_state     = Stolen;
		m_flagData[pMsg->GetData().m_flagOwner].m_carrierId = pMsg->GetData().m_carrierId;
		
		if(ForwardMessageToActiveManoeuvers(pMsg))
		{
			return false;
		}

		break;
		}
	case FlagDroppedMessageType:
		{
		FlagDroppedMessage* pMsg = reinterpret_cast<FlagDroppedMessage*>(pMessage);
		m_flagData[pMsg->GetData().m_flagOwner].m_state     = Dropped;
		m_flagData[pMsg->GetData().m_flagOwner].m_position  = pMsg->GetData().m_dropPosition;
		m_flagData[pMsg->GetData().m_flagOwner].m_carrierId = 0;
		
		if(ForwardMessageToActiveManoeuvers(pMsg))
		{
			return false;
		}

		break;
		}
	case FlagReturnedMessageType:
		{
		FlagReturnedMessage* pMsg = reinterpret_cast<FlagReturnedMessage*>(pMessage);
		m_flagData[pMsg->GetData().m_flagOwner].m_state     = InBase;
		m_flagData[pMsg->GetData().m_flagOwner].m_position  = m_flagData[pMsg->GetData().m_flagOwner].m_basePosition;
		m_flagData[pMsg->GetData().m_flagOwner].m_carrierId = 0;
		
		if(ForwardMessageToActiveManoeuvers(pMsg))
		{
			return false;
		}

		break;
		}
	default:
		// Forward other messages to the base class implementation of the function
		return TeamAI::ProcessMessage(pMessage);
	}

	return true;
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
// Checks whether the preconditions for a certain manoeuvre are fulfilled.
// Param1: Identifies the manoeuvre, for which to check the preconditions.
//--------------------------------------------------------------------------------------
bool MultiflagCTFTeamAI::ManoeuvrePreconditionsFulfilled(TeamManoeuvreType manoeuvre)
{
	unsigned int numberOfAvailableEntities = 0;
	std::vector<Entity*> availableEntities;

	// Check how many entities are freely available to engage in new manoeuvres
	for(std::vector<Entity*>::iterator it = GetTeamMembers().begin(); it != GetTeamMembers().end(); ++it)
	{
		// If the entity is not registered with a manoeuvre, remember it as available
		if(!m_entityManoeuvreMap[(*it)->GetId()])
		{
			++numberOfAvailableEntities;
			availableEntities.push_back(*it);
		}
	}

	EntityTeam enemyTeam = None;
	if(GetTeam() == TeamRed)
	{
		enemyTeam = TeamBlue;
	}else
	{
		enemyTeam = TeamRed;
	}


	switch(manoeuvre)
	{
	case TestAllMoveManoeuvre:
		
		return (numberOfAvailableEntities >= m_manoeuvres[TestAllMoveManoeuvre]->GetMinNumberOfParticipants());
		break;
	case DefendBaseEntrancesManoeuvre:
		return (numberOfAvailableEntities >= m_manoeuvres[DefendBaseEntrancesManoeuvre]->GetMinNumberOfParticipants()) &&
			   (m_flagData[GetTeam()].m_state == InBase);
		break;
	case RushBaseAttackManoeuvre:
		return (numberOfAvailableEntities >= m_manoeuvres[RushBaseAttackManoeuvre]->GetMinNumberOfParticipants()) &&
			   (m_flagData[enemyTeam].m_state == InBase);
		break;
	default:
		return TeamAI::ManoeuvrePreconditionsFulfilled(manoeuvre);
	}
}

//--------------------------------------------------------------------------------------
// Checks whether the necessary conditions for the execution of a certain manoeuvre are 
// still given
// Param1: Identifies the manoeuvre, for which to check the conditions.
// Returns true if the conditions for the manoeuvre are still fulfilled, false otherwise.
//--------------------------------------------------------------------------------------
bool MultiflagCTFTeamAI::ManoeuvreStillValid(TeamManoeuvreType manoeuvre)
{
	switch(manoeuvre)
	{
	case TestAllMoveManoeuvre:
		return true;
		break;
	default:
		return TeamAI::ManoeuvreStillValid(manoeuvre);
	}
}


//--------------------------------------------------------------------------------------
// Processes a message sent to the team AI.
// Param1: A pointer to the message to process.
//--------------------------------------------------------------------------------------
void MultiflagCTFTeamAI::InitiateManoeuvre(TeamManoeuvreType manoeuvre)
{
	// Check if this AI has a manoeuvre of that type associated to it
	std::unordered_map<TeamManoeuvreType, TeamManoeuvre*>::iterator foundIt = m_manoeuvres.find(manoeuvre);
	if(foundIt != m_manoeuvres.end())
	{
		// Keep track of how many entities have been added to the manoeuvre.
		unsigned int addedEntities = 0;

		std::vector<Entity*>::iterator it = GetTeamMembers().begin();

		// Add available entities to the manoeuvre until the maximally allowed number is reached.
		while((addedEntities <= foundIt->second->GetMaxNumberOfParticipants()) && (it != GetTeamMembers().end()))
		{
			// If the entity is not engaged in another manoeuver, add it to this one.
			if(!m_entityManoeuvreMap[(*it)->GetId()])
			{
				foundIt->second->AddParticipant(*it);
				// Remember that this entity is now executing that manoeuver
				m_entityManoeuvreMap[(*it)->GetId()] = foundIt->second;
				++addedEntities;
			}
			++it;
		}

		// Initiate the manoeuvre
		foundIt->second->Initiate();
	}else
	{
		// Forward the call to the base class
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
	// Check if this AI has a manoeuvre of that type associated to it
	std::unordered_map<TeamManoeuvreType, TeamManoeuvre*>::iterator foundIt = m_manoeuvres.find(manoeuvre);
	if(foundIt != m_manoeuvres.end())
	{
		// Update the manoeuvre
		return foundIt->second->Update(deltaTime);
	}else
	{
		// Forward the call to the base class
		return TeamAI::UpdateManoeuvre(manoeuvre, deltaTime);
	}
}

//--------------------------------------------------------------------------------------
// Processes a message sent to the team AI.
// Param1: A pointer to the message to process.
//--------------------------------------------------------------------------------------
void MultiflagCTFTeamAI::TerminateManoeuvre(TeamManoeuvreType manoeuvre)
{
	// Check if this AI has a manoeuvre of that type associated to it
	std::unordered_map<TeamManoeuvreType, TeamManoeuvre*>::iterator foundIt = m_manoeuvres.find(manoeuvre);
	if(foundIt != m_manoeuvres.end())
	{
		// Release all entities from the manoeuvre that is about to terminate
		for(std::vector<Entity*>::const_iterator it = foundIt->second->GetParticipants().begin(); it != foundIt->second->GetParticipants().end(); ++it)
		{
			m_entityManoeuvreMap[(*it)->GetId()] = nullptr;
		}

		// Terminate the manoeuvre
		foundIt->second->Terminate();
	}else
	{
		// Forward the call to the base class
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

const FlagData& MultiflagCTFTeamAI::GetFlagData(EntityTeam team) const
{
	return m_flagData[team];
}

//--------------------------------------------------------------------------------------
// Registers an objective, in this case a flag, with the team AI.
// Param1: A pointer to the objective to register.
//--------------------------------------------------------------------------------------
void MultiflagCTFTeamAI::RegisterObjective(Objective* pObjective)
{
	if(pObjective)
	{
		m_flagData[pObjective->GetTeam()].m_basePosition = pObjective->GetPosition();
		m_flagData[pObjective->GetTeam()].m_position     = pObjective->GetPosition();
	}
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