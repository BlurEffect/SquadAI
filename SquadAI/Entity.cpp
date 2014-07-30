/* 
*  Kevin Meergans, SquadAI, 2014
*  Entity.cpp
*  This is the abstract base class for AI controlled entities that are able
*  to move and fight.
*/
	
// Includes
#include "Entity.h"
#include "TestEnvironment.h"

Entity::Entity(void) : CollidableObject(),
					   m_pEnvironment(nullptr),
					   m_team(EntityTeam(None)),
					   m_readyForAttack(false),
					   m_movementTargetSet(false),
					   m_movementTarget(0.0f, 0.0f),
					   m_currentHealth(0.0f),
					   m_maximalHealth(0.0f)
{
}

Entity::~Entity(void)
{
}

//--------------------------------------------------------------------------------------
// Initialises the entity.
// Param1:  A unique identifier for the object.
// Param2:  The start position of the entity.
// Param3:  The initial rotation of the entity.
// Param4:  The uniform scale of the entity.
// Param5:  The category the object belongs to.
// Param6:  The type of the collider that should be created for this entity.
// Param7:  The collider data that should be used for the creation of the collider.
// Param8:  A pointer to the test environment that the entity is part of.
// Param9:  The maximal health for this entity.
// Param10: Identifies the team that this entity belongs to.
// Returns true if the entity was initialised successfully, false otherwise.
//--------------------------------------------------------------------------------------
bool Entity::Initialise(unsigned long id, const XMFLOAT2& position, float rotation, float uniformScale, ObjectCategory category, ColliderType colliderType, void* pColliderData, TestEnvironment* pEnvironment, float maxHealth, EntityTeam team)
{
	if(!CollidableObject::Initialise(id, position, rotation, uniformScale, category, colliderType, pColliderData))
	{
		return false;
	}

	// Check for invalid values
	if((team == None) || (!pEnvironment))
	{
		return false;
	}

	m_pEnvironment = pEnvironment;
	m_team = team;
	m_maximalHealth = maxHealth;
	m_currentHealth = m_maximalHealth;

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
// Activates the entity at the start of the simulation.
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
	m_knownThreats.clear();
	m_suspectedThreats.clear();
	m_readyForAttack = false;
	m_movementTargetSet = false;
	m_movementTarget = XMFLOAT2(0.0f, 0.0f);
	m_currentHealth = m_maximalHealth;
}


//--------------------------------------------------------------------------------------
// Adds a new threat to the entity's list of known threats.
// Param1: A pointer to the hostile entity that has become a known threat.
//--------------------------------------------------------------------------------------
void Entity::AddKnownThreat(Entity* pThreat)
{
	if(pThreat)
	{
		m_knownThreats.push_back(pThreat);
	}
}

//--------------------------------------------------------------------------------------
// Adds a new threat to the entity's list of known threats.
// Param1: The id of the entity that is no longer a known threat to this entity.
//--------------------------------------------------------------------------------------
void Entity::RemoveKnownThreat(unsigned long id)
{
	std::vector<Entity*>::iterator foundIt = std::find_if(m_knownThreats.begin(), m_knownThreats.end(), Entity::FindEntityById(id));
		
	if(foundIt != m_knownThreats.end())
	{
		m_knownThreats.erase(foundIt);
	}
}

//--------------------------------------------------------------------------------------
// Tells whether the entity associated to the passed in ID is a known threat to this entity.
// Param1: The id of the entity that should be checked whether it is a known threat.
// Returns true if the entity associated to the passed in ID is a known threat to this entity,
// false otherwise.
//--------------------------------------------------------------------------------------
bool Entity::IsKnownThreat(unsigned long id)
{
	std::vector<Entity*>::iterator foundIt = std::find_if(m_knownThreats.begin(), m_knownThreats.end(), Entity::FindEntityById(id));
		
	return (foundIt != m_knownThreats.end());
}

