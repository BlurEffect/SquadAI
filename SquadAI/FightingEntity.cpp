/* 
*  Kevin Meergans, SquadAI, 2014
*  MovingEntity.cpp
*  Base class for all moving entities in the test environment.
*/

// Includes
#include "FightingEntity.h"

FightingEntity::FightingEntity(unsigned long id, EntityType type, const XMFLOAT2& position, float rotation, float scale, float radius, TestEnvironment* pEnvironment, const EntityMovementData& movementData, const EntitySensorData& sensorData, const EntityCombatData& combatData)
	: MovingEntity(id, type, position, rotation, scale, radius, pEnvironment, movementData),																																						
	  m_sensorData(sensorData),
	  m_combatData(combatData)
{
}

FightingEntity::~FightingEntity(void)
{
}

//--------------------------------------------------------------------------------------
// Initialises the fighting entity.
// Returns true if the fighting entity was initialised successfully, false otherwise.
//--------------------------------------------------------------------------------------
bool FightingEntity::Initialise(void)
{
	return m_sensors.Initialise(m_sensorData.m_fieldOfView, m_sensorData.m_viewingDistance, this) && 
		   m_combatManager.Initialise(this) &&
		   MovingEntity::Initialise();

}

//--------------------------------------------------------------------------------------
// Updates the fighting entity.
// Param1: The time in seconds passed since the last frame.
//--------------------------------------------------------------------------------------
void FightingEntity::Update(float deltaTime)
{
	MovingEntity::Update(deltaTime);
}

// Data access functions
