/* 
*  Kevin Meergans, SquadAI, 2014
*  MovingEntity.cpp
*  Base class for all moving entities in the test environment.
*/

// Includes
#include "MovingEntity.h"

MovingEntity::MovingEntity(void) : Entity(),
								   m_velocity(0.0f, 0.0f),
								   m_maxVelocity(0.0f),
								   m_maxForce(0.0f)
{
}

MovingEntity::MovingEntity(unsigned long id, EntityType type, const XMFLOAT2& position, float rotation, TestEnvironment* pEnvironment, float maxVelocity, float maxForce) 
	: Entity(id, type, position, rotation, pEnvironment),																																						
	  m_velocity(0.0f, 0.0f),																																			
	  m_maxVelocity(maxVelocity),
	  m_maxForce(maxForce)
{
}

MovingEntity::~MovingEntity(void)
{
}

// Data access functions

const XMFLOAT2& MovingEntity::GetVelocity(void) const
{
	return m_velocity;
}

float MovingEntity::GetMaxVelocity(void) const
{
	return m_maxVelocity;
}

float MovingEntity::GetMaxForce(void) const
{
	return m_maxForce;
}

void MovingEntity::SetVelocity(const XMFLOAT2& velocity)
{
	m_velocity = velocity;
}

void MovingEntity::SetMaxVelocity(float maxVelocity)
{
	m_maxVelocity = maxVelocity;
}

void MovingEntity::SetMaxForce(float maxForce)
{
	m_maxForce = maxForce;
}