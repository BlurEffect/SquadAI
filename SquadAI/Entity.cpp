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
					   m_scale(1.0f),
					   m_radius(0.0f),
					   m_pEnvironment(nullptr)
{
}

Entity::~Entity(void)
{
}

//--------------------------------------------------------------------------------------
// Initialises the entity.
// Param1: The initialisation data to be used for initialisation of the entity.
// Returns true if the entity was initialised successfully, false otherwise.
//--------------------------------------------------------------------------------------
bool Entity::Initialise(const EntityInitData& initData)
{
	if(!initData.m_pEnvironment)
	{
		return false;
	}

	m_id		   = initData.m_id;
	m_type		   = initData.m_type;
	m_position	   = initData.m_position;
	m_rotation	   = initData.m_rotation;
	m_scale		   = initData.m_scale;
	m_radius	   = initData.m_radius;
	m_pEnvironment = initData.m_pEnvironment;

	return true;
}

//--------------------------------------------------------------------------------------
// Updates the entity.
// Param1: The time in seconds passed since the last frame.
//--------------------------------------------------------------------------------------
void Entity::Update(float deltaTime)
{
	// Do nothing
}
	
//--------------------------------------------------------------------------------------
// Resets the entity
//--------------------------------------------------------------------------------------
void Entity::Reset(void)
{
	// Do nothing
}

// Data access functions

unsigned long Entity::GetId(void) const
{
	return m_id;
}

EntityType Entity::GetType(void) const
{
	return m_type;
}

const XMFLOAT2& Entity::GetPosition(void) const
{
	return m_position;
}

float Entity::GetRotation(void) const
{
	return m_rotation;
}

float Entity::GetScale(void) const
{
	return m_scale;
}
	
float Entity::GetRadius(void) const
{
	return m_radius;
}

TestEnvironment* Entity::GetTestEnvironment(void)
{
	return m_pEnvironment;
}

void Entity::SetId(unsigned long id)
{
	m_id = id;
}

void Entity::SetType(EntityType type)
{
	m_type = type;
}

void Entity::SetPosition(const XMFLOAT2& position)
{
	m_position = position;
}

void Entity::SetRotation(float rotation)
{
	m_rotation = rotation;
}

void Entity::SetScale(float scale)
{
	m_scale = scale;
}

void Entity::SetRadius(float radius)
{
	m_radius = radius;
}

void Entity::SetTestEnvironment(TestEnvironment* pEnvironment)
{
	m_pEnvironment = pEnvironment;
}