//--------------------------------------------------------------------------------------
// Adds a new threat to the entity's list of known threats.
// Param1: The id of the entity that is no longer a known threat to this entity.
//--------------------------------------------------------------------------------------
void Entity::AddSuspectedThreat(unsigned long id, const XMFLOAT2& lastKnownPosition, const XMFLOAT2& lastKnownVelocity)
{
	m_suspectedThreats.push_back(SuspectedThreat(id, lastKnownPosition, lastKnownVelocity));
}

//--------------------------------------------------------------------------------------
// Adds a new threat to the entity's list of known threats.
// Param1: The id of the entity that is no longer a known threat to this entity.
//--------------------------------------------------------------------------------------
void Entity::RemoveSuspectedThreat(unsigned long id)
{
	std::vector<SuspectedThreat>::iterator foundIt = std::find_if(m_suspectedThreats.begin(), m_suspectedThreats.end(), Entity::FindSuspectedThreatById(id));
		
	if(foundIt != m_suspectedThreats.end())
	{
		m_suspectedThreats.erase(foundIt);
	}
}

//--------------------------------------------------------------------------------------
// Tells whether the entity associated to the passed in ID is a suspected threat to this entity.
// Param1: The id of the entity that should be checked whether it is a suspected threat.
// Returns true if the entity associated to the passed in ID is a suspected threat to this entity,
// false otherwise.
//--------------------------------------------------------------------------------------
bool Entity::IsSuspectedThreat(unsigned long id)
{
	std::vector<SuspectedThreat>::iterator foundIt = std::find_if(m_suspectedThreats.begin(), m_suspectedThreats.end(), Entity::FindSuspectedThreatById(id));
		
	return (foundIt != m_suspectedThreats.end());
}

// Data access functions

TestEnvironment* Entity::GetTestEnvironment(void)
{
	return m_pEnvironment;
}

EntityTeam Entity::GetTeam(void) const
{
	return m_team;
}

bool Entity::IsAlive(void) const
{
	return m_currentHealth > 0.0f;
}

const std::vector<Entity*>& Entity::GetKnownThreats(void) const
{
	return m_knownThreats;
}

const std::vector<SuspectedThreat>& Entity::GetSuspectedThreats(void) const
{
	return m_suspectedThreats;
}

bool Entity::IsReadyForAttack(void) const
{
	return m_readyForAttack;
}

bool Entity::IsMovementTargetSet(void) const
{
	return m_movementTargetSet;
}

const XMFLOAT2& Entity::GetMovementTarget(void) const
{
	return m_movementTarget;
}

float Entity::GetCurrentHealth(void) const
{
	return m_currentHealth;
}

float Entity::GetMaximalHealth(void) const
{
	return m_maximalHealth;
}

void Entity::SetTestEnvironment(TestEnvironment* pEnvironment)
{
	if(pEnvironment)
	{
		m_pEnvironment = pEnvironment;
	}
}

void Entity::SetTeam(EntityTeam team)
{
	m_team = team;
}

void Entity::SetReadyForAttack(bool readyForAttack)
{
	m_readyForAttack = readyForAttack;
}

void Entity::SetMovementTargetSet(bool targetSet)
{
	m_movementTargetSet = targetSet;
}

void Entity::SetMovementTarget(const XMFLOAT2& target)
{
	m_movementTarget = target;
}

void Entity::SetCurrentHealth(float health)
{
	m_currentHealth = health;
}

void Entity::SetMaximalHealth(float maxHealth)
{
	m_maximalHealth = maxHealth;
}

/*
// Includes
#include "Entity.h"
#include "TestEnvironment.h"

Entity::Entity(void) : m_id(0),
				       m_team(EntityTeam(None)),
					   m_type(ObjectType(0)),
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
	m_team         = initData.m_team;
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

EntityTeam Entity::GetTeam(void) const
{
	return m_team;
}

ObjectType Entity::GetType(void) const
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
	
Collider* Entity::GetCollider(void)
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

void Entity::SetTeam(EntityTeam team)
{
	m_team = team;
}

void Entity::SetType(ObjectType type)
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
*/