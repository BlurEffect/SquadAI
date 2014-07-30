/* 
*  Kevin Meergans, SquadAI, 2014
*  Soldier.cpp
*  Represents a fighting entity in the test environment.
*/

// Includes
#include "Soldier.h"

Soldier::Soldier(void) : Entity()
{
}

Soldier::~Soldier(void)
{
}

//--------------------------------------------------------------------------------------
// Initialises the soldier.
// Param1:  An ID uniquely identifying this entity.
// Param2:  The start position of the entity.
// Param3:  The initial rotation of the entity.
// Param4:  The uniform scale of the entity.
// Param5:  The category the object belongs to.
// Param6:  The type of the collider that should be created for this entity.
// Param7:  The collider data that should be used for the creation of the collider.
// Param8:  A pointer to the test environment the soldier is part of.
// Param9:  Identifies the team that this entity belongs to.
// Param10: The bundled soldier-specific properties.
// Returns true if the soldier was initialised successfully, false otherwise.
//--------------------------------------------------------------------------------------
bool Soldier::Initialise(unsigned long id, const XMFLOAT2& position, float rotation, float uniformScale, ObjectCategory category, ColliderType colliderType, void* pColliderData, TestEnvironment* pEnvironment, EntityTeam team, const SoldierProperties& soldierProperties)
{
	if(!Entity::Initialise(id, position, rotation, uniformScale, category, colliderType, pColliderData, pEnvironment, soldierProperties.m_maxHealth, team))
	{
		return false;
	}

	m_soldierProperties  = soldierProperties;

	Reset();      

	return (m_movementManager.Initialise(this, GetTestEnvironment()) && m_combatManager.Initialise(this, GetTestEnvironment()) && m_sensors.Initialise(this, GetTestEnvironment()));
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

	m_movementManager.FollowPath(GetTargetReachedRadius(), m_soldierProperties.m_maxSpeed);
	m_movementManager.AvoidCollisions(m_soldierProperties.m_maxCollisionSeeAhead, m_soldierProperties.m_maxCollisionAvoidanceForce);
	m_movementManager.Separate(m_soldierProperties.m_separationRadius, m_soldierProperties.m_maxSeparationForce);
	m_movementManager.StayAwayFromWalls(m_soldierProperties.m_avoidWallsRadius, m_soldierProperties.m_maxAvoidWallsForce);

	if(GetTeam() == TeamRed)
	{
		//m_sensors.CheckForThreats();
		//m_movementManager.Seek(XMFLOAT2(24.0f, 24.0f), 1.0f);
	}else
	{
		//m_movementManager.Seek(XMFLOAT2(-24.0f, -24.0f), 1.0f);
	}
	
	g_time += deltaTime;

	if((g_time >= 2.0f) && GetTeam() == TeamRed)
	{
		XMFLOAT2 target;
		target.x = GetPosition().x + 1.0f;
		target.y = GetPosition().y;

		m_combatManager.ShootAt(target);
		g_time = 0.0f;
	}

	m_movementManager.UpdatePosition(deltaTime, m_soldierProperties.m_maxSpeed, m_soldierProperties.m_maxTotalForce);
}

//--------------------------------------------------------------------------------------
// Moves the soldier entity towards a given target.
// Param1: The time in seconds passed since the last frame.
// Param2: The position, to which the soldier should move.
// Returns true if the target was reached, false otherwise.
//--------------------------------------------------------------------------------------
bool Soldier::MoveTo(float deltaTime, const XMFLOAT2& targetPosition)
{
	return true;
}

//--------------------------------------------------------------------------------------
// Makes the soldier attack a target at the given position.
// Param1: The time in seconds passed since the last frame.
// Param2: The position, that the soldier should attack.
// Returns true if the attack was successfully executed, false otherwise.
//--------------------------------------------------------------------------------------
bool Soldier::Attack(float deltaTime, const XMFLOAT2& targetPosition)
{
	return true;
}

//--------------------------------------------------------------------------------------
// Makes the soldier aim at a given position.
// Param1: The time in seconds passed since the last frame.
// Param2: The position, at which the soldier should aim.
// Returns true if the soldier is now aiming at the given position, false otherwise.
//--------------------------------------------------------------------------------------
bool Soldier::AimAt(float deltaTime, const XMFLOAT2& aimAtPosition)
{
	return true;
}

//--------------------------------------------------------------------------------------
// Determines and sets a patrol target position for the soldier.
// Param1: The time in seconds passed since the last frame.
// Returns true if a valid target position was found and set, false otherwise.
//--------------------------------------------------------------------------------------
bool Soldier::DeterminePatrolTarget(float deltaTime)
{
	return true;
}

//--------------------------------------------------------------------------------------
// Determines and sets a target position for the soldier to move to in order to approach threats.
// Param1: The time in seconds passed since the last frame.
// Returns true if a valid target position was found and set, false otherwise.
//--------------------------------------------------------------------------------------
bool Soldier::DetermineApproachThreatTarget(float deltaTime)
{
	return true;
}

