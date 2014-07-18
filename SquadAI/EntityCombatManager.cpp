/* 
*  Kevin Meergans, SquadAI, 2014
*  CombatManager.cpp
*  Each entity able to fight has a combat manager associated to it that
*  is responsible for controlling the attack and defense actions of the entity.
*/

// Includes
#include "EntityCombatManager.h"
#include "FightingEntity.h"

EntityCombatManager::EntityCombatManager(void)
{
}

EntityCombatManager::~EntityCombatManager(void)
{
}

bool EntityCombatManager::Initialise(FightingEntity* pEntity)
{
	return true;
}