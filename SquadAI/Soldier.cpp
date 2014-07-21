/* 
*  Kevin Meergans, SquadAI, 2014
*  Soldier.cpp
*  Represents a fighting entity in the test environment.
*/

// Includes
#include "Soldier.h"

Soldier::Soldier(void) : FightingEntity()
{
}

Soldier::~Soldier(void)
{
}

//--------------------------------------------------------------------------------------
// Initialises the soldier.
// Param1: The base initialisation data for the entity.
// Param2: The initialisation data required to initialise the movement component of the entity.
// Param3: The initialisation data required to initialise the sensors component of the entity.
// Param4: The initialisation data required to initialise the combat component of the entity.
// Returns true if the soldier was initialised successfully, false otherwise.
//--------------------------------------------------------------------------------------
bool Soldier::Initialise(EntityInitData initData, const EntityMovementInitData& movementInitData, const EntitySensorInitData& sensorInitData, const EntityCombatInitData& combatInitData)
{
	return FightingEntity::Initialise(initData, movementInitData, sensorInitData, combatInitData);
}

// Debug ->remove
float g_time = 0.0f;

//--------------------------------------------------------------------------------------
// Updates the soldier entity.
// Param1: The time in seconds passed since the last frame.
//--------------------------------------------------------------------------------------
void Soldier::Update(float deltaTime)
{
	//m_movementManager.FollowPath(GetTargetReachedRadius());
	//m_movementManager.AvoidObstacleCollisions(3.0f);
	//m_movementManager.Separate(GetSeparationRadius());

	//m_movementManager.Seek(XMFLOAT2(10.0f, 10.0f), 1.0f, 0.0f);
	g_time += deltaTime;

	if((g_time >= 2.0f) && GetType() == ASoldier)
	{
		XMFLOAT2 target;
		target.x = GetPosition().x + 1.0f;
		target.y = GetPosition().y;

		m_combatManager.ShootAt(target);
		g_time = 0.0f;
	}

	m_movementManager.Update(deltaTime);
}

void Soldier::Activate(void)
{
	m_movementManager.SetPathTo(XMFLOAT2(10.0f, 10.0f));
}