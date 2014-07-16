/* 
*  Kevin Meergans, SquadAI, 2014
*  MovementManager.h
*  Each entity able to move has a movement manager associated to it that
*  is responsible for controlling the position and velocity of the entity.
*/

#ifndef MOVEMENT_MANAGER_H
#define MOVEMENT_MANAGER_H

// Includes
#include <DirectXMath.h>
#include <vector>
#include "Pathfinder.h"
#include "ApplicationSettings.h"


class MovingEntity;

using namespace DirectX;

class MovementManager
{
public:
	MovementManager(void);
	~MovementManager(void);

	bool Initialise(MovingEntity* pEntity);
	void Update(float deltaTime);

	bool SetPathTo(const XMFLOAT2& targetPosition);

private:
	void FollowPath(float nodeReachedRadius);
	void Seek(const XMFLOAT2& targetPosition, float slowArrivalRadius);
	void AvoidCollisions();
	void Separate(float separationRadius);

	MovingEntity* m_pEntity;     // The entity that this movement manager is associated to

	XMFLOAT2              m_steeringForce; // The accumulated force that will be applied to the entity
	
	// For follow path behaviour
	std::vector<XMFLOAT2> m_path;        // The current path of the entity, empty when no destination for pathfinding
	unsigned int          m_currentNode; // The current target node within the path vector
	float                 m_reachedNodeRadius; // The radius that determines when a target node counts as reached
	// For seek behaviour
	XMFLOAT2              m_seekTarget;	// The current target to seek
	float				  m_slowArrivalRadius; // The entity will slow down when in this distance from the target
};

#endif // MOVEMENT_MANAGER_H