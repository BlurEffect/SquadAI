/* 
*  Kevin Meergans, SquadAI, 2014
*  TeamAI.cpp
*  Abstract base class for all team AIs.
*/

#include "TeamAI.h"
#include "Entity.h"
#include "TestEnvironment.h"

TeamAI::TeamAI(EntityTeam team, TestEnvironment* pEnvironment) : m_team(team),
																 m_pTestEnvironment(pEnvironment),
																 m_timeLeft(1.0f)
{
	for(unsigned int i = 0; i < NumberOfTeams-1; ++i)
	{
		m_scores[i] = 0;
	}
}

TeamAI::~TeamAI(void)
{
}

//--------------------------------------------------------------------------------------
// Processes all inbox messages that the team AI received.
// Param1: A pointer to the message to process.
//--------------------------------------------------------------------------------------
void TeamAI::ProcessMessage(Message* pMessage)
{
	switch(pMessage->GetType())
	{
	case EnemySpottedMessageType:
		{
		EnemySpottedMessage* pMsg = reinterpret_cast<EnemySpottedMessage*>(pMessage);
		// Try to add the new enemy record.
		std::pair<std::map<unsigned long, EnemyRecord>::iterator, bool> result = m_enemyRecords.insert(std::pair<unsigned long, EnemyRecord>(pMsg->GetData().m_enemyId, EnemyRecord(pMsg->GetData().m_enemyPosition, pMsg->GetData().m_spotterId)));
		if(!result.second)
		{
			// There already is a record for the spotted enemy -> Add the enemy as spotter and update the enemy position
			result.first->second.m_lastKnownPosition = pMsg->GetData().m_enemyPosition;
			result.first->second.m_spotterIds.insert(pMsg->GetData().m_spotterId);
		}
		break;
		}
	case LostSightOfEnemyMessageType:
		{
		LostSightOfEnemyMessage* pMsg = reinterpret_cast<LostSightOfEnemyMessage*>(pMessage);

		// Remove the entity from the lost of entities that are able to see the enemy
		std::map<unsigned long, EnemyRecord>::iterator foundIt = m_enemyRecords.find(pMsg->GetData().m_enemyId);
		if(foundIt != m_enemyRecords.end())
		{
			foundIt->second.m_spotterIds.erase(pMsg->GetData().m_entityId);
		}
		break;
		}
	case UpdateEnemyPositionMessageType:
		{
		UpdateEnemyPositionMessage* pMsg = reinterpret_cast<UpdateEnemyPositionMessage*>(pMessage);
		std::map<unsigned long, EnemyRecord>::iterator foundIt = m_enemyRecords.find(pMsg->GetData().m_enemyId);
		if(foundIt != m_enemyRecords.end())
		{
			foundIt->second.m_lastKnownPosition = pMsg->GetData().m_enemyPosition;
		}
		break;
		}
	case EnemyKilledMessageType:
		{
		EnemyKilledMessage* pMsg = reinterpret_cast<EnemyKilledMessage*>(pMessage);
		std::map<unsigned long, EnemyRecord>::iterator foundIt = m_enemyRecords.find(pMsg->GetData().m_enemyId);
		if(foundIt != m_enemyRecords.end())
		{
			m_enemyRecords.erase(foundIt);
		}
		break;
		}
	case ScoreUpdateMessageType:
		{
		ScoreUpdateMessage* pMsg = reinterpret_cast<ScoreUpdateMessage*>(pMessage);
		m_scores[pMsg->GetData().m_team] = static_cast<float>(pMsg->GetData().m_score) / pMsg->GetData().m_maxScore;
		break;
		}
	case TimeUpdateMessageType:
		{
		TimeUpdateMessage* pMsg = reinterpret_cast<TimeUpdateMessage*>(pMessage);
		m_timeLeft = pMsg->GetData().m_timeLeft / pMsg->GetData().m_maxTime;
		break;
		}
	}
	
}

//--------------------------------------------------------------------------------------
// Process a given event. Default implementation.
// Param1: The type of event.
// Param2: A pointer to the event data.
//--------------------------------------------------------------------------------------
void TeamAI::ProcessEvent(EventType type, void* pEventData)
{
	// Not expecting any events, just call the default implementation
	Communicator::ProcessEvent(type, pEventData);
}

