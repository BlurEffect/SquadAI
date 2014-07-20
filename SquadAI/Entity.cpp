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
					   m_pCollider(nullptr),
					   m_pEnvironment(nullptr)
{
}

Entity::~Entity(void)
{
	if(m_pCollider)
	{
		delete m_pCollider;
		m_pCollider = nullptr;
	}
}

//--------------------------------------------------------------------------------------
// Initialises the entity.
// Param1: The initialisation data to be used for initialisation of the entity.
// Returns true if the entity was initialised successfully, false otherwise.
//--------------------------------------------------------------------------------------
bool Entity::Initialise(const EntityInitData& initData)
{
	if(!initData.m_pEnvironment || !initData.m_pCollider)
	{
		return false;
	}

	m_id		   = initData.m_id;
	m_type		   = initData.m_type;
	m_position	   = initData.m_position;
	m_rotation	   = initData.m_rotation;
	m_scale		   = initData.m_scale;
	m_pEnvironment = initData.m_pEnvironment;

	return CreateCollider(initData.m_colliderType, initData.m_pCollider);
}

//--------------------------------------------------------------------------------------
// Creates the collider for the entity.
// Param1: The type of collider that should be created for this entity.
// Param2: A pointer to the collider that should be used to initialise the entity's one.
// Returns true if the collider was successfully created, false otherwise.
//--------------------------------------------------------------------------------------
bool Entity::CreateCollider(ColliderType type, Collider* pCollider)
{
	if(m_pCollider)
	{
		delete m_pCollider;
		m_pCollider = nullptr;
	}

	switch(type)
	{
	case CircleColliderType:
		m_pCollider = new CircleCollider(*reinterpret_cast<CircleCollider*>(pCollider));
		break;
	case AxisAlignedRectangleColliderType:
		m_pCollider = new AxisAlignedRectangleCollider(*reinterpret_cast<AxisAlignedRectangleCollider*>(pCollider));
		break;
	}

	return (m_pCollider) ? true : false;
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
// Activates the entity to start simulation.
//--------------------------------------------------------------------------------------
void Entity::Activate(void)
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
	
const Collider* Entity::GetCollider(void) const
{
	return m_pCollider;
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

void Entity::SetTestEnvironment(TestEnvironment* pEnvironment)
{
	m_pEnvironment = pEnvironment;
}