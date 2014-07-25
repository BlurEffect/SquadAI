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
	if(deltaTime == 0.0f)
	{
		return;
	}

	m_movementManager.FollowPath(GetTargetReachedRadius());
	m_movementManager.AvoidObstacleCollisions(GetMaxSeeAhead());
	m_movementManager.Separate(GetSeparationRadius());
	m_movementManager.StayAwayFromWalls(GetAvoidWallsRadius());

	if(GetType() == RedSoldierType)
	{
		//m_sensors.CheckForThreats();
		//m_movementManager.Seek(XMFLOAT2(24.0f, 24.0f), 1.0f);
	}else
	{
		//m_movementManager.Seek(XMFLOAT2(-24.0f, -24.0f), 1.0f);
	}
	
	g_time += deltaTime;

	if((g_time >= 2.0f) && GetType() == RedSoldierType)
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
	//m_movementManager.SetPathTo(XMFLOAT2(20.0f, 20.0f));

	if(GetType() == RedSoldierType)
	{
		m_movementManager.SetPathTo(XMFLOAT2(20.0f, 20.0f));
	}else
	{
		m_movementManager.SetPathTo(XMFLOAT2(-20.0f, -20.0f));
	}
}