/* 
*  Kevin Meergans, SquadAI, 2014
*  EntitySensors.cpp
*  This class simulates the senses of the entity and updates the entity's
*  memory according to observations made and events perceived.
*/

#define DEBUG

// Includes
#include "EntitySensors.h"
#include "Entity.h"
#include "TestEnvironment.h"


EntitySensors::EntitySensors(void) : m_pEntity(nullptr),
							         m_pEnvironment(nullptr)
{
}

EntitySensors::~EntitySensors(void)
{
}

//--------------------------------------------------------------------------------------
// Initialises the sensor component of the entity
// Param1: A pointer to the entity that the combat manager is associated to.
// Param2: A pointer to the test environment that the entity belongs to.
// Returns true if the sensor component was initialised successfully, false otherwise.
//--------------------------------------------------------------------------------------
bool EntitySensors::Initialise(Entity* pEntity, TestEnvironment* pTestEnvironment)
{
	if(!pEntity || !pTestEnvironment)
	{
		return false;
	}

	m_pEntity      = pEntity;
	m_pEnvironment = pTestEnvironment;

	return true;
}

//--------------------------------------------------------------------------------------
// Checks the field of view for hostile entities and adds them as threats to the 
// memory of the entity.
// Param1: The direction, into which the entity is currently viewing.
// Param2: The range how far the entity can see.
// Param3: The field of view of the entity.
//--------------------------------------------------------------------------------------
void EntitySensors::CheckForThreats(const XMFLOAT2& viewDirection, float viewingRange, float fieldOfView)
{
	// Find nearby hostile entities.

	std::multimap<float, CollidableObject*> enemies;

	if(m_pEntity->GetTeam() == TeamRed)
	{
		m_pEnvironment->GetNearbyObjects(m_pEntity->GetPosition(), viewingRange, GroupTeamBlue, enemies);
	}else
	{
		m_pEnvironment->GetNearbyObjects(m_pEntity->GetPosition(), viewingRange, GroupTeamRed, enemies);
	}

	if(!enemies.empty())
	{
		std::vector<Entity*> newKnownThreats;

		XMFLOAT2 gridPos;
		m_pEnvironment->WorldToGridPosition(m_pEntity->GetPosition(), gridPos);

		// Get the vector that represents the direction the entity is looking to
		XMFLOAT2 viewVector;
		XMStoreFloat2(&viewVector, XMLoadFloat2(&viewDirection));

		for(std::multimap<float, CollidableObject*>::iterator it = enemies.begin(); it != enemies.end(); ++it)
		{
			// Check if enemy is within line of sight and within visibiliy range
			if(reinterpret_cast<Entity*>(it->second)->IsAlive())
			{
				// Get the vector from the entity to the enemy
				XMFLOAT2 toEnemyVector;
				XMStoreFloat2(&toEnemyVector, XMLoadFloat2(&it->second->GetPosition()) - XMLoadFloat2(&m_pEntity->GetPosition()));

				// Get the angle between the vectors
				float angle = 0.0f;
				XMStoreFloat(&angle, XMVector2AngleBetweenVectors(XMLoadFloat2(&viewVector), XMLoadFloat2(&toEnemyVector)));
				
				// Check if enemy is in field of view
				if(abs(angle) <= fieldOfView)
				{
					XMFLOAT2 enemyGridPos;
					m_pEnvironment->WorldToGridPosition(it->second->GetPosition(), enemyGridPos);

					// Check if enemy is visible or hidden behind an obstacle
					if(m_pEnvironment->CheckLineOfSight(m_pEntity->GetPosition(), it->second->GetPosition()))
					{
						// Remember this enemy as a known threat
						newKnownThreats.push_back(reinterpret_cast<Entity*>(it->second));
#ifdef DEBUG
						m_pEntity->GetTestEnvironment()->RecordEvent(EnemySpottedLogEvent, m_pEntity, it->second);
#endif
					}
				}	
			}
		}

		// Update the entity's threats

		// Check for any previously known threats that are no longer visible and move them to the list of
		// suspected threats.

		std::vector<KnownThreat>::iterator it = m_pEntity->GetKnownThreats().begin();
		while(it != m_pEntity->GetKnownThreats().end())
		{
			std::vector<Entity*>::const_iterator foundIt = std::find_if(newKnownThreats.begin(), newKnownThreats.end(), Entity::FindEntityById(it->m_pEntity->GetId()));
			if(foundIt == newKnownThreats.end())
			{
				if(it->m_pEntity->IsAlive() && !m_pEntity->IsSuspectedThreat(it->m_pEntity->GetId()))
				{
					// The previously known threat can no longer be seen -> add to suspected threats
					m_pEntity->AddSuspectedThreat(it->m_pEntity->GetId(), it->m_pEntity->GetPosition(), false);

					// Notify team AI
					LostSightOfEnemyMessageData data(m_pEntity->GetId(), it->m_pEntity->GetId());
					m_pEntity->SendMessage(m_pEntity->GetTeamAI(), LostSightOfEnemyMessageType, &data);
				}

				it = m_pEntity->GetKnownThreats().erase(it);
			}else
			{
				++it;
			}
		}
		/*
		for(std::vector<KnownThreat>::iterator it = m_pEntity->GetKnownThreats().begin(); it != m_pEntity->GetKnownThreats().end(); ++it)
		{
			std::vector<Entity*>::const_iterator foundIt = std::find_if(newKnownThreats.begin(), newKnownThreats.end(), Entity::FindEntityById(it->m_pEntity->GetId()));
			if(foundIt == newKnownThreats.end())
			{
				if(it->m_pEntity->IsAlive() && !m_pEntity->IsSuspectedThreat(it->m_pEntity->GetId()))
				{
					// The previously known threat can no longer be seen -> add to suspected threats
					m_pEntity->AddSuspectedThreat(it->m_pEntity->GetId(), it->m_pEntity->GetPosition(), false);
				}

				m_pEntity->GetKnownThreats().erase(it++);
			}
		}
		*/
		// Set new known threats
		//m_pEntity->ClearKnownThreats();
		for(std::vector<Entity*>::iterator it = newKnownThreats.begin(); it != newKnownThreats.end(); ++it)
		{
			// Check if the threat was known before
			if(!m_pEntity->IsKnownThreat((*it)->GetId()))
			{

				// Notify team AI
				EnemySpottedMessageData data(m_pEntity->GetId(), (*it)->GetId(), (*it)->GetPosition());
				m_pEntity->SendMessage(m_pEntity->GetTeamAI(), EnemySpottedMessageType, &data);

				if(m_pEntity->IsSuspectedThreat((*it)->GetId()))
				{
					// If this known threat previously was a suspected threat (that has become visible now), remove it
					// from the list of suspected threats and add it as known threat.
				
					// Carry over whether the entity was hit from the previously suspected threat.
					m_pEntity->AddKnownThreat((*it), m_pEntity->GetSuspectedThreat((*it)->GetId())->m_hasHitEntity);

					m_pEntity->RemoveSuspectedThreat((*it)->GetId());
				
				}else
				{
					m_pEntity->AddKnownThreat((*it), false);
				}
			}else
			{
				// Note: Should only be called if enemy actually moved

				// Notify team AI.
				// Update the team AI's knowledge of the position of the enemy.
				UpdateEnemyPositionMessageData data((*it)->GetId(), (*it)->GetPosition());
				m_pEntity->SendMessage(m_pEntity->GetTeamAI(), UpdateEnemyPositionMessageType, &data);
			}
		}

	}else
	{
		// There are no enemies in range at the moment.

		if(!m_pEntity->GetKnownThreats().empty())
		{
			// Move all known threats to suspected threats.
			for(std::vector<KnownThreat>::const_iterator it = m_pEntity->GetKnownThreats().begin(); it != m_pEntity->GetKnownThreats().end(); ++it)
			{
				if(!m_pEntity->IsSuspectedThreat(it->m_pEntity->GetId()))
				{
					// The previously known threat can no longer be seen and is not yet contained in the list
					// of suspected threats -> Add to suspected threats.
					m_pEntity->AddSuspectedThreat(it->m_pEntity->GetId(), it->m_pEntity->GetPosition(), false);
				}
			}

			m_pEntity->ClearKnownThreats();
		}
	}
}
