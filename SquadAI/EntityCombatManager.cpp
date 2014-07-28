/* 
*  Kevin Meergans, SquadAI, 2014
*  CombatManager.cpp
*  Each entity able to fight has a combat manager associated to it that
*  is responsible for controlling the attack and defense actions of the entity.
*/

// Includes
#include "EntityCombatManager.h"
#include "Entity.h"
#include "TestEnvironment.h"

EntityCombatManager::EntityCombatManager(void) : m_pEntity(nullptr),
												 m_pEnvironment(nullptr)
{
}

EntityCombatManager::~EntityCombatManager(void)
{
}

//--------------------------------------------------------------------------------------
// Initialises the combat manager.
// Param1: A pointer to the entity that the combat manager is associated to.
// Param2: A pointer to the test environment that the entity belongs to.
// Returns true if the combat component was initialised successfully, false otherwise.
//--------------------------------------------------------------------------------------
bool EntityCombatManager::Initialise(Entity* pEntity, TestEnvironment* pTestEnvironment)
{
	if(!pEntity || !pTestEnvironment)
	{
		return false;
	}

	m_pEntity	   = pEntity;
	m_pEnvironment = pTestEnvironment;

	return true;
}

//--------------------------------------------------------------------------------------
// Resets the combat component of the entity.
//--------------------------------------------------------------------------------------
void EntityCombatManager::Reset(void)
{
	
}

//--------------------------------------------------------------------------------------
// Fires a projectile towards a specified target position.
// Param1: The target position to shoot at.
//--------------------------------------------------------------------------------------
void EntityCombatManager::ShootAt(const XMFLOAT2& target)
{
	m_pEnvironment->AddProjectile(m_pEntity->GetTeam(), m_pEntity->GetPosition(), target);
}

