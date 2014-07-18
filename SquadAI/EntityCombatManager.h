/* 
*  Kevin Meergans, SquadAI, 2014
*  EntityCombatManager.h
*  Each entity able to fight has a combat manager associated to it that
*  is responsible for controlling the attack and defense actions of the entity.
*/

#ifndef ENTITY_COMBAT_MANAGER_H
#define ENTITY_COMBAT_MANAGER_H

// Includes
#include <DirectXMath.h>

// Forward declarations
class FightingEntity;
struct EntityCombatInitData;

using namespace DirectX;

class EntityCombatManager
{
public:
	EntityCombatManager(void);
	~EntityCombatManager(void);

	bool Initialise(FightingEntity* pEntity, const EntityCombatInitData& initData);
private:

};

#endif // ENTITY_COMBAT_MANAGER_H