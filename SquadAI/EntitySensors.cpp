/* 
*  Kevin Meergans, SquadAI, 2014
*  EntitySensors.cpp
*  This class simulates the senses of the entity and updates the entity's
*  memory according to observations made and events perceived.
*/

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
	//std::list<FightingEntity*> enemies;

	//m_pTestEnvironment->GetEnemies(m_pEntity, enemies);

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
					if(m_pEnvironment->CheckLineOfSight(static_cast<int>(gridPos.x), static_cast<int>(gridPos.y), static_cast<int>(enemyGridPos.x), static_cast<int>(enemyGridPos.y)))
					{
						return;
						// Add threat
					}
				}	
			}
		}
	}
}
