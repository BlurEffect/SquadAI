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
#include <queue>
#include "CollidableObject.h"
#include "ObjectTypes.h"
#include "Behaviour.h"
#include "BehaviourFactory.h"
#include "Communicator.h"

// Forward declarations
class TestEnvironment;
class Message;

//--------------------------------------------------------------------------------------
// Bundles information associated to a known threat to an entity.
//--------------------------------------------------------------------------------------
struct KnownThreat
{
	KnownThreat(Entity* pEntity, bool hasHitEntity)
		: m_pEntity(pEntity),
		  m_hasHitEntity(hasHitEntity)
	{}

	Entity* m_pEntity;      // A pointer to the enemy entity
	bool    m_hasHitEntity; // Tells whether this known threat has successfully attacked the entity
};

//--------------------------------------------------------------------------------------
// Bundles information associated to a suspected threat to an entity.
//--------------------------------------------------------------------------------------
struct SuspectedThreat
{
	SuspectedThreat(unsigned long id, const XMFLOAT2& lastKnownPosition, bool hasHitEntity)
		: m_enemyId(id),
		  m_lastKnownPosition(lastKnownPosition),
		  m_hasHitEntity(hasHitEntity)
	{}

	unsigned long m_enemyId;			// The id of the hostile entity that has now become a suspected threat
	XMFLOAT2      m_lastKnownPosition;  // The position, where the enemy was last seen when it changed from a known to a suspected threat
	bool          m_hasHitEntity;       // Tells whether this suspected threat has successfully attacked the entity
};

using namespace DirectX;

class Entity : public CollidableObject, public Communicator
{
public:
	Entity(void);
	virtual ~Entity(void) = 0;

	bool         Initialise(unsigned long id, const XMFLOAT2& position, float rotation, float uniformScale, ObjectCategory category, ColliderType colliderType, void* pColliderData, TestEnvironment* pEnvironment, float maxHealth, EntityTeam team);
	//void         ProcessMessage(Message* pMessage);

	virtual void Update(float deltaTime);
	virtual void Activate(void);
	virtual void Reset(void);

	// Basic actions, to be implemented by concrete entities inheriting from this class
	virtual BehaviourStatus MoveToTarget(float deltaTime)						    = 0;
	virtual BehaviourStatus Attack(float deltaTime)									= 0;
	virtual BehaviourStatus AimAt(float deltaTime)								    = 0;
	virtual BehaviourStatus DetermineAttackTargetPosition(float deltaTime)          = 0;
	virtual BehaviourStatus Idle(float deltaTime)									= 0;
	virtual BehaviourStatus DeterminePatrolTarget(float deltaTime)			        = 0;
	virtual BehaviourStatus DetermineApproachThreatTarget(float deltaTime)          = 0;
	virtual BehaviourStatus UpdateThreats(float deltaTime)					        = 0;
	virtual BehaviourStatus DetermineGreatestKnownThreat(float deltaTime)           = 0;
	virtual BehaviourStatus DetermineGreatestSuspectedThreat(float deltaTime)       = 0;
	virtual BehaviourStatus UpdateAttackReadiness(float deltaTime)					= 0;
	virtual BehaviourStatus ResolveSuspectedThreat(float deltaTime)					= 0;
	
	// Threat management
	void AddKnownThreat(Entity* pThreat, bool hasHitEntity);
	void RemoveKnownThreat(unsigned long id);
	void ClearKnownThreats(void);
	bool IsKnownThreat(unsigned long id);
	const KnownThreat* GetKnownThreat(unsigned long id);
	void AddSuspectedThreat(unsigned long id, const XMFLOAT2& lastKnownPosition, bool hasHitEntity);
	void RemoveSuspectedThreat(unsigned long id);
	void ClearSuspectedThreats(void);
	bool IsSuspectedThreat(unsigned long id);
	const SuspectedThreat* GetSuspectedThreat(unsigned long id);
	bool IsInvestigatingGreatestSuspectedThreat(void);

	// Data access functions
	Behaviour*						    GetBehaviour(void);
	TestEnvironment*					GetTestEnvironment(void);
	EntityTeam							GetTeam(void) const;
	bool								IsAlive(void) const;
	std::vector<KnownThreat>&			GetKnownThreats(void);
	std::vector<SuspectedThreat>&		GetSuspectedThreats(void);
	const KnownThreat*					GetGreatestKnownThreat(void) const;
	const SuspectedThreat*				GetGreatestSuspectedThreat(void) const;
	bool								IsReadyForAttack(void) const;
	bool								IsAttackTargetSet(void) const;
	const XMFLOAT2&						GetAttackTarget(void) const;
	bool								IsMovementTargetSet(void) const;
	const XMFLOAT2&						GetMovementTarget(void) const;
	float								GetCurrentHealth(void) const;
	float								GetMaximalHealth(void) const;

	void SetTestEnvironment(TestEnvironment* pEnvironment);
	void SetTeam(EntityTeam team);
	void SetGreatestKnownThreat(KnownThreat* pThreat);
	void SetGreatestSuspectedThreat(SuspectedThreat* pThreat);
	void SetReadyForAttack(bool readyForAttack);
	void SetAttackTargetSet(bool targetSet);
	void SetAttackTarget(const XMFLOAT2& target);
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
	// Functor used to find a known threat within a container based on the id of the 
	// associated hostile entity.
	//--------------------------------------------------------------------------------------
	class FindKnownThreatById
	{
	public:
		FindKnownThreatById(unsigned long id) : m_id(id){}
		bool operator()(const KnownThreat& threat)
		{
			return threat.m_pEntity->GetId() == m_id;
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

	virtual void ProcessEvent(EventType type, void* pEventData);

protected:

	
	virtual void ProcessMessage(Message* pMessage);

	// Called to process events
	virtual void ProcessHit(float damage, unsigned long id, bool shooterAlive, const XMFLOAT2& position);
	virtual void ProcessEnemyKilled(EntityTeam team, unsigned long id);
	virtual void ProcessRespawn(const XMFLOAT2& respawnPosition);

private:

	Behaviour*       m_pBehaviour;   // The behaviour tree controlling the behaviour of this entity
	TestEnvironment* m_pEnvironment; // The test environment that the entity is part of
	EntityTeam       m_team;		 // The team the entity belongs to

	// This is pretty much used like a blackboard to communicate between different
	// nodes of the behaviour tree
	std::vector<KnownThreat>	 m_knownThreats;			 // Known Threats (enemies, whose position is definitely known)
	std::vector<SuspectedThreat> m_suspectedThreats;		 // Suspected Threats (positions, where enemies are expected and the id of the enemy that is expected to be there)
	KnownThreat*                 m_pGreatestKnownThreat;     // The greatest (most dangerous) known threat to the entity at the moment
	SuspectedThreat*             m_pGreatestSuspectedThreat; // The greatest (most dangerous) suspected threat to the entity at the moment
	bool						 m_readyForAttack;			 // Tells whether the entity is ready for attack
	bool						 m_movementTargetSet;		 // Tells whether there is a movement target set or not
	XMFLOAT2			         m_movementTarget;			 // The position to move to
	bool						 m_attackTargetSet;			 // Tells whether an attack target was set
	XMFLOAT2                     m_attackTarget;			 // The position to attack
	float						 m_currentHealth;			 // The current health state of the entity (percentage between 0.0 and 1.0 in relation to maximal health)
	float                        m_maximalHealth;			 // The maximal amount of health for this entity

};

#endif // ENTITY_H
