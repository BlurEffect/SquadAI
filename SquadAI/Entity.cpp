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
					   m_pBehaviour(nullptr),
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
	if(m_pBehaviour)
	{
		delete m_pBehaviour;
	}
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

	m_pBehaviour = BehaviourFactory::CreateBehaviourTree(SimpleMovementTree, this);
	if(!m_pBehaviour)
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

Behaviour* Entity::GetBehaviour(void)
{
	return m_pBehaviour;
}

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

const Entity* Entity::GetGreatestKnownThreat(void) const
{
	return m_pGreatestKnownThreat;
}

const SuspectedThreat* Entity::GetGreatestSuspectedThreat(void) const
{
	return m_pGreatestSuspectedThreat;
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

void Entity::SetGreatestKnownThreat(Entity* pThreat)
{
	m_pGreatestKnownThreat = pThreat;
}

void Entity::SetGreatestSuspectedThreat(SuspectedThreat* pThreat)
{
	m_pGreatestSuspectedThreat = pThreat;
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
