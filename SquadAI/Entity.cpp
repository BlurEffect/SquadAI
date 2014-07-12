/* 
*  Kevin Meergans, SquadAI, 2014
*  Entity.cpp
*  This is the abstract base class for all objects that can be present
*  within a test environment.
*/

// Includes
#include "Entity.h"
#include "TestEnvironment.h"

Entity::Entity(void) : m_id(0),
					   m_type(EntityType(0)),
					   m_position(0.0f, 0.0f),
					   m_rotation(0.0f),
					   m_pEnvironment(nullptr)
{
}

Entity::Entity(unsigned long id, EntityType type, const XMFLOAT2& position, float rotation, TestEnvironment* pEnvironment) : m_id(id), 
																															 m_type(type),
																															 m_position(position),
																															 m_rotation(rotation),
																															 m_pEnvironment(pEnvironment)
{
}

Entity::~Entity(void)
{
}

// Data access functions

unsigned long Entity::GetId(void) const
{
	return m_id;
}

const XMFLOAT2& Entity::GetPosition(void) const
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

TestEnvironment* Entity::GetTestEnvironment(void)
{
	return m_pEnvironment;
}

void Entity::SetId(unsigned long id)
{
	m_id = id;
}

void Entity::SetPosition(const XMFLOAT2& position)
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

void Entity::SetTestEnvironment(TestEnvironment* pEnvironment)
{
	m_pEnvironment = pEnvironment;
}