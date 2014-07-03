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

// Data access functions

const XMFLOAT3& Entity::GetPosition(void) const
{
	return m_position;
}

float Entity::GetRotation(void) const
{
	return m_rotation;
}

EntityType Entity::GetType(void) const
{
	return m_type;
}

void Entity::SetPosition(const XMFLOAT3& position)
{
	m_position = position;
}

void Entity::SetRotation(float rotation)
{
	m_rotation = rotation;
}

void Entity::SetType(EntityType type)
{
	m_type = type;
}