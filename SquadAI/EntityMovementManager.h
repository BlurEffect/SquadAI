/* 
*  Kevin Meergans, SquadAI, 2014
*  EntityMovementManager.h
*  Each entity able to move has a movement manager associated to it that
*  is responsible for controlling the position and velocity of the entity.
*/

#ifndef ENTITY_MOVEMENT_MANAGER_H
#define ENTITY_MOVEMENT_MANAGER_H

// Includes
#include <DirectXMath.h>
#include <vector>
#include <map>

// Forward declarations
class MovingEntity;
struct EntityMovementInitData;

using namespace DirectX;

class EntityMovementManager
{
public:
	EntityMovementManager(void);
	~EntityMovementManager(void);

	bool Initialise(MovingEntity* pEntity, const EntityMovementInitData& initData);
	void Update(float deltaTime);
	void Reset(void);

	bool SetPathTo(const XMFLOAT2& targetPosition);

	void LookAt(const XMFLOAT2& targetPosition);
	void Wait();
	void FollowPath(float nodeReachedRadius);
	void Seek(const XMFLOAT2& targetPosition, float targetReachedRadius);
	void AvoidObstacleCollisions(float seeAhead);
	void StayAwayFromWalls(float avoidWallsRadius);
	void Separate(float separationRadius);

	// Data access functions

	const XMFLOAT2& GetVelocity(void) const;
	float           GetMaxVelocity(void) const;
	float           GetMaxTotalForce(void) const;
	float           GetMaxSeeAhead(void) const;
	float           GetMaxCollisionAvoidanceForce(void) const;
	float           GetMaxAvoidWallsForce(void) const;
	float           GetMaxSeparationForce(void) const;
	float           GetTargetReachedRadius(void) const;
	float           GetAvoidWallsRadius(void) const;
	float           GetSeparationRadius(void) const;
	const XMFLOAT2& GetViewDirection(void) const;

	void SetVelocity(const XMFLOAT2& velocity);
	void SetMaxVelocity(float maxVelocity);
	void SetMaxTotalForce(float maxForce);
	void SetMaxSeeAhead(float maxSeeAhead);
	void SetMaxCollisionAvoidanceForce(float maxCollisionAvoidanceForce);
	void SetMaxAvoidWallsForce(float maxAvoidWallsForce);
	void SetMaxSeparationForce(float maxSeparationForce);
	void SetTargetReachedRadius(float targetReachedRadius);
	void SetAvoidWallsRadius(float slowArrivalRadius);
	void SetSeparationRadius(float separationRadius);

private:

	void SetInitialViewDirection(void);

	MovingEntity* m_pEntity; // The entity that this movement manager is associated to

	XMFLOAT2 m_velocity;                   // The initial velocity of the entity
	float    m_maxVelocity;		           // The maximal velocity allowed (limits the length of the velocity vector)
	float    m_maxTotalForce;	           // The maximal force that can impact this entity's movement
	float    m_maxSeeAhead;				   // The distance that the entity can see ahead and check for collisions with other entities
	float	 m_maxCollisionAvoidanceForce; // The maximal force that can result from avoiding collisions
	float    m_maxAvoidWallsForce;         // The maximal force that can result from avoiding walls (being pushed away from them)
	float	 m_maxSeparationForce;         // The maximal force that can result from separation from other entities
	float	 m_targetReachedRadius;		   // When the distance between an entity and its target is lower than this, latter one counts as reached
	float	 m_avoidWallsRadius;		   // When this close to an obstacle, the entity will be pushed away from it
	float	 m_separationRadius;		   // When an entity registers other entities within this radius it will steer for separation from them

	XMFLOAT2 m_viewDirection;			   // The current view direction of the entity.
	XMFLOAT2 m_steeringForce;			   // The accumulated force that will be applied to the entity
	
	// For follow path behaviour
	std::vector<XMFLOAT2> m_path;        // The current path of the entity, empty when no destination for pathfinding
	unsigned int          m_currentNode; // The current target node within the path vector
	
	// For seek behaviour
	XMFLOAT2              m_seekTarget;	// The current target to seek
};

#endif // ENTITY_MOVEMENT_MANAGER_H