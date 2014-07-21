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
	void Reset(void);

	void ShootAt(const XMFLOAT2& target);
	void Hit(float damage, const XMFLOAT2& direction);

	// Data access functions

	float GetCurrentHealth(void) const;
	float GetMaximalHealth(void) const;
	float IsAlive(void) const;
	void SetCurrentHealth(float health);

private:
	FightingEntity* m_pEntity; // The entity that this combat manager is associated to

	bool  m_isAlive;
	float m_currentHealth;
	float m_maximalHealth;
};

#endif // ENTITY_COMBAT_MANAGER_H