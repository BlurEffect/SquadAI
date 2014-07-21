/* 
*  Kevin Meergans, SquadAI, 2014
*  FightingEntity.h
*  Abstract base class for all fighting entities in the test environment.
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
struct EntitySensorInitData
{
	EntitySensorInitData(void) : m_fieldOfView(0.0f),
								 m_viewingDistance(0.0f)
	{}

	EntitySensorInitData(float fieldOfView, float viewingDistance) : m_fieldOfView(fieldOfView),
																     m_viewingDistance(viewingDistance)
	{}

	float m_fieldOfView;     // The angle determining the field of view in radians
	float m_viewingDistance; // The entity can look this far ahead
};

//--------------------------------------------------------------------------------------
// Bundles the data defining the properties for entity combat.
//--------------------------------------------------------------------------------------
struct EntityCombatInitData
{
	EntityCombatInitData(void)
	{}

	EntityCombatInitData(float maxHealth) : m_maxHealth(maxHealth)
	{}

	float m_maxHealth; // The maximal health of the entity
};


class FightingEntity : public MovingEntity
{
public:
	FightingEntity(void);
	virtual ~FightingEntity(void) = 0;

	bool Initialise(const EntityInitData& initData, const EntityMovementInitData& movementInitData, const EntitySensorInitData& sensorInitData, const EntityCombatInitData& combatInitData);
	virtual void Update(float deltaTime);
	virtual void Reset(void);
	virtual void Hit(float damage, const XMFLOAT2& direction);

	// Data access functions

	float GetFieldOfView(void) const;
	float GetViewingDistance(void) const;
	float GetCurrentHealth(void) const;
	float GetMaximalHealth(void) const;
	float IsAlive(void) const;

	void SetFieldOfView(float fieldOfView);
	void SetViewingDistance(float viewingDistance);
	void SetCurrentHealth(float health);
protected:
	EntitySensors       m_sensors;       // The sensor component for the entity
	EntityCombatManager m_combatManager; // The manager responsible for updating the entities velocity and position
};

#endif // FIGHTING_ENTITY_H