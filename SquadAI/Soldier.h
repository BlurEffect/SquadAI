/* 
*  Kevin Meergans, SquadAI, 2014
*  Soldier.h
*  Represents a fighting entity in the test environment.
*/

#ifndef SOLDIER_H
#define SOLDIER_H

// Includes
#include <DirectXMath.h>
#include "Entity.h"
#include "EntityMovementManager.h"
#include "EntityCombatManager.h"
#include "EntitySensors.h"

using namespace DirectX;

//--------------------------------------------------------------------------------------
// Bundles the data required to initialise a Soldier entity.
//--------------------------------------------------------------------------------------
struct SoldierProperties
{
	SoldierProperties(void) : m_maxSpeed(0.0f),
							  m_maxTotalForce(0.0f),
							  m_maxCollisionSeeAhead(0.0f),
							  m_maxCollisionAvoidanceForce(0.0f),
							  m_maxAvoidWallsForce(0.0f),
							  m_maxSeparationForce(0.0f),       
							  m_targetReachedRadius(0.0f),		
							  m_avoidWallsRadius(0.0f),		 
							  m_separationRadius(0.0f),
							  m_fireWeaponInterval(0.0f),
							  m_fieldOfView(0.0f),
							  m_viewingDistance(0.0f),
							  m_maxHealth(0.0f),
							  m_lookAroundInterval(0.0f)
	{}

	// Movement
	float m_maxSpeed;				    // The maximal speed allowed (limits the length of the velocity vector)
	float m_maxTotalForce;				// The maximal force that can impact this soldier's movement
	float m_maxCollisionSeeAhead;       // The distance that the soldier can see ahead and check for collisions with other objects
	float m_maxCollisionAvoidanceForce; // The maximal force that can result from avoiding collisions
	float m_maxAvoidWallsForce;         // The maximal force that can result from avoiding walls (being pushed away from them)
	float m_maxSeparationForce;         // The maximal force that can result from separation from other entities
	float m_targetReachedRadius;		// When the distance between a soldier and his target is lower than this, latter one counts as reached
	float m_avoidWallsRadius;		    // When this close to an obstacle, the soldier will get pushed away from it (prevents intersection with walls)
	float m_separationRadius;		    // When a soldier registers other entities within this radius it will steer for separation from them
	
	// Combat
	float m_fireWeaponInterval;			// After every shot, the soldier has to wait this many seconds before he can fire his weapon again

	// Sensors
	float m_fieldOfView;				// The angle determining the field of view in radians
	float m_viewingDistance;			// The entity can look this far ahead

	// Other
	float m_maxHealth;                  // The maximal health of the soldier
	float m_lookAroundInterval;         // Determines how often the entity changes its look at position when holding a spot
};

class Soldier : public Entity
{
public:
	Soldier(void);
	~Soldier(void);

	bool Initialise(unsigned long id, const XMFLOAT2& position, float rotation, float uniformScale, ObjectCategory category, ColliderType colliderType, void* pColliderData, TestEnvironment* pEnvironment, EntityTeam team, const SoldierProperties& soldierProperties);
	void Reset(void);

	// Basic actions as inherited from Entity
	BehaviourStatus MoveToTarget(float deltaTime);
	BehaviourStatus Attack(float deltaTime);
	BehaviourStatus AimAt(float deltaTime);
	BehaviourStatus DetermineAttackTargetPosition(float deltaTime);
	BehaviourStatus Idle(float deltaTime);
	BehaviourStatus DetermineMovementTarget(float deltaTime);
	BehaviourStatus DetermineApproachThreatTarget(float deltaTime);
	BehaviourStatus UpdateThreats(float deltaTime);
	BehaviourStatus DetermineGreatestKnownThreat(float deltaTime);
	BehaviourStatus DetermineGreatestSuspectedThreat(float deltaTime);
	BehaviourStatus UpdateAttackReadiness(float deltaTime);
	BehaviourStatus ResolveSuspectedThreat(float deltaTime);

	BehaviourStatus DeterminePathToTarget(float deltaTime);
	BehaviourStatus DetermineObservationTarget(float deltaTime);
	BehaviourStatus LookAtTarget(float deltaTime);

	bool IsMovingToHighestPriorityTarget(void);

	void ProcessEvent(EventType type, void* pEventData);

	bool IsAtTarget(const XMFLOAT2& target);

	// Data access functions
	 
	const XMFLOAT2& GetVelocity(void) const;
	float           GetMaxSpeed(void) const;
	float           GetMaxTotalForce(void) const;
	float           GetMaxCollisionSeeAhead(void) const;
	float           GetMaxCollisionAvoidanceForce(void) const;
	float           GetMaxAvoidWallsForce(void) const;
	float           GetMaxSeparationForce(void) const;
	float           GetTargetReachedRadius(void) const;
	float           GetAvoidWallsRadius(void) const;
	float           GetSeparationRadius(void) const;
	float           GetFireWeaponInterval(void) const;
	const XMFLOAT2& GetViewDirection(void) const;
	float			GetFieldOfView(void) const;
	float			GetViewingDistance(void) const;

	void SetMaxSpeed(float speed);
	void SetMaxTotalForce(float maxForce);
	void SetMaxCollisionSeeAhead(float maxSeeAhead);
	void SetMaxCollisionAvoidanceForce(float maxCollisionAvoidanceForce);
	void SetMaxAvoidWallsForce(float maxAvoidWallsForce);
	void SetMaxSeparationForce(float maxSeparationForce);
	void SetTargetReachedRadius(float targetReachedRadius);
	void SetAvoidWallsRadius(float slowArrivalRadius);
	void SetSeparationRadius(float separationRadius);
	void SetFireWeaponInterval(float fireWeaponInterval);
	void SetFieldOfView(float fieldOfView);
	void SetViewingDistance(float viewingDistance);

	//--------------------------------------------------------------------------------------
	// Functor that can be used to find a soldier with a specific ID.
	//--------------------------------------------------------------------------------------
	class FindSoldierById
	{
	public:
		FindSoldierById(unsigned long id) : m_id(id){}
		bool operator()(const Soldier& pSoldier)
		{
			return pSoldier.GetId() == m_id;
		}
	private:
		unsigned long m_id;
	};

protected:
	void ProcessMessage(Message* pMessage);

private:

	// Soldier components
	EntityMovementManager m_movementManager;   // The movement manager used by this soldier
	EntityCombatManager   m_combatManager;	   // The combat manager used by this soldier
	EntitySensors         m_sensors;		   // The sensor component used by this soldier

	SoldierProperties     m_soldierProperties; // Determines some properties of the soldier related to movement, combat and sensors

	float                 m_fireWeaponTimer;   // Used to determine when the soldier is ready to fire his weapon
	float                 m_changeObservationTargetTimer;  // Used to determine when the soldier should change his observation target

	XMFLOAT2              m_currentMovementTarget; // Used to check whether the soldier is still moving towards the correct position (that is highest priority movement target)
};

#endif // SOLDIER_H