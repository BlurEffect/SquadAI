/* 
*  Kevin Meergans, SquadAI, 2014
*  FightingEntity.h
*  Base class for all fighting entities in the test environment.
*/

#ifndef FIGHTING_ENTITY_H
#define FIGHTING_ENTITY_H

// Includes
#include <DirectXMath.h>
#include "MovingEntity.h"
#include "EntityCombatManager.h"
#include "EntitySensors.h"

using namespace DirectX;

//--------------------------------------------------------------------------------------
// Bundles the data defining the properties for entity sensing.
//--------------------------------------------------------------------------------------
struct EntitySensorData
{
	EntitySensorData(void) : m_fieldOfView(0.0f),
							 m_viewingDistance(0.0f)
	{}

	EntitySensorData(float fieldOfView, float viewingDistance) : m_fieldOfView(fieldOfView),
													             m_viewingDistance(viewingDistance)
	{}

	float		    m_fieldOfView;     // The angle determining the field of view in radians
	float		    m_viewingDistance; // The entity can look this far ahead
};

//--------------------------------------------------------------------------------------
// Bundles the data defining the properties for entity combat.
//--------------------------------------------------------------------------------------
struct EntityCombatData
{
	EntityCombatData(void)
	{}

	EntityCombatData(float maxHealth) : m_maxHealth(maxHealth)
	{}

	float m_maxHealth; // The maximal health of the entity
};


class FightingEntity : public MovingEntity
{
public:
	FightingEntity(unsigned long id, EntityType type, const XMFLOAT2& position, float rotation, float scale, float radius, TestEnvironment* pEnvironment, const EntityMovementData& movementData, const EntitySensorData& sensorData, const EntityCombatData& combatData);
	virtual ~FightingEntity(void);

	virtual bool Initialise(void);
	virtual void Update(float deltaTime);

private:
	EntitySensorData	m_sensorData;    // The sensor properties of the entity
	EntityCombatData	m_combatData;    // The combat properties of the entity
	EntitySensors       m_sensors;       // The sensor component for the entity
	EntityCombatManager m_combatManager; // The manager responsible for updating the entities velocity and position
};

#endif // FIGHTING_ENTITY_H