/*
//--------------------------------------------------------------------------------------
// Processes a message sent to the team AI.
// Param1: A pointer to the message to process.
//--------------------------------------------------------------------------------------
void TeamAI::ProcessMessage(Message* pMessage)
{
	switch(pMessage->GetType())
	{
	case EnemySpottedMessageType:
		{
		EnemySpottedMessage* pMsg = reinterpret_cast<EnemySpottedMessage*>(pMessage);
		// Try to add the new enemy record.
		std::pair<std::map<unsigned long, EnemyRecord>::iterator, bool> result = m_enemyRecords.insert(std::pair<unsigned long, EnemyRecord>(pMsg->GetEnemyId(), EnemyRecord(pMsg->GetEnemyPosition(), pMsg->GetSpotterId())));
		if(!result.second)
		{
			// There already is a record for the spotted enemy -> Add the enemy as spotter and update the enemy position
			result.first->second.m_lastKnownPosition = pMsg->GetEnemyPosition();
			result.first->second.m_spotterIds.insert(pMsg->GetSpotterId());
		}
		break;
		}
	case LostSightOfEnemyMessageType:
		{
		LostSightOfEnemyMessage* pMsg = reinterpret_cast<LostSightOfEnemyMessage*>(pMessage);

		// Remove the entity from the lost of entities that are able to see the enemy
		std::map<unsigned long, EnemyRecord>::iterator foundIt = m_enemyRecords.find(pMsg->GetEnemyId());
		if(foundIt != m_enemyRecords.end())
		{
			foundIt->second.m_spotterIds.erase(pMsg->GetEntityId());
		}
		break;
		}
	case UpdateEnemyPositionMessageType:
		{
		UpdateEnemyPositionMessage* pMsg = reinterpret_cast<UpdateEnemyPositionMessage*>(pMessage);
		std::map<unsigned long, EnemyRecord>::iterator foundIt = m_enemyRecords.find(pMsg->GetEnemyId());
		if(foundIt != m_enemyRecords.end())
		{
			foundIt->second.m_lastKnownPosition = pMsg->GetEnemyPosition();
		}
		break;
		}
	case EnemyKilledMessageType:
		{
		EnemyKilledMessage* pMsg = reinterpret_cast<EnemyKilledMessage*>(pMessage);
		std::map<unsigned long, EnemyRecord>::iterator foundIt = m_enemyRecords.find(pMsg->GetEnemyId());
		if(foundIt != m_enemyRecords.end())
		{
			m_enemyRecords.erase(foundIt);
		}
		break;
		}
	case ScoreUpdateMessageType:
		{
		ScoreUpdateMessage* pMsg = reinterpret_cast<ScoreUpdateMessage*>(pMessage);
		m_scores[pMsg->GetTeam()] = static_cast<float>(pMsg->GetScore()) / pMsg->GetMaxScore();
		break;
		}
	case TimeUpdateMessageType:
		{
		TimeUpdateMessage* pMsg = reinterpret_cast<TimeUpdateMessage*>(pMessage);
		m_timeLeft = pMsg->GetTimeLeft() / pMsg->GetMaxTime();
		break;
		}
	}
}
*/

//--------------------------------------------------------------------------------------
// Puts an entity under the control of the team AI.
// Param1: A pointer to the entity that should be put under the control of the team AI.
//--------------------------------------------------------------------------------------
void TeamAI::AddTeamMember(Entity* pEntity)
{
	if(pEntity)
	{
		std::vector<Entity*>::iterator foundIt = std::find_if(m_teamMembers.begin(), m_teamMembers.end(), Entity::FindEntityById(pEntity->GetId()));
		if(foundIt == m_teamMembers.end())
		{
			// Add the team member only if it is not already a member.
			m_teamMembers.push_back(pEntity);
		}
	}
}

//--------------------------------------------------------------------------------------
// Removes an entity from the control of the team AI.
// Param1: The ID of the entity that should no longer be controlled by the team AI.
//--------------------------------------------------------------------------------------
void TeamAI::RemoveTeamMember(unsigned long id)
{
	std::vector<Entity*>::iterator foundIt = std::find_if(m_teamMembers.begin(), m_teamMembers.end(), Entity::FindEntityById(id));
	if(foundIt != m_teamMembers.end())
	{
		// Remove the team member.
		m_teamMembers.erase(foundIt);
	}
}

//--------------------------------------------------------------------------------------
// Resets the team AI after a completed match.
//--------------------------------------------------------------------------------------
void TeamAI::Reset(void)
{
	m_enemyRecords.clear();
	ResetCommunication();

	for(unsigned int i = 0; i < NumberOfTeams-1; ++i)
	{
		m_scores[i] = 0;

	}	m_timeLeft = 1.0f;
}

// Data access functions

EntityTeam TeamAI::GetTeam(void) const
{
	return m_team;
}

const TestEnvironment* TeamAI::GetTestEnvironment(void) const
{
	return m_pTestEnvironment;
}

void TeamAI::SetTeam(EntityTeam team)
{
	m_team = team;
}

void TeamAI::SetTestEnvironment(TestEnvironment* pEnvironment)
{
	m_pTestEnvironment = pEnvironment;
}
