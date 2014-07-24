/* 
*  Kevin Meergans, SquadAI, 2014
*  EntityMovementManager.cpp
*  Each entity able to move has a movement manager associated to it that
*  is responsible for controlling the position and velocity of the entity.
*/

// Includes
#include "EntityMovementManager.h"
#include "MovingEntity.h"
#include "Pathfinder.h"
#include "TestEnvironment.h"

EntityMovementManager::EntityMovementManager(void) : m_pEntity(nullptr),
									 				 m_velocity(XMFLOAT2(0.0f, 0.0f)),
									 				 m_maxVelocity(0.0f),
													 m_maxTotalForce(0.0f),
													 m_maxSeeAhead(0.0f),
													 m_maxCollisionAvoidanceForce(0.0f),
													 m_maxAvoidWallsForce(0.0f),
													 m_maxSeparationForce(0.0f),
													 m_targetReachedRadius(0.0f),
													 m_avoidWallsRadius(0.0f),
													 m_separationRadius(0.0f),
													 m_viewDirection(0.0f, 1.0f),
													 m_steeringForce(0.0f, 0.0f),
													 m_currentNode(0),
													 m_seekTarget(0.0f, 0.0f)
{
}

EntityMovementManager::~EntityMovementManager(void)
{
}

//--------------------------------------------------------------------------------------
// Initialises movement manager for a given entity.
// Param1: A pointer to the entity that this movement manager will be associated to.
// Param2: The initialisation data required to initialise the movement component of the entity.
// Returns true if the movement manager could be initialised successfully, false otherwise.
//--------------------------------------------------------------------------------------
bool EntityMovementManager::Initialise(MovingEntity* pEntity, const EntityMovementInitData& initData)
{
	if(!pEntity)
	{
		return false;
	}

	m_pEntity = pEntity;

	m_maxVelocity				 = initData.m_maxVelocity;
	m_maxTotalForce				 = initData.m_maxTotalForce;
	m_maxSeeAhead				 = initData.m_maxSeeAhead;
	m_maxCollisionAvoidanceForce = initData.m_maxCollisionAvoidanceForce;
	m_maxAvoidWallsForce         = initData.m_maxAvoidWallsForce;
	m_maxSeparationForce		 = initData.m_maxSeparationForce;
	m_targetReachedRadius		 = initData.m_targetReachedRadius;
	m_avoidWallsRadius			 = initData.m_avoidWallsRadius;
	m_separationRadius			 = initData.m_separationRadius;

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
		m_viewDirection = XMFLOAT2(0.0f, 1.0f);
	}else if(m_pEntity->GetRotation() == 90.0f)
	{
		m_viewDirection = XMFLOAT2(1.0f, 0.0f);
	}else if(m_pEntity->GetRotation() == 180.0f)
	{
		m_viewDirection = XMFLOAT2(0.0f, -1.0f);
	}else
	{
		// 270 degrees rotation
		m_viewDirection = XMFLOAT2(-1.0f, 0.0f);
	}
}

