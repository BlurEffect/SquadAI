/* 
*  Kevin Meergans, SquadAI, 2014
*  MovingEntity.cpp
*  Base class for all moving entities in the test environment.
*/

// Includes
#include "MovingEntity.h"

MovingEntity::MovingEntity(void) : Entity()
{
}

MovingEntity::~MovingEntity(void)
{
}

//--------------------------------------------------------------------------------------
// Initialises the moving entity.
// Param1: The basic initialisation data for the entity.
// Param2: The movement specific initialisation data for the entity.
// Returns true if the moving entity was initialised successfully, false otherwise.
//--------------------------------------------------------------------------------------
bool MovingEntity::Initialise(const EntityInitData& initData, const EntityMovementInitData& movementInitData)
{
	return Entity::Initialise(initData) &&
		   m_movementManager.Initialise(this, movementInitData);
}

//--------------------------------------------------------------------------------------
// Updates the moving entity.
// Param1: The time in seconds passed since the last frame.
//--------------------------------------------------------------------------------------
void MovingEntity::Update(float deltaTime)
{
	m_movementManager.Update(deltaTime);
}

//--------------------------------------------------------------------------------------
// Resets the entity.
//--------------------------------------------------------------------------------------
void MovingEntity::Reset(void)
{
	m_movementManager.Reset();
}

// Data access functions (forward calls to the movement manager)

const XMFLOAT2& MovingEntity::GetVelocity(void) const
{
	return m_movementManager.GetVelocity();
}

float MovingEntity::GetMaxVelocity(void) const
{
	return m_movementManager.GetMaxVelocity();
}

float MovingEntity::GetMaxTotalForce(void) const
{
	return m_movementManager.GetMaxTotalForce();
}

float MovingEntity::GetMaxSeeAhead(void) const
{
	return m_movementManager.GetMaxSeeAhead();
}

float MovingEntity::GetMaxCollisionAvoidanceForce(void) const
{
	return m_movementManager.GetMaxCollisionAvoidanceForce();
}

float MovingEntity::GetMaxSeparationForce(void) const
{
	return m_movementManager.GetMaxSeparationForce();
}

float MovingEntity::GetTargetReachedRadius(void) const
{
	return m_movementManager.GetTargetReachedRadius();
}

float MovingEntity::GetSlowArrivalRadius(void) const
{
	return m_movementManager.GetSlowArrivalRadius();
}

float MovingEntity::GetSeparationRadius(void) const
{
	return m_movementManager.GetSeparationRadius();
}

void MovingEntity::SetVelocity(const XMFLOAT2& velocity)
{
	m_movementManager.SetVelocity(velocity);
}

void MovingEntity::SetMaxVelocity(float maxVelocity)
{
	m_movementManager.SetMaxVelocity(maxVelocity);
}

void MovingEntity::SetMaxTotalForce(float maxForce)
{
	m_movementManager.SetMaxTotalForce(maxForce);
}

void MovingEntity::SetMaxSeeAhead(float maxSeeAhead)
{
	m_movementManager.SetMaxSeeAhead(maxSeeAhead);
}

void MovingEntity::SetMaxCollisionAvoidanceForce(float maxCollisionAvoidanceForce)
{
	m_movementManager.SetMaxCollisionAvoidanceForce(maxCollisionAvoidanceForce);
}

void MovingEntity::SetMaxSeparationForce(float maxSeparationForce)
{
	m_movementManager.SetMaxSeparationForce(maxSeparationForce);
}

void MovingEntity::SetTargetReachedRadius(float targetReachedRadius)
{
	m_movementManager.SetTargetReachedRadius(targetReachedRadius);
}

void MovingEntity::SetSlowArrivalRadius(float slowArrivalRadius)
{
	m_movementManager.SetSlowArrivalRadius(slowArrivalRadius);
}

void MovingEntity::SetSeparationRadius(float separationRadius)
{
	m_movementManager.SetSeparationRadius(separationRadius);
}




	