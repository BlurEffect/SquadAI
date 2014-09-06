/* 
*  Kevin Meergans, SquadAI, 2014
*  EntityMovementManager.cpp
*  Each entity able to move has a movement manager associated to it that
*  is responsible for controlling the position and velocity of the entity.
*/

// Includes
#include "EntityMovementManager.h"
#include "Entity.h"
#include "Pathfinder.h"
#include "TestEnvironment.h"

EntityMovementManager::EntityMovementManager(void) : m_pEntity(nullptr),
												  	 m_pEnvironment(nullptr),
													 m_velocity(XMFLOAT2(0.0f, 0.0f)),
													 m_steeringForce(0.0f, 0.0f),
													 m_currentNode(0),
													 m_seekTarget(0.0f, 0.0f)
{
}

EntityMovementManager::~EntityMovementManager(void)
{
}

//--------------------------------------------------------------------------------------
// Initialises movement manager.
// Param1: A pointer to the entity that the movement manager is associated with.
// Param2: A pointer to the test environment that the entity is part of.
// Returns true if the movement manager could be initialised successfully, false otherwise.
//--------------------------------------------------------------------------------------
bool EntityMovementManager::Initialise(Entity* pEntity, TestEnvironment* pTestEnvironment)
{
	if(!pEntity || !pTestEnvironment)
	{
		return false;
	}

	m_pEntity      = pEntity;
	m_pEnvironment = pTestEnvironment;

	SetInitialViewDirection();

	return true;
}

//--------------------------------------------------------------------------------------
// Initialises the view direction variable based on the initial rotation of the entity.
//--------------------------------------------------------------------------------------
void EntityMovementManager::SetInitialViewDirection(void)
{
	// Only four possible start values
	if(m_pEntity->GetRotation() == 0.0f)
	{
		m_pEntity->SetViewDirection(XMFLOAT2(0.0f, 1.0f));
	}else if(m_pEntity->GetRotation() == 90.0f)
	{
		m_pEntity->SetViewDirection(XMFLOAT2(1.0f, 0.0f));
	}else if(m_pEntity->GetRotation() == 180.0f)
	{
		m_pEntity->SetViewDirection(XMFLOAT2(0.0f, -1.0f));
	}else
	{
		// 270 degrees rotation
		m_pEntity->SetViewDirection(XMFLOAT2(-1.0f, 0.0f));
	}
}

//--------------------------------------------------------------------------------------
// Updates the position and velocity of the entity associated to this movement manager
// using the accumulated sum of all forces impacting the entity.
// Param1: The time in seconds passed since the last frame.
// Param2: The maximal speed of the entity.
// Param3: The maximal size of the accumulated forces.
// Param4: The handicap to use if the entity is currently being handicapped.
//--------------------------------------------------------------------------------------
void EntityMovementManager::UpdatePosition(float deltaTime, float maxSpeed, float maxForce, float handicap)
{
	// Truncate steering force to not be greater than the maximal allowed force
	float magnitude = 0.0f;
	XMStoreFloat(&magnitude, XMVector2Length(XMLoadFloat2(&m_steeringForce)));

	if(magnitude > maxForce)
	{
		// Truncate the vector to be of the magnitude corresponding to the maximal allowed force
		XMStoreFloat2(&m_steeringForce, XMVector2Normalize(XMLoadFloat2(&m_steeringForce)) * maxForce);
	}

	// Calculate the new velocity for the entity
	XMFLOAT2 newVelocity;
	XMStoreFloat2(&newVelocity, XMLoadFloat2(&m_velocity) + XMLoadFloat2(&m_steeringForce));

	// Truncate the velocity if it is greater than the maximally allowed velocity for the entity
	XMStoreFloat(&magnitude, XMVector2Length(XMLoadFloat2(&newVelocity)));

	if(magnitude > maxSpeed)
	{
		// Truncate the vector to be of the magnitude corresponding to the maximal allowed velocity
		XMStoreFloat2(&newVelocity, XMVector2Normalize(XMLoadFloat2(&newVelocity)) * maxSpeed);
	}

	// Set the new velocity and position on the entity

	m_velocity = newVelocity;
		
	XMFLOAT2 newPosition;

	if(m_pEntity->IsHandicapped())
	{
		XMStoreFloat2(&newPosition, XMLoadFloat2(&m_pEntity->GetPosition()) + XMLoadFloat2(&newVelocity) * deltaTime * handicap);
	}else
	{
		XMStoreFloat2(&newPosition, XMLoadFloat2(&m_pEntity->GetPosition()) + XMLoadFloat2(&newVelocity) * deltaTime);
	}

	m_pEntity->SetPosition(newPosition);
	m_pEntity->UpdateColliderPosition(newPosition);

	// Update the rotation to make the entity face the direction, in which it is moving
	if(!(newVelocity.x == 0.0f && newVelocity.y == 0.0f))
	{
		XMFLOAT2 lookAtPoint(0.0f, 0.0f);
		XMStoreFloat2(&lookAtPoint, XMLoadFloat2(&m_pEntity->GetPosition()) + XMLoadFloat2(&newVelocity));
		LookAt(lookAtPoint);
	}

	// Reset the steering force for the next frame
	m_steeringForce.x = 0.0f;
	m_steeringForce.y = 0.0f;
}

