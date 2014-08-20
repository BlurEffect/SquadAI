/* 
*  Kevin Meergans, SquadAI, 2014
*  TeamAI.cpp
*  Abstract base class for all team AIs.
*/

#include "TeamAI.h"
#include "Entity.h"
#include "TestEnvironment.h"

TeamAI::TeamAI(void) : m_pBehaviour(nullptr),
				       m_team(None),
					   m_characteristic(CharNone),
					   m_pTestEnvironment(nullptr),
					   m_timeLeft(1.0f)
{
	for(unsigned int i = 0; i < NumberOfTeams-1; ++i)
	{
		m_scores[i] = 0;
	}
}

TeamAI::~TeamAI(void)
{
	if(m_pBehaviour)
	{
		delete m_pBehaviour;
		m_pBehaviour = nullptr;
	}

	ClearOrders();
}

//--------------------------------------------------------------------------------------
// Initialises the team AI.
// Param1: The team that this Ai will control.
// Param2: A pointer to the test environment, in which the matches take place.
//--------------------------------------------------------------------------------------
bool TeamAI::Initialise(EntityTeam team, TestEnvironment* pEnvironment, TeamAICharacteristic characteristic)
{
	if(!pEnvironment)
	{
		return false;
	}
	
	m_team = team;
	m_characteristic = characteristic;
	m_pTestEnvironment = pEnvironment;
	
	return true;
}

//--------------------------------------------------------------------------------------
// Updates the team AI.
// Param1: The time in seconds passed since the last frame.
//--------------------------------------------------------------------------------------
void TeamAI::Update(float deltaTime)
{
	if(deltaTime == 0.0f)
	{
		// No need to traverse the tree.
		return;
	}

	m_pBehaviour->Tick(deltaTime);
}

//--------------------------------------------------------------------------------------
// Cancels an order and deletes it from the list of active orders.
// Param1: The id of the entity, whose active order should be cancelled.
//--------------------------------------------------------------------------------------
void TeamAI::CancelOrder(unsigned long id)
{
	std::unordered_map<unsigned long, Order*>::iterator foundIt = m_activeOrders.find(id);

	if(foundIt != m_activeOrders.end())
	{
		delete foundIt->second;
		foundIt->second = nullptr;

		m_activeOrders.erase(foundIt);
	}
	
	// Find the entity
	std::vector<Entity*>::iterator foundEntity = std::find_if(m_teamMembers.begin(), m_teamMembers.end(), Entity::FindEntityById(id));
	if(foundEntity != m_teamMembers.end())
	{
		// Notify the entity that the order was cancelled
		SendMessage((*foundEntity), CancelOrderMessageType, nullptr);
	}
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
		std::pair<std::unordered_map<unsigned long, EnemyRecord>::iterator, bool> result = m_enemyRecords.insert(std::pair<unsigned long, EnemyRecord>(pMsg->GetData().m_enemyId, EnemyRecord(pMsg->GetData().m_enemyPosition, pMsg->GetData().m_spotterId)));
		if(!result.second)
		{
			// There already is a record for the spotted enemy -> Add the enemy as spotter and update the enemy position
			result.first->second.m_lastKnownPosition = pMsg->GetData().m_enemyPosition;
			result.first->second.m_spotterIds.insert(pMsg->GetData().m_spotterId);
		}

		// Update any attack orders on this enemy with the newest position
		UpdateAttackOrders(pMsg->GetData().m_enemyId);

		break;
		}
	case LostSightOfEnemyMessageType:
		{
		LostSightOfEnemyMessage* pMsg = reinterpret_cast<LostSightOfEnemyMessage*>(pMessage);

		// Remove the entity from the lost of entities that are able to see the enemy
		std::unordered_map<unsigned long, EnemyRecord>::iterator foundIt = m_enemyRecords.find(pMsg->GetData().m_enemyId);
		if(foundIt != m_enemyRecords.end())
		{
			foundIt->second.m_spotterIds.erase(pMsg->GetData().m_entityId);
		}
		break;
		}
	case UpdateEnemyPositionMessageType:
		{
		UpdateEnemyPositionMessage* pMsg = reinterpret_cast<UpdateEnemyPositionMessage*>(pMessage);
		std::unordered_map<unsigned long, EnemyRecord>::iterator foundIt = m_enemyRecords.find(pMsg->GetData().m_enemyId);
		if(foundIt != m_enemyRecords.end())
		{
			foundIt->second.m_lastKnownPosition = pMsg->GetData().m_enemyPosition;
			//UpdateAttackOrders(pMsg->GetData().m_enemyId);
		}
		break;
		}
	case EntityKilledMessageType:
		{
		EntityKilledMessage* pMsg = reinterpret_cast<EntityKilledMessage*>(pMessage);
		
		if(pMsg->GetData().m_team != GetTeam())
		{
			// An enemy was killed
			std::unordered_map<unsigned long, EnemyRecord>::iterator foundIt = m_enemyRecords.find(pMsg->GetData().m_id);
			if(foundIt != m_enemyRecords.end())
			{
				m_enemyRecords.erase(foundIt);
			}

			// Make sure to cancel all active attack orders associated to the killed enemy
			std::unordered_map<unsigned long, Order*>::iterator it = m_activeOrders.begin();
			while(it != m_activeOrders.end())
			{
				if(it->second->GetOrderType() == AttackEnemyOrder && reinterpret_cast<AttackOrder*>(it->second)->GetEnemyId() == pMsg->GetData().m_id)
				{
					// TODO: Check if this actually works, involves erasing of the iterator
					CancelOrder(it->first);
				}else
				{
					++it;
				}
			}

		}else
		{
			// A friendly was killed
			for(std::unordered_map<unsigned long, EnemyRecord>::iterator it = m_enemyRecords.begin(); it != m_enemyRecords.end(); ++it)
			{
				it->second.m_spotterIds.erase(pMsg->GetData().m_id);
			}

			// Team AI can decide whether it wants to cancel the orders of the killed member or not
		}
		break;
		}
	case AttackedByEnemyMessageType:
		{
		AttackedByEnemyMessage* pMsg = reinterpret_cast<AttackedByEnemyMessage*>(pMessage);
		// Add processing code
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
	case UpdateOrderStateMessageType:
		{
		UpdateOrderStateMessage* pMsg = reinterpret_cast<UpdateOrderStateMessage*>(pMessage);
		
		if(pMsg->GetData().m_orderState == SucceededOrderState || pMsg->GetData().m_orderState == FailedOrderState)
		{
			CancelOrder(pMsg->GetData().m_entityId);
		}
		break;
		}
	}
	
}

