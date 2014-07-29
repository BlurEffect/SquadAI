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

							  // add combat stuff here
							  m_fieldOfView(0.0f),
							  m_viewingDistance(0.0f),
							  m_maxHealth(0.0f)
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

	// Sensors
	float m_fieldOfView;				// The angle determining the field of view in radians
	float m_viewingDistance;			// The entity can look this far ahead

	// Other
	float m_maxHealth;                  // The maximal health of the soldier
};

class Soldier : public Entity
{
public:
	Soldier(void);
	~Soldier(void);

	bool Initialise(unsigned long id, const XMFLOAT2& position, float rotation, float uniformScale, ObjectCategory category, ColliderType colliderType, void* pColliderData, EntityTeam team, const SoldierProperties& soldierProperties, TestEnvironment* pEnvironment);
	void Update(float deltaTime);
	void Activate(void);
	void Reset(void);

	void Hit(float damage, const XMFLOAT2& direction);

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
	const XMFLOAT2& GetViewDirection(void) const;
	float			GetFieldOfView(void) const;
	float			GetViewingDistance(void) const;
	float			GetCurrentHealth(void) const;
	float			GetMaximalHealth(void) const;
	float			IsAlive(void) const;

	void SetMaxSpeed(float speed);
	void SetMaxTotalForce(float maxForce);
	void SetMaxCollisionSeeAhead(float maxSeeAhead);
	void SetMaxCollisionAvoidanceForce(float maxCollisionAvoidanceForce);
	void SetMaxAvoidWallsForce(float maxAvoidWallsForce);
	void SetMaxSeparationForce(float maxSeparationForce);
	void SetTargetReachedRadius(float targetReachedRadius);
	void SetAvoidWallsRadius(float slowArrivalRadius);
	void SetSeparationRadius(float separationRadius);
	void SetFieldOfView(float fieldOfView);
	void SetViewingDistance(float viewingDistance);
	void SetCurrentHealth(float health);

private:

	TestEnvironment*      m_pEnvironment;      // A pointer to the test environment that the soldier is part of

	// Soldier components
	EntityMovementManager m_movementManager;   // The movement manager used by this soldier
	EntityCombatManager   m_combatManager;	   // The combat manager used by this soldier
	EntitySensors         m_sensors;		   // The sensor component used by this soldier

	SoldierProperties     m_soldierProperties; // Determines some properties of the soldier related to movement, combat and sensors

	// Soldier state
	bool				  m_isAlive;           // Tells whether the soldier is currently alive or dead
	float				  m_currentHealth;	   // The current health of the soldier

};

#endif // SOLDIER_H