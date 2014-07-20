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

	return true;
}

//--------------------------------------------------------------------------------------
// Fires a projectile towards a specified target position.
// Param1: The target position to shoot at.
//--------------------------------------------------------------------------------------
void EntityCombatManager::ShootAt(const XMFLOAT2& target)
{
	m_pEntity->GetTestEnvironment()->AddProjectile(m_pEntity->GetPosition(), target);
}