//--------------------------------------------------------------------------------------
// Updates the currently active attack orders by making sure the contained enemy 
// positions are up to data.
// Param1: The id of the enemy that moved and for which associated attack orders have to be updated.
//--------------------------------------------------------------------------------------
void TeamAI::UpdateAttackOrders(unsigned long enemyId)
{
	// Find the record for the enemy
	std::unordered_map<unsigned long, EnemyRecord>::iterator foundIt = m_enemyRecords.find(enemyId);

	if(foundIt == m_enemyRecords.end())
	{
		return;
	}

	for(std::unordered_map<unsigned long, Order*>::iterator it = m_activeOrders.begin(); it != m_activeOrders.end(); ++it)
	{
		if(it->second->GetOrderType() == AttackEnemyOrder)
		{
			AttackOrder* pAttackOrder = reinterpret_cast<AttackOrder*>(it->second);
			if(pAttackOrder->GetEnemyId() == enemyId)
			{
				// Update the order with the latest known position of the enemy
				pAttackOrder->SetEnemyPosition(foundIt->second.m_lastKnownPosition);
			}
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

//--------------------------------------------------------------------------------------
// Deletes all active orders.
//--------------------------------------------------------------------------------------
void TeamAI::ClearOrders(void)
{
	for(std::unordered_map<unsigned long, Order*>::iterator it = m_activeOrders.begin(); it != m_activeOrders.end(); ++it)
	{
		if(it->second)
		{
			delete it->second;
			it->second = nullptr;
		}
	}

	m_activeOrders.clear();
}

//--------------------------------------------------------------------------------------
// Processes a message sent to the team AI.
// Param1: A pointer to the message to process.
//--------------------------------------------------------------------------------------
void TeamAI::InitiateManoeuvre(TeamManoeuvreType manoeuvre)
{
	
}

//--------------------------------------------------------------------------------------
// Processes a message sent to the team AI.
// Param1: A pointer to the message to process.
// Param2: The time passed since the last update.
// Returns a behaviour status code representing the current state of the manoeuvre.
//--------------------------------------------------------------------------------------
BehaviourStatus TeamAI::UpdateManoeuvre(TeamManoeuvreType manoeuvre, float deltaTime)
{
	return StatusSuccess;
}

//--------------------------------------------------------------------------------------
// Processes a message sent to the team AI.
// Param1: A pointer to the message to process.
//--------------------------------------------------------------------------------------
void TeamAI::TerminateManoeuvre(TeamManoeuvreType manoeuvre)
{

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

	// Clear team members
	m_teamMembers.clear();
}

// Data access functions

EntityTeam TeamAI::GetTeam(void) const
{
	return m_team;
}

TeamAICharacteristic TeamAI::GetCharacteristic(void) const
{
	return m_characteristic;
}

const TestEnvironment* TeamAI::GetTestEnvironment(void) const
{
	return m_pTestEnvironment;
}

std::vector<Entity*>& TeamAI::GetTeamMembers(void)
{
	return m_teamMembers;
}

std::unordered_map<unsigned long, EnemyRecord>& TeamAI::GetEnemyRecords(void)
{
	return m_enemyRecords;
}

std::unordered_map<unsigned long, Order*>& TeamAI::GetActiveOrders(void)
{
	return m_activeOrders;
}

float TeamAI::GetScore(EntityTeam team) const
{
	return m_scores[team];
}

float TeamAI::GetTimeLeft(void) const
{
	return m_timeLeft;
}

void TeamAI::SetTeam(EntityTeam team)
{
	m_team = team;
}

void TeamAI::SetCharacteristic(TeamAICharacteristic characteristic)
{
	m_characteristic = characteristic;
}

void TeamAI::SetTestEnvironment(TestEnvironment* pEnvironment)
{
	m_pTestEnvironment = pEnvironment;
}

