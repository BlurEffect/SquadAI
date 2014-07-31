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
class Entity;
class TestEnvironment;

using namespace DirectX;

class EntityMovementManager
{
public:
	EntityMovementManager(void);
	~EntityMovementManager(void);

	bool Initialise(Entity* pEntity, TestEnvironment* pTestEnvironment);
	void UpdatePosition(float deltaTime, float maxSpeed, float maxForce);
	void Reset(void);

	bool SetPathTo(const XMFLOAT2& targetPosition);
	bool Seek(const XMFLOAT2& targetPosition, float targetReachedRadius, float speed);
	bool FollowPath(float nodeReachedRadius, float speed);
	void LookAt(const XMFLOAT2& targetPosition);
	void Wait();
	
	void AvoidCollisions(float seeAhead, float maximalForce);
	void StayAwayFromWalls(float avoidWallsRadius, float maximalForce);
	void Separate(float separationRadius, float maximalForce);

	bool IsPathSet(void) const;

	// Data access functions
	
	const XMFLOAT2& GetVelocity(void) const;
	const XMFLOAT2& GetViewDirection(void) const;

private:

	void SetInitialViewDirection(void);

	Entity*          m_pEntity;      // The entity that this movement manager is associated to
	TestEnvironment* m_pEnvironment; // The test environment that the entity is part of

	XMFLOAT2 m_velocity;      // The initial velocity of the entity
	XMFLOAT2 m_viewDirection; // The current view direction of the entity.
	XMFLOAT2 m_steeringForce; // The accumulated force that will be applied to the entity
	
	// For follow path behaviour
	std::vector<XMFLOAT2> m_path;        // The current path of the entity, empty when no destination for pathfinding
	unsigned int          m_currentNode; // The current target node within the path vector
	
	// For seek behaviour
	XMFLOAT2              m_seekTarget;	// The current target to seek
};

#endif // ENTITY_MOVEMENT_MANAGER_H