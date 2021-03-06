/* 
*  Kevin Meergans, SquadAI, 2014
*  MultiflagCTFTeamAI.cpp
*  Team AI that controls its entities to play Multiflag Capture-the-Flag matches.
*/

#include "MultiflagCTFTeamAI.h"
#include "TestEnvironment.h"
#include "Logger.h"

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

		if(!m_pBehaviour)
		{
			m_pBehaviour = BehaviourFactory::CreateBehaviourTree(SimpleTeamMultiflagCTFTree, this);
			if(!m_pBehaviour)
			{
				return false;
			}

			reinterpret_cast<TeamBehaviour*>(m_pBehaviour)->CalculateCharacteristicValues();
		}

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
	
	// Defend base entrances manoeuvre
	DefendBaseEntrancesInitData defendEntrancesData(5.0f);
	TeamManoeuvre* pDefendBaseEntrancesManoeuvre = TeamManoeuvreFactory::CreateTeamManoeuvre(DefendBaseEntrancesManoeuvre, 3, 6, this, &defendEntrancesData);
	if(!pDefendBaseEntrancesManoeuvre)
	{
		return false;
	}
	m_manoeuvres.insert(std::pair<TeamManoeuvreType, TeamManoeuvre*>(DefendBaseEntrancesManoeuvre, pDefendBaseEntrancesManoeuvre));

	// Rush base attack manoeuvre
	RushBaseAttackInitData rushBaseData(5.0f);
	TeamManoeuvre* pRushBaseAttackManoeuvre = TeamManoeuvreFactory::CreateTeamManoeuvre(RushBaseAttackManoeuvre, 4, 8, this, &rushBaseData);
	if(!pRushBaseAttackManoeuvre)
	{
		return false;
	}
	m_manoeuvres.insert(std::pair<TeamManoeuvreType, TeamManoeuvre*>(RushBaseAttackManoeuvre, pRushBaseAttackManoeuvre));

	// Run flag home manoeuvre
	TeamManoeuvre* pRunFlagHomeManoeuvre = TeamManoeuvreFactory::CreateTeamManoeuvre(RunTheFlagHomeManoeuvre, 1, 1, this, nullptr);
	if(!pRunFlagHomeManoeuvre)
	{
		return false;
	}
	m_manoeuvres.insert(std::pair<TeamManoeuvreType, TeamManoeuvre*>(RunTheFlagHomeManoeuvre, pRunFlagHomeManoeuvre));

	// Return dropped flag manoeuvre
	TeamManoeuvre* pReturnDroppedFlag = TeamManoeuvreFactory::CreateTeamManoeuvre(ReturnDroppedFlagManoeuvre, 1, 4, this, nullptr);
	if(!pReturnDroppedFlag)
	{
		return false;
	}
	m_manoeuvres.insert(std::pair<TeamManoeuvreType, TeamManoeuvre*>(ReturnDroppedFlagManoeuvre, pReturnDroppedFlag));

	// Coordinated base attack manoeuvre
	CoordinatedBaseAttackInitData coordinatedBaseAttackData(2, 15.0f);
	TeamManoeuvre* pCoordinatedBaseAttack = TeamManoeuvreFactory::CreateTeamManoeuvre(CoordinatedBaseAttackManoeuvre, 4, 6, this, &coordinatedBaseAttackData);
	if(!pCoordinatedBaseAttack)
	{
		return false;
	}
	m_manoeuvres.insert(std::pair<TeamManoeuvreType, TeamManoeuvre*>(CoordinatedBaseAttackManoeuvre, pCoordinatedBaseAttack));

	// Distraction base attack manoeuvre
	DistractionBaseAttackInitData distractionBaseAttackData(1, 10.0f);
	TeamManoeuvre* pDistractionBaseAttack = TeamManoeuvreFactory::CreateTeamManoeuvre(DistractionBaseAttackManoeuvre, 3, 5, this, &distractionBaseAttackData);
	if(!pDistractionBaseAttack)
	{
		return false;
	}
	m_manoeuvres.insert(std::pair<TeamManoeuvreType, TeamManoeuvre*>(DistractionBaseAttackManoeuvre, pDistractionBaseAttack));

	// Simple base attack manoeuvre
	TeamManoeuvre* pSimpleBaseAttack = TeamManoeuvreFactory::CreateTeamManoeuvre(SimpleBaseAttackManoeuvre, 1, 3, this, nullptr);
	if(!pSimpleBaseAttack)
	{
		return false;
	}
	m_manoeuvres.insert(std::pair<TeamManoeuvreType, TeamManoeuvre*>(SimpleBaseAttackManoeuvre, pSimpleBaseAttack));

	// Pick up dropped flag manoeuvre
	TeamManoeuvre* pPickUpDroppedFlag = TeamManoeuvreFactory::CreateTeamManoeuvre(PickUpDroppedFlagManoeuvre, 1, 4, this, nullptr);
	if(!pPickUpDroppedFlag)
	{
		return false;
	}
	m_manoeuvres.insert(std::pair<TeamManoeuvreType, TeamManoeuvre*>(PickUpDroppedFlagManoeuvre, pPickUpDroppedFlag));

	// Simple base defence manoeuvre
	TeamManoeuvre* pSimpleBaseDefence = TeamManoeuvreFactory::CreateTeamManoeuvre(SimpleBaseDefenceManoeuvre, 1, 3, this, nullptr);
	if(!pSimpleBaseDefence)
	{
		return false;
	}
	m_manoeuvres.insert(std::pair<TeamManoeuvreType, TeamManoeuvre*>(SimpleBaseDefenceManoeuvre, pSimpleBaseDefence));

	// Active base defence manoeuvre
	ActiveBaseDefenceInitData activeBaseDefenceData(20.0f);
	TeamManoeuvre* pActiveBaseDefenceAttack = TeamManoeuvreFactory::CreateTeamManoeuvre(ActiveBaseDefenceManoeuvre, 2, 4, this, &activeBaseDefenceData);
	if(!pActiveBaseDefenceAttack)
	{
		return false;
	}
	m_manoeuvres.insert(std::pair<TeamManoeuvreType, TeamManoeuvre*>(ActiveBaseDefenceManoeuvre, pActiveBaseDefenceAttack));

	// Guarded flag capture manoeuvre
	GuardedFlagCaptureInitData guardedFlagCaptureData(6.0f, 0.1f);
	TeamManoeuvre* pGuardedFlagCapture = TeamManoeuvreFactory::CreateTeamManoeuvre(GuardedFlagCaptureManoeuvre, 3, 5, this, &guardedFlagCaptureData);
	if(!pGuardedFlagCapture)
	{
		return false;
	}
	m_manoeuvres.insert(std::pair<TeamManoeuvreType, TeamManoeuvre*>(GuardedFlagCaptureManoeuvre, pGuardedFlagCapture));

	// Intercept flag carrier manoeuvre
	InterceptFlagCarrierInitData interceptFlagCarrierData(5.0f, 0.2f);
	TeamManoeuvre* pInterceptFlagCarrier = TeamManoeuvreFactory::CreateTeamManoeuvre(InterceptFlagCarrierManoeuvre, 1, 4, this, &interceptFlagCarrierData);
	if(!pInterceptFlagCarrier)
	{
		return false;
	}
	m_manoeuvres.insert(std::pair<TeamManoeuvreType, TeamManoeuvre*>(InterceptFlagCarrierManoeuvre, pInterceptFlagCarrier));

	return true;
}