//--------------------------------------------------------------------------------------
// Updates the position and velocity of the entity associated to this movement manager
// using the accumulated sum of all forces impacting the entity.
// Param1: The time in seconds passed since the last frame.
//--------------------------------------------------------------------------------------
void EntityMovementManager::Update(float deltaTime)
{
	// Truncate steering force to not be greater than the maximal allowed force
	float magnitude = 0.0f;
	XMStoreFloat(&magnitude, XMVector2Length(XMLoadFloat2(&m_steeringForce)));

	if(magnitude > m_maxTotalForce)
	{
		// Truncate the vector to be of the magnitude corresponding to the maximal allowed force
		XMStoreFloat2(&m_steeringForce, XMVector2Normalize(XMLoadFloat2(&m_steeringForce)) * m_maxTotalForce);
	}

	// Calculate the new velocity for the entity
	XMFLOAT2 newVelocity;
	XMStoreFloat2(&newVelocity, XMLoadFloat2(&m_velocity) + XMLoadFloat2(&m_steeringForce));

	// Truncate the velocity if it is greater than the maximally allowed velocity for the entity
	XMStoreFloat(&magnitude, XMVector2Length(XMLoadFloat2(&newVelocity)));

	if(magnitude > m_maxVelocity)
	{
		// Truncate the vector to be of the magnitude corresponding to the maximal allowed velocity
		XMStoreFloat2(&newVelocity, XMVector2Normalize(XMLoadFloat2(&newVelocity)) * m_maxVelocity);
	}

	// Set the new velocity and position on the entity

	m_velocity = newVelocity;
	//XMStoreFloat2(&m_velocity, XMLoadFloat2(&newVelocity) * deltaTime );
		

	XMFLOAT2 newPosition;
	XMStoreFloat2(&newPosition, XMLoadFloat2(&m_pEntity->GetPosition()) + XMLoadFloat2(&newVelocity) * deltaTime);

	m_pEntity->SetPosition(newPosition);
	m_pEntity->GetCollider()->SetCentre(m_pEntity->GetPosition());

	// Update the rotation to make the entity face the direction, in which it is moving
	if(!(newVelocity.x == 0.0f && newVelocity.y == 0.0f))
	{
		LookAt(newVelocity);

		//m_viewDirection = newVelocity;
		//float rotation = (atan2(m_viewDirection.x, m_viewDirection.y)) * 180 / XM_PI;
		//m_pEntity->SetRotation(rotation);
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
	m_velocity = XMFLOAT2(0.0f, 0.0f);
	m_currentNode = 0;
}

//--------------------------------------------------------------------------------------
// Calculate a path to the target position and set it active.
// Param1: The target position of the entity.
// Returns true if a path for the entity could be created successfully, false otherwise.
//--------------------------------------------------------------------------------------
bool EntityMovementManager::SetPathTo(const XMFLOAT2& targetPosition)
{
	bool result = false;
	result = m_pEntity->GetTestEnvironment()->GetPathfinder().CalculatePath(AStar, EuclideanDistance, m_pEntity->GetPosition(), targetPosition, m_path);

	if(result)
	{
		m_currentNode = 0;
	}

	return result;
}

//--------------------------------------------------------------------------------------
// Move the entity to a specified target position.
// Param1: The target position of the entity.
// Param2: The radius around the target position from which the target counts as reached
//--------------------------------------------------------------------------------------
void EntityMovementManager::Seek(const XMFLOAT2& targetPosition, float targetReachedRadius)
{
	XMFLOAT2 desiredVelocity;
	XMStoreFloat2(&desiredVelocity, XMLoadFloat2(&targetPosition) - XMLoadFloat2(&m_pEntity->GetPosition()));

	// Get the distance from the current position of the entity to the target
	float distance;
	XMStoreFloat(&distance, XMVector2Length(XMLoadFloat2(&desiredVelocity)));

	// Normalise the desired velocity
	XMStoreFloat2(&desiredVelocity, XMVector2Normalize(XMLoadFloat2(&desiredVelocity)));

	
	XMStoreFloat2(&desiredVelocity, XMLoadFloat2(&desiredVelocity) * m_maxVelocity);
	

	// Target reached
	if(distance <= targetReachedRadius)
	{
		desiredVelocity = XMFLOAT2(0.0f, 0.0f);
	}

	XMFLOAT2 force;
	XMStoreFloat2(&force, XMLoadFloat2(&desiredVelocity) - XMLoadFloat2(&m_velocity));

	// Add the seek force to the accumulated steering force
	XMStoreFloat2(&m_steeringForce, XMLoadFloat2(&m_steeringForce) + XMLoadFloat2(&force));
}

//--------------------------------------------------------------------------------------
// Updates the view direction of the entity and rotates it towards the specified direction.
// Param1: The direction, into which the entity should look.
//--------------------------------------------------------------------------------------
void EntityMovementManager::LookAt(const XMFLOAT2& direction)
{
	XMStoreFloat2(&m_viewDirection, XMLoadFloat2(&direction));
	float rotation = (atan2(m_viewDirection.x, m_viewDirection.y)) * 180 / XM_PI;
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
// Param1: When the entity has approached the target by this distance, it counts as reached.
//--------------------------------------------------------------------------------------
void EntityMovementManager::FollowPath(float nodeReachedRadius)
{
	if(!m_path.empty())
	{
		// There is an active path
		XMFLOAT2 target = m_path[m_currentNode];
	
		// Calculate the distance between the current position of the entity and the target
		float distance = 0.0f;
		XMStoreFloat(&distance, XMVector2Length(XMLoadFloat2(&target) - XMLoadFloat2(&m_pEntity->GetPosition())));

		if(distance <= nodeReachedRadius)
		{
			// The entity has reached the node
			++m_currentNode;

			if(m_currentNode >= m_path.size())
			{
				// Final destination reached, clear the path
				m_path.clear();
				m_velocity = XMFLOAT2(0.0f, 0.0f);

				// Debug, remove this
				//m_pEntity->GetTestEnvironment()->AddProjectile(m_pEntity->GetPosition(), XMFLOAT2(m_pEntity->GetPosition().x + 10, m_pEntity->GetPosition().y + 10));
			}
		}else
		{
			if(m_currentNode < m_path.size() - 1)
			{
				Seek(m_path[m_currentNode], nodeReachedRadius);
			}else
			{
				// Slow arrival for the last node
				Seek(m_path[m_currentNode], nodeReachedRadius);
			}
		}
	}
}

//--------------------------------------------------------------------------------------
// Calculate the collision avoidance force and add it to the total force. This force 
// is used to avoid collisions of the entity with static obstacles in the environment, 
// such as walls.
// Param1: Obstacles within this distance in front of the entity will be avoided.
//--------------------------------------------------------------------------------------
void EntityMovementManager::AvoidObstacleCollisions(float seeAhead)
{
	if(m_velocity.x == 0.0f && m_velocity.y == 0.0f)
	{
		return;
	}

	std::multimap<float, Entity*> nearbyEntities;
	m_pEntity->GetTestEnvironment()->GetNearbyEntities(m_pEntity, seeAhead, GroupAllSoldiersAndObstacles, nearbyEntities);

	if(!nearbyEntities.empty())
	{
		XMFLOAT2 lineEndPoint(0.0f, 0.0f);
		XMStoreFloat2(&lineEndPoint, XMLoadFloat2(&m_pEntity->GetPosition()) + XMVector2Normalize(XMLoadFloat2(&m_pEntity->GetViewDirection())) * seeAhead);

		for(std::multimap<float, Entity*>::iterator it = nearbyEntities.begin(); it != nearbyEntities.end(); ++it)
		{
			
			if(it->second->GetCollider()->CheckLineCollision(m_pEntity->GetPosition(), lineEndPoint))
			{
				/*

				lineEndPoint = XMFLOAT2(it->second->GetPosition().x + 10.0f, it->second->GetPosition().y);

				XMVECTOR avoidanceForce = XMVector2Normalize(XMLoadFloat2(&lineEndPoint) - XMLoadFloat2(&it->second->GetPosition())) * m_maxCollisionAvoidanceForce;
		
				// Add the collision avoidance force to the accumulated steering force
				XMStoreFloat2(&m_steeringForce, XMLoadFloat2(&m_steeringForce) + avoidanceForce);

				break;

				*/

				// Alternative

				// Determine whether the other entity is left or right of this entity

				XMFLOAT2 entityToObject(0.0f, 0.0f);
				XMStoreFloat2(&entityToObject, XMLoadFloat2(&it->second->GetPosition()) - XMLoadFloat2(&m_pEntity->GetPosition()));

				float dot = GetViewDirection().x * (-entityToObject.y) + GetViewDirection().y * entityToObject.x;	
					
				XMFLOAT2 avoidanceVector = GetViewDirection();

				if(dot > 0)
				{
					// Object is right of entity, steer left to avoid
					avoidanceVector.x = -GetViewDirection().y;
					avoidanceVector.y = GetViewDirection().x;
				}else
				{
					// Object is left of entity, steer right to avoid
					avoidanceVector.x = GetViewDirection().y;
					avoidanceVector.y = -GetViewDirection().x;
				}
					
				XMStoreFloat2(&m_steeringForce, XMLoadFloat2(&m_steeringForce) + XMVector2Normalize(XMLoadFloat2(&avoidanceVector)) * m_maxCollisionAvoidanceForce);
					
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
//--------------------------------------------------------------------------------------
void EntityMovementManager::StayAwayFromWalls(float avoidWallsRadius)
{
	// Get nearby obstacles

	std::multimap<float, Entity*> nearbyEntities;
	m_pEntity->GetTestEnvironment()->GetNearbyEntities(m_pEntity, avoidWallsRadius, GroupObstacles, nearbyEntities);

	if(!nearbyEntities.empty())
	{
		XMVECTOR avoidanceForce = XMVectorZero();

		for(std::multimap<float, Entity*>::iterator it = nearbyEntities.begin(); it != nearbyEntities.end(); ++it)
		{
			// Scale the force according to the proximity of the nearby entity. Thus the push from close objects will be
			// stronger than that from objects that are farther away.
			avoidanceForce += (XMLoadFloat2(&m_pEntity->GetPosition()) - XMLoadFloat2(&(it->second->GetPosition()))) / it->first;
		}

		// Truncate the force according to the maximally allowed wall avoidance force.
		avoidanceForce = XMVector2Normalize(avoidanceForce) * m_maxAvoidWallsForce;

		// Add the collision avoidance force to the accumulated steering force
		XMStoreFloat2(&m_steeringForce, XMLoadFloat2(&m_steeringForce) + avoidanceForce);
	}
}

//--------------------------------------------------------------------------------------
// Calculate the separation force and add it to the total force.
// Param1: The entity will try to move away from entities that are at this distance or closer.
//--------------------------------------------------------------------------------------
void EntityMovementManager::Separate(float separationRadius)
{
	XMVECTOR separationVector = XMVectorZero(); // The separation force to prevent overlapping of moving entities

	// Find the moving entities that are in close proximity to this one (check both teams)

	std::multimap<float, Entity*> nearbySoldiers;
	m_pEntity->GetTestEnvironment()->GetNearbyEntities(m_pEntity, GetSeparationRadius(), GroupAllSoldiers, nearbySoldiers);

	if(!nearbySoldiers.empty())
	{
		for(std::multimap<float, Entity*>::const_iterator it = nearbySoldiers.begin(); it !=  nearbySoldiers.end(); ++it)
		{
			// Scale the force according to the proximity of the nearby entity. Thus the push from close objects will be
			// stronger than that from objects that are farther away.
			separationVector += (XMLoadFloat2(&m_pEntity->GetPosition()) - XMLoadFloat2(&it->second->GetPosition())) / it->first;
		}

		// Truncate the force according to the maximally allowed separation force.
		separationVector = XMVector2Normalize(separationVector) * m_maxSeparationForce;

		// Add the separation force to the accumulated steering force
		XMStoreFloat2(&m_steeringForce, XMLoadFloat2(&m_steeringForce) + separationVector);
	}
}

// Data access functions

const XMFLOAT2& EntityMovementManager::GetVelocity(void) const
{
	return m_velocity;
}

float EntityMovementManager::GetMaxVelocity(void) const
{
	return m_maxVelocity;
}

float EntityMovementManager::GetMaxTotalForce(void) const
{
	return m_maxTotalForce;
}

float EntityMovementManager::GetMaxSeeAhead(void) const
{
	return m_maxSeeAhead;
}

float EntityMovementManager::GetMaxCollisionAvoidanceForce(void) const
{
	return m_maxCollisionAvoidanceForce;
}

float EntityMovementManager::GetMaxAvoidWallsForce(void) const
{
	return m_maxAvoidWallsForce;
}

float EntityMovementManager::GetMaxSeparationForce(void) const
{
	return m_maxSeparationForce;
}

float EntityMovementManager::GetTargetReachedRadius(void) const
{
	return m_targetReachedRadius;
}

float EntityMovementManager::GetAvoidWallsRadius(void) const
{
	return m_avoidWallsRadius;
}

float EntityMovementManager::GetSeparationRadius(void) const
{
	return m_separationRadius;
}

const XMFLOAT2& EntityMovementManager::GetViewDirection(void) const
{
	return m_viewDirection;
}

void EntityMovementManager::SetVelocity(const XMFLOAT2& velocity)
{
	m_velocity = velocity;
}

void EntityMovementManager::SetMaxVelocity(float maxVelocity)
{
	m_maxVelocity = maxVelocity;
}

void EntityMovementManager::SetMaxTotalForce(float maxForce)
{
	m_maxTotalForce = maxForce;
}

void EntityMovementManager::SetMaxSeeAhead(float maxSeeAhead)
{
	m_maxSeeAhead = maxSeeAhead;
}

void EntityMovementManager::SetMaxCollisionAvoidanceForce(float maxCollisionAvoidanceForce)
{
	m_maxCollisionAvoidanceForce = maxCollisionAvoidanceForce;
}

void EntityMovementManager::SetMaxAvoidWallsForce(float maxAvoidWallsForce)
{
	m_maxAvoidWallsForce = maxAvoidWallsForce;
}

void EntityMovementManager::SetMaxSeparationForce(float maxSeparationForce)
{
	m_maxSeparationForce = maxSeparationForce;
}

void EntityMovementManager::SetTargetReachedRadius(float targetReachedRadius)
{
	m_targetReachedRadius = targetReachedRadius;
}

void EntityMovementManager::SetAvoidWallsRadius(float avoidWallsRadius)
{
	m_avoidWallsRadius = avoidWallsRadius;
}

void EntityMovementManager::SetSeparationRadius(float separationRadius)
{
	m_separationRadius = separationRadius;
}
