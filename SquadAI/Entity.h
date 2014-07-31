/* 
*  Kevin Meergans, SquadAI, 2014
*  Entity.h
*  This is the abstract base class for AI controlled entities that are able
*  to move and fight.
*/

#ifndef ENTITY_H
#define ENTITY_H

// Includes
#include <DirectXMath.h>
#include <vector>
#include "CollidableObject.h"
#include "ObjectTypes.h"
#include "Behaviour.h"

// Forward declarations
class TestEnvironment;

//--------------------------------------------------------------------------------------
// Bundles information associated to a suspected threat to an entity.
//--------------------------------------------------------------------------------------
struct SuspectedThreat
{
	SuspectedThreat(unsigned long id, const XMFLOAT2& lastKnownPosition, const XMFLOAT2& lastKnownVelocity)
		: m_enemyId(id),
		  m_lastKnownPosition(lastKnownPosition),
		  m_lastKnownVelocity(lastKnownVelocity)
	{}

	unsigned long m_enemyId;			// The id of the hostile entity that has now become a suspected threat
	XMFLOAT2      m_lastKnownPosition;  // The position, where the enemy was last seen when it changed from a known to a suspected threat
	XMFLOAT2      m_lastKnownVelocity;  // The direction into which the enemy was moving when it changed from a known to a suspected threat
};

using namespace DirectX;

class Entity : public CollidableObject
{
public:
	Entity(void);
	virtual ~Entity(void) = 0;

	bool         Initialise(unsigned long id, const XMFLOAT2& position, float rotation, float uniformScale, ObjectCategory category, ColliderType colliderType, void* pColliderData, TestEnvironment* pEnvironment, float maxHealth, EntityTeam team);
	virtual void Update(float deltaTime) = 0;
	virtual void Activate(void) = 0;
	virtual void Reset(void);

	// Basic actions
	virtual BehaviourStatus MoveToTarget(float deltaTime)						    = 0;
	virtual BehaviourStatus Attack(float deltaTime, const XMFLOAT2& targetPosition) = 0;
	virtual BehaviourStatus AimAt(float deltaTime, const XMFLOAT2& aimAtPosition)   = 0;
	virtual BehaviourStatus Idle(float deltaTime)									= 0;
	virtual BehaviourStatus DeterminePatrolTarget(float deltaTime)			        = 0;
	virtual BehaviourStatus DetermineApproachThreatTarget(float deltaTime)          = 0;
	virtual BehaviourStatus UpdateThreats(float deltaTime)					        = 0;
	virtual BehaviourStatus DetermineGreatestThreat(float deltaTime)                = 0;
	virtual BehaviourStatus UpdateAttackReadiness(float deltaTime)					= 0;

	// Threat management
	void AddKnownThreat(Entity* pThreat);
	void RemoveKnownThreat(unsigned long id);
	bool IsKnownThreat(unsigned long id);
	void AddSuspectedThreat(unsigned long id, const XMFLOAT2& lastKnownPosition, const XMFLOAT2& lastKnownVelocity);
	void RemoveSuspectedThreat(unsigned long id);
	bool IsSuspectedThreat(unsigned long id);

	// Data access functions
	TestEnvironment*					GetTestEnvironment(void);
	EntityTeam							GetTeam(void) const;
	bool								IsAlive(void) const;
	const std::vector<Entity*>&			GetKnownThreats(void) const;
	const std::vector<SuspectedThreat>& GetSuspectedThreats(void) const;
	const Entity*						GetGreatestKnownThreat(void) const;
	const SuspectedThreat*				GetGreatestSuspectedThreat(void) const;
	bool								IsReadyForAttack(void) const;
	bool								IsMovementTargetSet(void) const;
	const XMFLOAT2&						GetMovementTarget(void) const;
	float								GetCurrentHealth(void) const;
	float								GetMaximalHealth(void) const;

	void SetTestEnvironment(TestEnvironment* pEnvironment);
	void SetTeam(EntityTeam team);
	void SetGreatestKnownThreat(Entity* pThreat);
	void SetGreatestSuspectedThreat(SuspectedThreat* pThreat);
	void SetReadyForAttack(bool readyForAttack);
	void SetMovementTargetSet(bool targetSet);
	void SetMovementTarget(const XMFLOAT2& target);
	void SetCurrentHealth(float health);
	void SetMaximalHealth(float maxHealth);

	//--------------------------------------------------------------------------------------
	// Functor used to find an entity within a container based on its id.
	//--------------------------------------------------------------------------------------
	class FindEntityById
	{
	public:
		FindEntityById(unsigned long id) : m_id(id){}
		bool operator()(const Entity* pEntity)
		{
			return pEntity->GetId() == m_id;
		}
	private:
		unsigned long m_id;
	};

	//--------------------------------------------------------------------------------------
	// Functor used to find suspected threat within a container based on its id.
	//--------------------------------------------------------------------------------------
	class FindSuspectedThreatById
	{
	public:
		FindSuspectedThreatById(unsigned long id) : m_id(id){}
		bool operator()(const SuspectedThreat& threat)
		{
			return threat.m_enemyId == m_id;
		}
	private:
		unsigned long m_id;
	};

private:
	TestEnvironment* m_pEnvironment; // The test environment that the entity is part of
	EntityTeam       m_team;		 // The team the entity belongs to

	std::vector<Entity*>		 m_knownThreats;			 // Known Threats (enemies, whose position is definitely known)
	std::vector<SuspectedThreat> m_suspectedThreats;		 // Suspected Threats (positions, where enemies are expected and the id of the enemy that is expected to be there)
	Entity*                      m_pGreatestKnownThreat;     // The greatest (most dangerous) known threat to the entity at the moment
	SuspectedThreat*             m_pGreatestSuspectedThreat; // The greatest (most dangerous) suspected threat to the entity at the moment
	bool						 m_readyForAttack;			 // Tells whether the entity is ready for attack
	bool						 m_movementTargetSet;		 // Tells whether there is a movement target set or not
	XMFLOAT2			         m_movementTarget;			 // The position to move to
	float						 m_currentHealth;			 // The current health state of the entity (percentage between 0.0 and 1.0 in relation to maximal health)
	float                        m_maximalHealth;			 // The maximal amount of health for this entity


};

#endif // ENTITY_H
