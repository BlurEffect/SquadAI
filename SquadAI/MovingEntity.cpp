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

MovingEntity::MovingEntity(unsigned long id, EntityType type, const XMFLOAT2& position, float rotation) : Entity(id, type, position, rotation)
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

void MovingEntity::SetVelocity(const XMFLOAT2& velocity)
{
	m_velocity = velocity;
}