//--------------------------------------------------------------------------------------
// Resets the movement component of the entity.
//--------------------------------------------------------------------------------------
void EntityMovementManager::Reset(void)
{
	m_path.clear();
	m_velocity = XMFLOAT2(0.0f, 0.0f);
	m_currentNode = 0;
}

//--------------------------------------------------------------------------------------
// Calculate a path to the target position.
// Param1: The target position of the path.
// Returns a pointer to the path that was created, nullptr if no path exists.
//--------------------------------------------------------------------------------------
std::vector<XMFLOAT2>* EntityMovementManager::CreatePathTo(const XMFLOAT2& targetPosition)
{
	bool result = false;
	result = m_pEnvironment->GetPathfinder().CalculatePath(AStar, EuclideanDistance, m_pEntity->GetPosition(), targetPosition, m_path);

	if(result)
	{
		m_currentNode = 0;
		return &m_path;
	}else
	{
		return nullptr;
	}
}

//--------------------------------------------------------------------------------------
// Move the entity to a specified target position.
// Param1: The target position of the entity.
// Param2: The radius around the target position from which the target counts as reached.
// Param3: The speed, at which the entity should seek the target.
// Returns true if the target was reached, false if there is still way to go.
//--------------------------------------------------------------------------------------
bool EntityMovementManager::Seek(const XMFLOAT2& targetPosition, float targetReachedRadius, float speed)
{
	XMFLOAT2 desiredVelocity;
	XMStoreFloat2(&desiredVelocity, XMLoadFloat2(&targetPosition) - XMLoadFloat2(&m_pEntity->GetPosition()));

	// Get the distance from the current position of the entity to the target
	float distance;
	XMStoreFloat(&distance, XMVector2Length(XMLoadFloat2(&desiredVelocity)));

	// Normalise the desired velocity
	XMStoreFloat2(&desiredVelocity, XMVector2Normalize(XMLoadFloat2(&desiredVelocity)));

	XMStoreFloat2(&desiredVelocity, XMLoadFloat2(&desiredVelocity) * speed);
	
	bool targetReached = false;

	// Target reached
	if(distance <= targetReachedRadius)
	{
		desiredVelocity = XMFLOAT2(0.0f, 0.0f);
		targetReached = true;
	}

	XMFLOAT2 force;
	XMStoreFloat2(&force, XMLoadFloat2(&desiredVelocity) - XMLoadFloat2(&m_velocity));

	// Add the seek force to the accumulated steering force
	XMStoreFloat2(&m_steeringForce, XMLoadFloat2(&m_steeringForce) + XMLoadFloat2(&force));

	return targetReached;
}

//--------------------------------------------------------------------------------------
// Rotates the entity towards a specific point.
// Param1: The position, the entity should look at.
//--------------------------------------------------------------------------------------
void EntityMovementManager::LookAt(const XMFLOAT2& lookAtPosition)
{
	XMFLOAT2 newViewDirection(0.0f, 1.0f);
	XMStoreFloat2(&newViewDirection, XMVector2Normalize(XMLoadFloat2(&lookAtPosition) - XMLoadFloat2(&m_pEntity->GetPosition())));
	
	m_pEntity->SetViewDirection(newViewDirection);
	
	float rotation = (atan2(m_pEntity->GetViewDirection().x, m_pEntity->GetViewDirection().y)) * 180 / XM_PI;
	m_pEntity->SetRotation(rotation);
}

