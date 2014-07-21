/* 
*  Kevin Meergans, SquadAI, 2014
*  MovingEntity.cpp
*  Base class for all moving entities in the test environment.
*/

// Includes
#include "FightingEntity.h"

FightingEntity::FightingEntity(void) : MovingEntity()
{
}

FightingEntity::~FightingEntity(void)
{
}

//--------------------------------------------------------------------------------------
// Initialises the fighting entity.
// Param1: The base initialisation data for the entity.
// Param2: The initialisation data required to initialise the movement component of the entity.
// Param3: The initialisation data required to initialise the sensors component of the entity.
// Param4: The initialisation data required to initialise the combat component of the entity.
// Returns true if the fighting entity was initialised successfully, false otherwise.
//--------------------------------------------------------------------------------------
bool FightingEntity::Initialise(const EntityInitData& initData, const EntityMovementInitData& movementInitData, const EntitySensorInitData& sensorInitData, const EntityCombatInitData& combatInitData)
{
	return MovingEntity::Initialise(initData, movementInitData) &&
		   m_sensors.Initialise(this, sensorInitData) && 
		   m_combatManager.Initialise(this, combatInitData);
}

//--------------------------------------------------------------------------------------
// Updates the fighting entity.
// Param1: The time in seconds passed since the last frame.
//--------------------------------------------------------------------------------------
void FightingEntity::Update(float deltaTime)
{
	MovingEntity::Update(deltaTime);
}

//--------------------------------------------------------------------------------------
// Resets the fighting entity.
//--------------------------------------------------------------------------------------
void FightingEntity::Reset(void)
{
	m_combatManager.Reset();
	MovingEntity::Reset();
}

//--------------------------------------------------------------------------------------
// Called when the entity is hit by a hostile projectile.
// Param1: The target position to shoot at.
// Param2: The velocity of the projectile that hit the entity. Used to determine the position of the threat.
//--------------------------------------------------------------------------------------
void FightingEntity::Hit(float damage, const XMFLOAT2& direction)
{
	m_combatManager.Hit(damage, direction);
}

// Data access functions

float FightingEntity::GetFieldOfView(void) const
{
	return m_sensors.GetFieldOfView();
}

float FightingEntity::GetViewingDistance(void) const
{
	return m_sensors.GetViewingDistance();
}

float FightingEntity::GetCurrentHealth(void) const
{
	return m_combatManager.GetCurrentHealth();
}

float FightingEntity::GetMaximalHealth(void) const
{
	return m_combatManager.GetMaximalHealth();
}

float FightingEntity::IsAlive(void) const
{
	return m_combatManager.IsAlive();
}

void FightingEntity::SetFieldOfView(float fieldOfView)
{
	m_sensors.SetFieldOfView(fieldOfView);
}

void FightingEntity::SetViewingDistance(float viewingDistance)
{
	m_sensors.SetViewingDistance(viewingDistance);
}

void FightingEntity::SetCurrentHealth(float health)
{
	return m_combatManager.SetCurrentHealth(health);
}