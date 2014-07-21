/* 
*  Kevin Meergans, SquadAI, 2014
*  CombatManager.cpp
*  Each entity able to fight has a combat manager associated to it that
*  is responsible for controlling the attack and defense actions of the entity.
*/

// Includes
#include "EntityCombatManager.h"
#include "FightingEntity.h"
#include "TestEnvironment.h"

EntityCombatManager::EntityCombatManager(void)
{
}

EntityCombatManager::~EntityCombatManager(void)
{
}

//--------------------------------------------------------------------------------------
// Initialises the combat manager.
// Param1: A pointer to the entity that the combat manager are associated to.
// Param2: The initialisation data required to initialise the combat component of the entity.
// Returns true if the combat component was initialised successfully, false otherwise.
//--------------------------------------------------------------------------------------
bool EntityCombatManager::Initialise(FightingEntity* pEntity, const EntityCombatInitData& initData)
{
	if(!pEntity)
	{
		return false;
	}

	m_pEntity = pEntity;
	m_maximalHealth = initData.m_maxHealth;
	m_currentHealth = m_maximalHealth;
	m_isAlive = (m_currentHealth > 0.0f);

	return true;
}

//--------------------------------------------------------------------------------------
// Resets the combat component of the entity.
//--------------------------------------------------------------------------------------
void EntityCombatManager::Reset(void)
{
	m_currentHealth = m_maximalHealth;
	m_isAlive = (m_currentHealth > 0.0f);
}

//--------------------------------------------------------------------------------------
// Fires a projectile towards a specified target position.
// Param1: The target position to shoot at.
//--------------------------------------------------------------------------------------
void EntityCombatManager::ShootAt(const XMFLOAT2& target)
{
	m_pEntity->GetTestEnvironment()->AddProjectile(m_pEntity->GetType(), m_pEntity->GetPosition(), target);
}

//--------------------------------------------------------------------------------------
// Called when the entity is hit by a hostile projectile.
// Param1: The target position to shoot at.
// Param2: The velocity of the projectile that hit the entity. Used to determine the position of the threat.
//--------------------------------------------------------------------------------------
void EntityCombatManager::Hit(float damage, const XMFLOAT2& direction)
{
	// add new suspected threat somewhere -> event queue?

	m_currentHealth -= damage;

	if(m_currentHealth <= 0.0f)
	{
		m_isAlive = false;
	}
}

// Data access functions

float EntityCombatManager::GetCurrentHealth(void) const
{
	return m_currentHealth;
}

float EntityCombatManager::GetMaximalHealth(void) const
{
	return m_maximalHealth;
}

float EntityCombatManager::IsAlive(void) const
{
	return m_currentHealth > 0.0f;
}

void EntityCombatManager::SetCurrentHealth(float health)
{
	m_currentHealth = health;
}