//--------------------------------------------------------------------------------------
// Ensures that the entity stays where it is. A change of the view direction is allowed though.
//--------------------------------------------------------------------------------------
void EntityMovementManager::Wait()
{
	m_velocity      = XMFLOAT2(0.0f, 0.0f);
	m_steeringForce = XMFLOAT2(0.0f, 0.0f);
}

//--------------------------------------------------------------------------------------
// Calculate the path following force and add it to the total force.
// Param1: A pointer to the path to follow.
// Param2: When the entity has approached the target by this distance, it counts as reached.
// Param3: The speed, at which the entity should follow the path.
// Returns true if the end of the current path was reached, false if there is still way to go.
//--------------------------------------------------------------------------------------
bool EntityMovementManager::FollowPath(std::vector<XMFLOAT2>* pPath, float nodeReachedRadius, float speed)
{
	if(!pPath)
	{
		return false;
	}

	if(!pPath->empty())
	{
		// There is an active path
		XMFLOAT2 target = (*pPath)[m_currentNode];
	
		// Calculate the distance between the current position of the entity and the target
		float distance = 0.0f;
		XMStoreFloat(&distance, XMVector2Length(XMLoadFloat2(&target) - XMLoadFloat2(&m_pEntity->GetPosition())));

		if(distance <= nodeReachedRadius)
		{
			// The entity has reached the node
			++m_currentNode;

			if(m_currentNode >= pPath->size())
			{
				// Final destination reached, clear the path
				pPath->clear();
				m_velocity = XMFLOAT2(0.0f, 0.0f);
			
				return true;
			}else
			{
				Seek((*pPath)[m_currentNode], nodeReachedRadius, speed);
				return false;
			}
		}else
		{
			Seek((*pPath)[m_currentNode], nodeReachedRadius, speed);
			return false;
		}
	}

	return true;
}

//--------------------------------------------------------------------------------------
// Calculate the collision avoidance force and add it to the total force. This force 
// is used to avoid collisions of an entity with static obstacles in the environment and
// other entities.
// Param1: Obstacles and entities within this distance in front of the entity will be avoided.
// Param2: The maximal size of this force.
//--------------------------------------------------------------------------------------
void EntityMovementManager::AvoidCollisions(float seeAheadDistance, float maximalForce)
{
	if(m_velocity.x == 0.0f && m_velocity.y == 0.0f)
	{
		return;
	}

	std::multimap<float, CollidableObject*> nearbyObjects;
	m_pEnvironment->GetNearbyObjects(m_pEntity->GetPosition(), seeAheadDistance, GroupAllSoldiersAndObstacles, nearbyObjects);

	// One element is entity itself -> Check if there are more than one
	if(nearbyObjects.size() > 1)
	{
		XMFLOAT2 lineEndPoint(0.0f, 0.0f);
		XMStoreFloat2(&lineEndPoint, XMLoadFloat2(&m_pEntity->GetPosition()) + XMVector2Normalize(XMLoadFloat2(&m_pEntity->GetViewDirection())) * seeAheadDistance);

		for(std::multimap<float, CollidableObject*>::iterator it = nearbyObjects.begin(); it != nearbyObjects.end(); ++it)
		{
			
			if((it->second->GetId() != m_pEntity->GetId()) && reinterpret_cast<CollidableObject*>(it->second)->GetCollider()->CheckLineCollision(m_pEntity->GetPosition(), lineEndPoint))
			{
				// Determine whether the other entity is left or right of this entity

				XMFLOAT2 entityToObject(0.0f, 0.0f);
				XMStoreFloat2(&entityToObject, XMLoadFloat2(&it->second->GetPosition()) - XMLoadFloat2(&m_pEntity->GetPosition()));

				float dot = m_pEntity->GetViewDirection().x * (-entityToObject.y) + m_pEntity->GetViewDirection().y * entityToObject.x;	
					
				XMFLOAT2 avoidanceVector = m_pEntity->GetViewDirection();

				if(dot > 0)
				{
					// Object is right of entity, steer left to avoid
					avoidanceVector.x = -m_pEntity->GetViewDirection().y;
					avoidanceVector.y = m_pEntity->GetViewDirection().x;
				}else
				{
					// Object is left of entity, steer right to avoid
					avoidanceVector.x = m_pEntity->GetViewDirection().y;
					avoidanceVector.y = -m_pEntity->GetViewDirection().x;
				}
					
				XMStoreFloat2(&m_steeringForce, XMLoadFloat2(&m_steeringForce) + XMVector2Normalize(XMLoadFloat2(&avoidanceVector)) * maximalForce);
					
				// Bail out of the function as soon as the first collision is found (the nearby entities are already sorted
				// by their distance from the original entity, thus the first collision found is the closest one)

				return;
			}
			
		}
	}
}

