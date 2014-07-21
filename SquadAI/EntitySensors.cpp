/* 
*  Kevin Meergans, SquadAI, 2014
*  EntitySensors.cpp
*  This class simulates the senses of the entity and updates the entity's
*  memory according to observations made and events perceived.
*/

// Includes
#include "EntitySensors.h"
#include "FightingEntity.h"
#include "TestEnvironment.h"


EntitySensors::EntitySensors(void) : m_fieldOfView(0.0f),
							         m_viewingDistance(0.0f)
{
}

EntitySensors::~EntitySensors(void)
{
}

//--------------------------------------------------------------------------------------
// Initialises the sensor component of the entity
// Param1: A pointer to the entity that the sensors are associated to.
// Param2: The initialisation data required to initialise the sensors of the entity.
// Returns true if the sensor component was initialised successfully, false otherwise.
//--------------------------------------------------------------------------------------
bool EntitySensors::Initialise(FightingEntity* pEntity, const EntitySensorInitData& initData)
{
	if(!pEntity)
	{
		return false;
	}

	m_pEntity         = pEntity;

	m_fieldOfView     = initData.m_fieldOfView;
	m_viewingDistance = initData.m_viewingDistance;

	return true;
}

//--------------------------------------------------------------------------------------
// Checks the field of view for hostile entities and adds them as threats to the 
// memory of the entity.
//--------------------------------------------------------------------------------------
void EntitySensors::CheckForThreats(void)
{
	std::list<FightingEntity*> enemies;

	m_pEntity->GetTestEnvironment()->GetEnemies(m_pEntity, enemies);
	if(!enemies.empty())
	{
		XMFLOAT2 gridPos;
		m_pEntity->GetTestEnvironment()->WorldToGridPosition(m_pEntity->GetPosition(), gridPos);

		// Get the vector that represents the direction the entity is looking to
		XMFLOAT2 viewVector;
		XMStoreFloat2(&viewVector, XMLoadFloat2(&m_pEntity->GetPosition()) + XMLoadFloat2(&m_pEntity->GetVelocity()));

		float squareViewingDistance = m_viewingDistance * m_viewingDistance;

		for(std::list<FightingEntity*>::iterator it = enemies.begin(); it != enemies.end(); ++it)
		{
			// Check if enemy is within line of sight and within visibiliy range

			if((*it)->IsAlive())
			{

				// Get the vector from the entity to the enemy
				XMFLOAT2 toEnemyVector;
				XMStoreFloat2(&toEnemyVector, XMLoadFloat2(&(*it)->GetPosition()) - XMLoadFloat2(&m_pEntity->GetPosition()));

				// Check the distance to the enemy
				float squareDistanceToEnemy = 0.0f;
				XMStoreFloat(&squareDistanceToEnemy, XMVector2Dot(XMLoadFloat2(&toEnemyVector), XMLoadFloat2(&toEnemyVector)));

				// Check if enemy is in range
				if(squareViewingDistance < squareViewingDistance)
				{
					// Get the angle between the vectors
					float angle = 0.0f;
					XMStoreFloat(&angle, XMVector2AngleBetweenVectors(XMLoadFloat2(&viewVector), XMLoadFloat2(&toEnemyVector)));
				
					// Check if enemy is in field of view
					if(abs(angle) <= m_fieldOfView)
					{
						XMFLOAT2 enemyGridPos;
						m_pEntity->GetTestEnvironment()->WorldToGridPosition((*it)->GetPosition(), enemyGridPos);

						// Check if enemy is visible or hidden behind an obstacle
						if(m_pEntity->GetTestEnvironment()->CheckLineOfSight(static_cast<int>(gridPos.x), static_cast<int>(gridPos.y), static_cast<int>(enemyGridPos.x), static_cast<int>(enemyGridPos.y)))
						{
							// Add threat
						}
					}	
				}
			}
		}
	}
}

// Data access functions

float EntitySensors::GetFieldOfView(void) const
{
	return m_fieldOfView;
}

float EntitySensors::GetViewingDistance(void) const
{
	return m_viewingDistance;
}

void EntitySensors::SetFieldOfView(float fieldOfView)
{
	m_fieldOfView = fieldOfView;
}

void EntitySensors::SetViewingDistance(float viewingDistance)
{
	m_viewingDistance = viewingDistance;
}