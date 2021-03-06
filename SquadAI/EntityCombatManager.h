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
class Entity;
class TestEnvironment;
struct KnownThreat;
struct SuspectedThreat;

using namespace DirectX;

class EntityCombatManager
{
public:
	EntityCombatManager(void);
	~EntityCombatManager(void);

	bool Initialise(Entity* pEntity, TestEnvironment* pTestEnvironment);
	void Reset(void);

	KnownThreat* DetermineGreatestKnownThreat(void);
	SuspectedThreat* DetermineGreatestSuspectedThreat(void);

	void ShootAt(const XMFLOAT2& target);

private:
	Entity*          m_pEntity;      // The entity that this combat manager is associated to
	TestEnvironment* m_pEnvironment; // The test environment that the entity is part of
	
};

#endif // ENTITY_COMBAT_MANAGER_H