//--------------------------------------------------------------------------------------
// Updates the known and suspected threats for the soldier.
// Param1: The time in seconds passed since the last frame.
// Returns true if the threats were updated successfully, false otherwise.
//--------------------------------------------------------------------------------------
bool Soldier::UpdateThreats(float deltaTime)
{
	return true;
}

//--------------------------------------------------------------------------------------
// Updates the attack readiness of the soldier.
// Param1: The time in seconds passed since the last frame.
// Returns true if the attack readiness was updated successfully, false otherwise.
//--------------------------------------------------------------------------------------
bool Soldier::UpdateAttackReadiness(float deltaTime)
{
	return true;
}


//--------------------------------------------------------------------------------------
// Activates the soldier entity.
//--------------------------------------------------------------------------------------
void Soldier::Activate(void)
{
	//m_movementManager.SetPathTo(XMFLOAT2(20.0f, 20.0f));

	if(GetTeam() == TeamRed)
	{
		m_movementManager.SetPathTo(XMFLOAT2(20.0f, 20.0f));
	}else
	{
		m_movementManager.SetPathTo(XMFLOAT2(-20.0f, -20.0f));
	}
}

//--------------------------------------------------------------------------------------
// Called when the entity is hit by a hostile projectile.
// Param1: The target position to shoot at.
// Param2: The velocity of the projectile that hit the entity. Used to determine the position of the threat.
//--------------------------------------------------------------------------------------
void Soldier::Hit(float damage, const XMFLOAT2& direction)
{
	// add new suspected threat somewhere -> event queue?

	SetCurrentHealth(GetCurrentHealth() - damage);

}


//--------------------------------------------------------------------------------------
// Resets the soldier entity.
//--------------------------------------------------------------------------------------
void Soldier::Reset(void)
{
	m_movementManager.Reset();
	m_combatManager.Reset();

	Entity::Reset();
}

// Data access functions

// Movement related

const XMFLOAT2& Soldier::GetVelocity(void) const
{
	return m_movementManager.GetVelocity();
}

float Soldier::GetMaxSpeed(void) const
{
	return m_soldierProperties.m_maxSpeed;
}

float Soldier::GetMaxTotalForce(void) const
{
	return m_soldierProperties.m_maxTotalForce;
}

float Soldier::GetMaxCollisionSeeAhead(void) const
{
	return m_soldierProperties.m_maxCollisionSeeAhead;
}

float Soldier::GetMaxCollisionAvoidanceForce(void) const
{
	return m_soldierProperties.m_maxCollisionAvoidanceForce;
}

float Soldier::GetMaxAvoidWallsForce(void) const
{
	return m_soldierProperties.m_maxAvoidWallsForce;
}

float Soldier::GetMaxSeparationForce(void) const
{
	return m_soldierProperties.m_maxSeparationForce;
}

float Soldier::GetTargetReachedRadius(void) const
{
	return m_soldierProperties.m_targetReachedRadius;
}

float Soldier::GetAvoidWallsRadius(void) const
{
	return m_soldierProperties.m_maxAvoidWallsForce;
}

float Soldier::GetSeparationRadius(void) const
{
	return m_soldierProperties.m_separationRadius;
}

const XMFLOAT2& Soldier::GetViewDirection(void) const
{
	return m_movementManager.GetViewDirection();
}

float Soldier::GetFieldOfView(void) const
{
	return m_soldierProperties.m_fieldOfView;
}

float Soldier::GetViewingDistance(void) const
{
	return m_soldierProperties.m_viewingDistance;
}

void Soldier::SetMaxSpeed(float maxSpeed)
{
	m_soldierProperties.m_maxSpeed = maxSpeed;
}

void Soldier::SetMaxTotalForce(float maxForce)
{
	m_soldierProperties.m_maxTotalForce = maxForce;
}

void Soldier::SetMaxCollisionSeeAhead(float maxSeeAhead)
{
	m_soldierProperties.m_maxCollisionSeeAhead = maxSeeAhead;
}

void Soldier::SetMaxCollisionAvoidanceForce(float maxCollisionAvoidanceForce)
{
	m_soldierProperties.m_maxCollisionAvoidanceForce = maxCollisionAvoidanceForce;
}

void Soldier::SetMaxAvoidWallsForce(float maxAvoidWallsForce)
{
	m_soldierProperties.m_maxAvoidWallsForce = maxAvoidWallsForce;
}

void Soldier::SetMaxSeparationForce(float maxSeparationForce)
{
	m_soldierProperties.m_maxSeparationForce = maxSeparationForce;
}

void Soldier::SetTargetReachedRadius(float targetReachedRadius)
{
	m_soldierProperties.m_targetReachedRadius = targetReachedRadius;
}

void Soldier::SetAvoidWallsRadius(float avoidWallsRadius)
{
	m_soldierProperties.m_avoidWallsRadius = avoidWallsRadius;
}

void Soldier::SetSeparationRadius(float separationRadius)
{
	m_soldierProperties.m_separationRadius = separationRadius;
}

void Soldier::SetFieldOfView(float fieldOfView)
{
	m_soldierProperties.m_fieldOfView = fieldOfView;
}

void Soldier::SetViewingDistance(float viewingDistance)
{
	m_soldierProperties.m_viewingDistance = viewingDistance;
}