//--------------------------------------------------------------------------------------
// Calculate the wall avoidance force and add it to the total force. This force 
// is used to push the entity away from walls (objects) that it is getting too close to.
// Param1: Obstacles within this radius around the entity will be avoided.
// Param2: The maximal size of this force.
//--------------------------------------------------------------------------------------
void EntityMovementManager::StayAwayFromWalls(float avoidWallsRadius, float maximalForce)
{
	// Get nearby obstacles

	std::multimap<float, CollidableObject*> nearbyObjects;
	m_pEnvironment->GetNearbyObjects(m_pEntity->GetPosition(), avoidWallsRadius, GroupObstacles, nearbyObjects);

	if(!nearbyObjects.empty())
	{
		XMVECTOR avoidanceForce = XMVectorZero();

		for(std::multimap<float, CollidableObject*>::iterator it = nearbyObjects.begin(); it != nearbyObjects.end(); ++it)
		{
			// Scale the force according to the proximity of the nearby entity. Thus the push from close objects will be
			// stronger than that from objects that are farther away.
			avoidanceForce += (XMLoadFloat2(&m_pEntity->GetPosition()) - XMLoadFloat2(&(it->second->GetPosition()))) / it->first;
		}

		// Truncate the force according to the maximally allowed wall avoidance force.
		avoidanceForce = XMVector2Normalize(avoidanceForce) * maximalForce;

		// Add the collision avoidance force to the accumulated steering force
		XMStoreFloat2(&m_steeringForce, XMLoadFloat2(&m_steeringForce) + avoidanceForce);
	}
}

//--------------------------------------------------------------------------------------
// Calculate the separation force and add it to the total force.
// Param1: The entity will try to move away from entities that are at this distance or closer.
// Param2: The maximal size of this force.
//--------------------------------------------------------------------------------------
void EntityMovementManager::Separate(float separationRadius, float maximalForce)
{
	XMVECTOR separationVector = XMVectorZero(); // The separation force to prevent overlapping of moving entities

	// Find the moving entities that are in close proximity to this one (check both teams)

	std::multimap<float, CollidableObject*> nearbySoldiers;
	m_pEnvironment->GetNearbyObjects(m_pEntity->GetPosition(), separationRadius, GroupAllSoldiers, nearbySoldiers);

	// The entity itself will be included in the list of soldiers -> check if there are others as well
	if(nearbySoldiers.size() > 1)
	{
		for(std::multimap<float, CollidableObject*>::const_iterator it = nearbySoldiers.begin(); it !=  nearbySoldiers.end(); ++it)
		{
			if(it->second->GetId() != m_pEntity->GetId())
			{
				// Scale the force according to the proximity of the nearby entity. Thus the push from close objects will be
				// stronger than that from objects that are farther away.

				// Avoid possible division by zero
				float proximity = (it->first != 0) ? it->first : 0.01f;

				separationVector += (XMLoadFloat2(&m_pEntity->GetPosition()) - XMLoadFloat2(&it->second->GetPosition())) / proximity;
			}
		}

		// Truncate the force according to the maximally allowed separation force.
		separationVector = XMVector2Normalize(separationVector) * maximalForce;

		// Add the separation force to the accumulated steering force
		XMStoreFloat2(&m_steeringForce, XMLoadFloat2(&m_steeringForce) + separationVector);
	}
}

// Data access functions

const XMFLOAT2& EntityMovementManager::GetVelocity(void) const
{
	return m_velocity;
}

unsigned int EntityMovementManager::GetCurrentNode(void) const
{
	return m_currentNode;
}

void EntityMovementManager::SetCurrentNode(unsigned int node)
{
	m_currentNode = node;
}