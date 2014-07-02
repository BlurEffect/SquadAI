/* 
*  Kevin Meergans, SquadAI, 2014
*  Entity.cpp
*  This is the abstract base class for all objects that can be present
*  within a test environment.
*/

// Includes
#include "Entity.h"

Entity::Entity(void) : m_type(EntityType(0)),
					   m_position(0.0f, 0.0f, 0.0f),
					   m_rotation(0.0f)
{
}

Entity::Entity(EntityType type, const XMFLOAT3& position, float rotation) : m_type(type),
																			m_position(position),
																			m_rotation(rotation)
{
}

Entity::~Entity(void)
{
}