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

MovingEntity::MovingEntity(unsigned long id, EntityType type, const XMFLOAT2& position, float rotation, float scale, float radius, TestEnvironment* pEnvironment, const EntityMovementData& movementData) 
	: Entity(id, type, position, rotation, scale, radius, pEnvironment),																																						
	  m_movementData(movementData)
{
}

MovingEntity::~MovingEntity(void)
{
}

//--------------------------------------------------------------------------------------
// Initialises the moving entity.
// Returns true if the moving entity was initialised successfully, false otherwise.
//--------------------------------------------------------------------------------------
bool MovingEntity::Initialise(void)
{
	return m_movementManager.Initialise(this);
}

//--------------------------------------------------------------------------------------
// Updates the moving entity.
//--------------------------------------------------------------------------------------
void MovingEntity::Update(void)
{
	m_movementManager.Update();
}

// Data access functions

const XMFLOAT2& MovingEntity::GetVelocity(void) const
{
	return m_movementData.m_velocity;
}

float MovingEntity::GetMaxVelocity(void) const
{
	return m_movementData.m_maxVelocity;
}

float MovingEntity::GetMaxForce(void) const
{
	return m_movementData.m_maxForce;
}

float MovingEntity::GetMaxSeeAhead(void) const
{
	return m_movementData.m_maxSeeAhead;
}

void MovingEntity::SetVelocity(const XMFLOAT2& velocity)
{
	m_movementData.m_velocity = velocity;
}

void MovingEntity::SetMaxVelocity(float maxVelocity)
{
	m_movementData.m_maxVelocity = maxVelocity;
}

void MovingEntity::SetMaxForce(float maxForce)
{
	m_movementData.m_maxForce = maxForce;
}

void MovingEntity::SetMaxSeeAhead(float maxSeeAhead)
{
	m_movementData.m_maxSeeAhead = maxSeeAhead;
}