//--------------------------------------------------------------------------------------
// Updates the team AI.
// Param1: The time in seconds passed since the last frame.
//--------------------------------------------------------------------------------------
void MultiflagCTFTeamAI::Update(float deltaTime)
{
	TeamAI::Update(deltaTime);

	EntityTeam enemyTeam = (GetTeam() == TeamRed) ? TeamBlue : TeamRed;
	
	if(m_flagData[enemyTeam].m_carrierId != 0)
	{
		// Get the position of the flag carrier and set it as the flag's current position
		m_flagData[enemyTeam].m_position = (*(std::find_if(GetTeamMembers().begin(), GetTeamMembers().end(), Entity::FindEntityById(m_flagData[enemyTeam].m_carrierId))))->GetPosition();
	}
}

//--------------------------------------------------------------------------------------
// Prepares the team AI for the simulation.
//--------------------------------------------------------------------------------------
void MultiflagCTFTeamAI::PrepareForSimulation(void)
{
	
}

//--------------------------------------------------------------------------------------
// Processes all inbox messages that the team AI received.
// Param1: A pointer to the message to process.
//--------------------------------------------------------------------------------------
void MultiflagCTFTeamAI::ProcessMessage(Message* pMessage)
{
	switch(pMessage->GetType())
	{
	case EnemySpottedMessageType:
		{
		EnemySpottedMessage* pMsg = reinterpret_cast<EnemySpottedMessage*>(pMessage);
		// Try to add the new enemy record.
		std::pair<std::unordered_map<unsigned long, EnemyRecord>::iterator, bool> result = GetEnemyRecords().insert(std::pair<unsigned long, EnemyRecord>(pMsg->GetData().m_enemyId, EnemyRecord(pMsg->GetData().m_enemyPosition)));
		if(!result.second)
		{
			// There already is a record for the spotted enemy -> Add the enemy as spotter and update the enemy position
			result.first->second.m_lastKnownPosition = pMsg->GetData().m_enemyPosition;
		}

		// Add the enemy to the list of spotted enemies for the team member that spotted the enemy
		// The set will prevent enemy IDs to be included more than once in the list.
		m_spottedEnemies[pMsg->GetData().m_spotterId].insert(pMsg->GetData().m_enemyId);

		// If the spotted enemy is the enemy flag carrier, update the flag position
		if(m_flagData[GetTeam()].m_carrierId == pMsg->GetData().m_enemyId)
		{
			m_flagData[GetTeam()].m_position = pMsg->GetData().m_enemyPosition;
		}
		ForwardMessageToActiveManoeuvers(pMessage);
		break;
		}
	case UpdateEnemyPositionMessageType:
		{
		UpdateEnemyPositionMessage* pMsg = reinterpret_cast<UpdateEnemyPositionMessage*>(pMessage);
		std::unordered_map<unsigned long, EnemyRecord>::iterator foundIt = GetEnemyRecords().find(pMsg->GetData().m_enemyId);
		if(foundIt != GetEnemyRecords().end())
		{
			foundIt->second.m_lastKnownPosition = pMsg->GetData().m_enemyPosition;
			//UpdateAttackOrders(pMsg->GetData().m_enemyId);
			ForwardMessageToActiveManoeuvers(pMessage);

			// If the enemy is the enemy flag carrier, update the flag position
			if(m_flagData[GetTeam()].m_carrierId == pMsg->GetData().m_enemyId)
			{
				m_flagData[GetTeam()].m_position = pMsg->GetData().m_enemyPosition;
			}
		}
		break;
		}
	case FlagPickedUpMessageType:
		{
		FlagPickedUpMessage* pMsg = reinterpret_cast<FlagPickedUpMessage*>(pMessage);
		m_flagData[pMsg->GetData().m_flagOwner].m_state     = Stolen;
		m_flagData[pMsg->GetData().m_flagOwner].m_carrierId = pMsg->GetData().m_carrierId;
		
		ForwardMessageToActiveManoeuvers(pMsg);

		break;
		}
	case FlagDroppedMessageType:
		{
		FlagDroppedMessage* pMsg = reinterpret_cast<FlagDroppedMessage*>(pMessage);
		m_flagData[pMsg->GetData().m_flagOwner].m_state     = Dropped;
		m_flagData[pMsg->GetData().m_flagOwner].m_position  = pMsg->GetData().m_dropPosition;
		m_flagData[pMsg->GetData().m_flagOwner].m_carrierId = 0;
		
		ForwardMessageToActiveManoeuvers(pMsg);

		break;
		}
	case FlagReturnedMessageType:
		{
		FlagReturnedMessage* pMsg = reinterpret_cast<FlagReturnedMessage*>(pMessage);
		m_flagData[pMsg->GetData().m_flagOwner].m_state     = InBase;
		m_flagData[pMsg->GetData().m_flagOwner].m_position  = m_flagData[pMsg->GetData().m_flagOwner].m_basePosition;
		m_flagData[pMsg->GetData().m_flagOwner].m_carrierId = 0;
		
		ForwardMessageToActiveManoeuvers(pMsg);

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
// Checks whether the preconditions for a certain manoeuvre are fulfilled.
// Param1: Identifies the manoeuvre, for which to check the preconditions.
//--------------------------------------------------------------------------------------
bool MultiflagCTFTeamAI::ManoeuvrePreconditionsFulfilled(TeamManoeuvreType manoeuvre)
{
	EntityTeam team = GetTeam();
	GetTestEnvironment()->RecordEvent(TeamManoeuvrePreconditionCheckLogEvent, &team, &manoeuvre);

	unsigned int numberOfAvailableEntities = 0;
	std::vector<Entity*> availableEntities;

	// Check how many entities are freely available to engage in new manoeuvres
	for(std::vector<Entity*>::iterator it = GetTeamMembers().begin(); it != GetTeamMembers().end(); ++it)
	{
		// If the entity is alive and not yet registered with a manoeuvre, remember it as available
		if((*it)->IsAlive() && !m_entityManoeuvreMap[(*it)->GetId()])
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
	case DefendBaseEntrancesManoeuvre:
		// If one of the entities is the flag carrier, it has to be ensured not to involve him in the defend manoeuvre
		if(m_flagData[enemyTeam].m_state == Stolen)
		{
			std::vector<Entity*>::iterator foundIt = std::find_if(availableEntities.begin(), availableEntities.end(), Entity::FindEntityById(m_flagData[enemyTeam].m_carrierId));
			if(foundIt != availableEntities.end())
			{
				// Deduce the flag carrier from the available entities
				--numberOfAvailableEntities;
			}
		}

		// Don't count flag carriers!
		return (numberOfAvailableEntities >= m_manoeuvres[manoeuvre]->GetMinNumberOfParticipants()) &&
			   (m_flagData[GetTeam()].m_state == InBase);
		break;
	case RushBaseAttackManoeuvre:
		return (numberOfAvailableEntities >= m_manoeuvres[manoeuvre]->GetMinNumberOfParticipants()) &&
			   (m_flagData[enemyTeam].m_state == InBase);
		break;
	case CoordinatedBaseAttackManoeuvre:
		return (numberOfAvailableEntities >= m_manoeuvres[manoeuvre]->GetMinNumberOfParticipants()) &&
				(m_flagData[enemyTeam].m_state == InBase);
		break;
	case DistractionBaseAttackManoeuvre:
		return (numberOfAvailableEntities >= m_manoeuvres[manoeuvre]->GetMinNumberOfParticipants()) &&
				(m_flagData[enemyTeam].m_state == InBase);
		break;
	case SimpleBaseAttackManoeuvre:
		return (numberOfAvailableEntities >= m_manoeuvres[manoeuvre]->GetMinNumberOfParticipants()) &&
				(m_flagData[enemyTeam].m_state == InBase);
		break;
	case PickUpDroppedFlagManoeuvre:
		return (numberOfAvailableEntities >= m_manoeuvres[manoeuvre]->GetMinNumberOfParticipants()) &&
				(m_flagData[enemyTeam].m_state == Dropped);
		break;
	case RunTheFlagHomeManoeuvre:
		return (m_flagData[enemyTeam].m_state == Stolen);
		break;
	case ReturnDroppedFlagManoeuvre:
		return (numberOfAvailableEntities >= m_manoeuvres[manoeuvre]->GetMinNumberOfParticipants()) &&
			   (m_flagData[GetTeam()].m_state == Dropped);
		break;
	case SimpleBaseDefenceManoeuvre:
		return (numberOfAvailableEntities >= m_manoeuvres[manoeuvre]->GetMinNumberOfParticipants()) &&
			   (m_flagData[GetTeam()].m_state == InBase);
		break;
	case ActiveBaseDefenceManoeuvre:
		return (numberOfAvailableEntities >= m_manoeuvres[manoeuvre]->GetMinNumberOfParticipants()) &&
			   (m_flagData[GetTeam()].m_state == InBase);
		break;
	case GuardedFlagCaptureManoeuvre:
		return (numberOfAvailableEntities >= m_manoeuvres[manoeuvre]->GetMinNumberOfParticipants()) &&
			   (m_flagData[enemyTeam].m_state == Stolen);
		break;
	case InterceptFlagCarrierManoeuvre:
		return (numberOfAvailableEntities >= m_manoeuvres[manoeuvre]->GetMinNumberOfParticipants()) &&
			(m_flagData[GetTeam()].m_state == Stolen);
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
	case DefendBaseEntrancesManoeuvre:
		return (m_flagData[GetTeam()].m_state == InBase);
		break;
	case RushBaseAttackManoeuvre:
		return (m_flagData[enemyTeam].m_state == InBase);
		break;
	case CoordinatedBaseAttackManoeuvre:
		return (m_flagData[enemyTeam].m_state == InBase);
		break;
	case DistractionBaseAttackManoeuvre:
		return (m_flagData[enemyTeam].m_state == InBase);
		break;
	case SimpleBaseAttackManoeuvre:
		return (m_flagData[enemyTeam].m_state == InBase);
		break;
	case PickUpDroppedFlagManoeuvre:
		return (m_flagData[enemyTeam].m_state == Dropped);
		break;
	case RunTheFlagHomeManoeuvre:
		return (m_flagData[enemyTeam].m_state == Stolen);
		break;
	case ReturnDroppedFlagManoeuvre:
		return (m_flagData[GetTeam()].m_state == Dropped);
		break;
	case SimpleBaseDefenceManoeuvre:
		return (m_flagData[GetTeam()].m_state == InBase);
		break;
	case ActiveBaseDefenceManoeuvre:
		return (m_flagData[GetTeam()].m_state == InBase);
		break;
	case GuardedFlagCaptureManoeuvre:
		return (m_flagData[enemyTeam].m_state == Stolen);
		break;
	case InterceptFlagCarrierManoeuvre:
		return (m_flagData[GetTeam()].m_state == Stolen);
		break;
	default:
		return TeamAI::ManoeuvreStillValid(manoeuvre);
	}
}


//--------------------------------------------------------------------------------------
// Initiates a manoeuvre.
// Param1: Identifies the manoeuvre to initiate.
// Returns a behaviour status code representing the current state of the initiation of 
// the manoeuvre.
//--------------------------------------------------------------------------------------
BehaviourStatus MultiflagCTFTeamAI::InitiateManoeuvre(TeamManoeuvreType manoeuvre)
{
	std::unordered_map<TeamManoeuvreType, TeamManoeuvre*>::iterator foundIt = m_manoeuvres.find(manoeuvre);
	if(foundIt == m_manoeuvres.end())
	{
		return TeamAI::InitiateManoeuvre(manoeuvre);
	}

	// Abort running behaviours of the same category
	if(m_activeManoeuvres[foundIt->second->GetCategory()] != nullptr)
	{
		TerminateManoeuvre(m_activeManoeuvres[foundIt->second->GetCategory()]->GetType());
	}

	EntityTeam team = GetTeam();
	GetTestEnvironment()->RecordEvent(TeamManoeuvreInitLogEvent, &team, &manoeuvre);

	EntityTeam enemyTeam = (GetTeam() == TeamRed) ? TeamBlue : TeamRed;

	if(manoeuvre == RunTheFlagHomeManoeuvre || manoeuvre == GuardedFlagCaptureManoeuvre)
	{
		// Add the new flag carrier as participant to the manoeuvre

		std::vector<Entity*>::iterator foundIt = std::find_if(GetTeamMembers().begin(), GetTeamMembers().end(), Entity::FindEntityById(m_flagData[enemyTeam].m_carrierId));
	
		// If the flag carrier is still part of another (defensive) manoeuvre, release it from that first, before assigning it to the
		// new manoeuvre.

		if(m_entityManoeuvreMap[(*foundIt)->GetId()] != nullptr)
		{
			ReleaseEntityFromManoeuvre((*foundIt)->GetId());
		}

		m_manoeuvres[manoeuvre]->AddParticipant(*foundIt);
		m_entityManoeuvreMap[(*foundIt)->GetId()] = m_manoeuvres[manoeuvre];
	}

	if(manoeuvre != RunTheFlagHomeManoeuvre)
	{
		std::vector<Entity*> availableEntities;

		// Get all available entities
		for(std::vector<Entity*>::iterator it = GetTeamMembers().begin(); it != GetTeamMembers().end(); ++it)
		{
			// If the entity is alive and not yet registered with a manoeuvre, remember it as available
			if((*it)->IsAlive() && !m_entityManoeuvreMap[(*it)->GetId()])
			{
				availableEntities.push_back(*it);
			}
		}

		// Sort available entities according to their proximity to this position.
		XMFLOAT2 sortPosition(0.0f, 0.0f);

		switch(manoeuvre)
		{
		case DefendBaseEntrancesManoeuvre:
		case SimpleBaseDefenceManoeuvre:
		case ActiveBaseDefenceManoeuvre:
		case InterceptFlagCarrierManoeuvre:
			sortPosition = m_flagData[GetTeam()].m_basePosition;
			break;
		case RushBaseAttackManoeuvre:
		case CoordinatedBaseAttackManoeuvre:
		case DistractionBaseAttackManoeuvre:
		case SimpleBaseAttackManoeuvre:
			sortPosition = m_flagData[enemyTeam].m_basePosition;
			break;
		case PickUpDroppedFlagManoeuvre:
			sortPosition = m_flagData[enemyTeam].m_position;
			break;
		case ReturnDroppedFlagManoeuvre:
			sortPosition = m_flagData[GetTeam()].m_position;
			break;
		case GuardedFlagCaptureManoeuvre:
			sortPosition = m_flagData[enemyTeam].m_position;
			break;
		}

		// Sort the entities by distance to the current drop position of the enemy's flag
		std::sort(availableEntities.begin(), availableEntities.end(), Entity::CompareEntityByDistanceToTarget(sortPosition));
		
		// Start adding entities to the manoeuvre making sure that the closest ones are added first.
		unsigned int addedEntities = 0;

		unsigned int numberOfEntitiesToAdd = (manoeuvre != GuardedFlagCaptureManoeuvre) ? (m_manoeuvres[manoeuvre]->GetMaxNumberOfParticipants()) : (m_manoeuvres[manoeuvre]->GetMaxNumberOfParticipants() - 1);

		std::vector<Entity*>::iterator it = availableEntities.begin();

		// Add available entities to the manoeuvre until the maximally allowed number is reached.
		while((addedEntities < numberOfEntitiesToAdd) && (it != availableEntities.end()))
		{
			m_manoeuvres[manoeuvre]->AddParticipant(*it);
			// Remember that this entity is now executing that manoeuver
			m_entityManoeuvreMap[(*it)->GetId()] = m_manoeuvres[manoeuvre];
			++addedEntities;
			++it;
		}
	}
	
	return m_manoeuvres[manoeuvre]->Initiate();
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
	EntityTeam team = GetTeam();
	GetTestEnvironment()->RecordEvent(TeamManoeuvreTerminateLogEvent, &team, &manoeuvre);

	// Check if this AI has a manoeuvre of that type associated to it
	std::unordered_map<TeamManoeuvreType, TeamManoeuvre*>::iterator foundIt = m_manoeuvres.find(manoeuvre);
	if(foundIt != m_manoeuvres.end())
	{
		if(foundIt->second->IsActive())
		{
			// Release all entities from the manoeuvre that is about to terminate
			for(std::vector<Entity*>::const_iterator it = foundIt->second->GetParticipants().begin(); it != foundIt->second->GetParticipants().end(); ++it)
			{
				m_entityManoeuvreMap[(*it)->GetId()] = nullptr;
			}

			// Terminate the manoeuvre
			foundIt->second->Terminate();
			m_activeManoeuvres[foundIt->second->GetCategory()] = nullptr;
		}
	}else
	{
		// Forward the call to the base class
		TeamAI::TerminateManoeuvre(manoeuvre);
	}
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

	TeamAI::Reset();
}