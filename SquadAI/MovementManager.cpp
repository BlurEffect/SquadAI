/* 
*  Kevin Meergans, SquadAI, 2014
*  MovementManager.cpp
*  Each entity able to move has a movement manager associated to it that
*  is responsible for controlling the position and velocity of the entity.
*/

// Includes
#include "MovementManager.h"
#include "MovingEntity.h"
#include "TestEnvironment.h"
#include "Pathfinder.h"

MovementManager::MovementManager(void) : m_pEntity(nullptr),
								         m_steeringForce(0.0f, 0.0f),
										 m_currentNode(0),
										 m_reachedNodeRadius(0.0f),
										 m_seekTarget(0.0f, 0.0f),
										 m_slowArrivalRadius(0.0f)
{
}

MovementManager::~MovementManager(void)
{
}

//--------------------------------------------------------------------------------------
// Initialises movement manager for a given entity.
// Param1: A pointer to the entity that this movement manager will be associated to.
// Returns true if the movement manager could be initialised successfully, false otherwise.
//--------------------------------------------------------------------------------------
bool MovementManager::Initialise(MovingEntity* pEntity)
{
	if(!pEntity)
	{
		return false;
	}

	m_pEntity = pEntity;
	m_pEntity->SetVelocity(XMFLOAT2(0.0f, 0.0f));

	SetPathTo(XMFLOAT2(20.0f, 20.0f));

	return true;
}

//--------------------------------------------------------------------------------------
// Updates the position and velocity of the entity associated to this movement manager
// using the accumulated sum of all forces impacting the entity.
// Param1: The time in seconds passed since the last frame.
//--------------------------------------------------------------------------------------
void MovementManager::Update(float deltaTime)
{
	FollowPath(2.0f); // 1.5f
	//Seek(XMFLOAT2(20,20), 5.0f);
	AvoidCollisions();
	Separate(3.0f);

	// Truncate steering force to not be greater than the maximal allowed force
	float magnitude = 0.0f;
	XMStoreFloat(&magnitude, XMVector2Length(XMLoadFloat2(&m_steeringForce)));

	if(magnitude > m_pEntity->GetMaxForce())
	{
		// Truncate the vector to be of the magnitude corresponding to the maximal allowed force
		XMStoreFloat2(&m_steeringForce, XMVector2Normalize(XMLoadFloat2(&m_steeringForce)) * m_pEntity->GetMaxForce());
	}

	// Calculate the new velocity for the entity
	XMFLOAT2 newVelocity;
	XMStoreFloat2(&newVelocity, XMLoadFloat2(&m_pEntity->GetVelocity()) + XMLoadFloat2(&m_steeringForce));

	// Truncate the velocity if it is greater than the maximally allowed velocity for the entity
	XMStoreFloat(&magnitude, XMVector2Length(XMLoadFloat2(&newVelocity)));

	if(magnitude > m_pEntity->GetMaxVelocity())
	{
		// Truncate the vector to be of the magnitude corresponding to the maximal allowed force
		XMStoreFloat2(&newVelocity, XMVector2Normalize(XMLoadFloat2(&newVelocity)) * m_pEntity->GetMaxVelocity());
	}

	// Set the new velocity and position on the entity

	m_pEntity->SetVelocity(newVelocity);

	XMFLOAT2 newPosition;
	XMStoreFloat2(&newPosition, XMLoadFloat2(&m_pEntity->GetPosition()) + XMLoadFloat2(&newVelocity) * deltaTime);

	m_pEntity->SetPosition(newPosition);

	// Update the rotation to make the entity face the direction, in which it is moving
	float rotation = (atan2(newVelocity.x, newVelocity.y)) * 180 / XM_PI;
	m_pEntity->SetRotation(rotation);

	// Reset the steering force for the next frame
	m_steeringForce.x = 0.0f;
	m_steeringForce.y = 0.0f;
}

//--------------------------------------------------------------------------------------
// Calculate a path to the target position and set it active.
// Param1: The target position of the entity.
// Returns true if a path for the entity could be created successfully, false otherwise.
//--------------------------------------------------------------------------------------
bool MovementManager::SetPathTo(const XMFLOAT2& targetPosition)
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
// Param2: The radius around the target position from which the entity will slow down upon approaching.
//--------------------------------------------------------------------------------------
void MovementManager::Seek(const XMFLOAT2& targetPosition, float slowArrivalRadius)
{
	XMFLOAT2 desiredVelocity;
	XMStoreFloat2(&desiredVelocity, XMLoadFloat2(&targetPosition) - XMLoadFloat2(&m_pEntity->GetPosition()));

	// Get the distance from the current position of the entity to the target
	float distance;
	XMStoreFloat(&distance, XMVector2Length(XMLoadFloat2(&desiredVelocity)));

	// Normalise the desired velocity
	XMStoreFloat2(&desiredVelocity, XMVector2Normalize(XMLoadFloat2(&desiredVelocity)));

	if(distance <= slowArrivalRadius)
	{
		XMStoreFloat2(&desiredVelocity, XMLoadFloat2(&desiredVelocity) * m_pEntity->GetMaxVelocity() * distance/slowArrivalRadius);
	}else
	{
		XMStoreFloat2(&desiredVelocity, XMLoadFloat2(&desiredVelocity) * m_pEntity->GetMaxVelocity());
	}

	XMFLOAT2 force;
	XMStoreFloat2(&force, XMLoadFloat2(&desiredVelocity) - XMLoadFloat2(&m_pEntity->GetVelocity()));

	// Add the seek force to the accumulated steering force
	XMStoreFloat2(&m_steeringForce, XMLoadFloat2(&m_steeringForce) + XMLoadFloat2(&force));
}

//--------------------------------------------------------------------------------------
// Calculate the path following force and add it to the total force.
// Param1: When the entity has approached the target by this distance, it counts as reached.
//--------------------------------------------------------------------------------------
void MovementManager::FollowPath(float nodeReachedRadius)
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
				m_pEntity->SetVelocity(XMFLOAT2(0.0f, 0.0f));
			}
		}else
		{
			if(m_currentNode < m_path.size() - 1)
			{
				Seek(m_path[m_currentNode], 0.0f);
			}else
			{
				// Slow arrival for the last node
				Seek(m_path[m_currentNode], 2.0f);
			}
		}
	}
}

//--------------------------------------------------------------------------------------
// Calculate the collision avoidance force and add it to the total force.
//--------------------------------------------------------------------------------------
void MovementManager::AvoidCollisions()
{
	const Entity* pCollisionObject = m_pEntity->GetTestEnvironment()->GetCollisionObject(*m_pEntity);
	
	if(pCollisionObject != nullptr)
	{
		XMVECTOR ahead = XMLoadFloat2(&m_pEntity->GetPosition()) + XMVector2Normalize(XMLoadFloat2(&m_pEntity->GetVelocity())) * m_pEntity->GetMaxSeeAhead();

		XMFLOAT2 avoidanceForce;
		
		//
		/*
		XMFLOAT2 a = m_pEntity->GetPosition();
		a.x += m_pEntity->GetTestEnvironment()->GetGridSize() * 0.5f;
		a.y += m_pEntity->GetTestEnvironment()->GetGridSize() * 0.5f;

		ahead = XMLoadFloat2(&a) + XMVector2Normalize(XMLoadFloat2(&m_pEntity->GetVelocity())) * m_pEntity->GetMaxSeeAhead();


		XMFLOAT2 b;
		XMStoreFloat2(&b, XMLoadFloat2(&a) + XMLoadFloat2(&m_pEntity->GetVelocity()));

		XMFLOAT2 c = pCollisionObject->GetPosition();
		c.x += m_pEntity->GetTestEnvironment()->GetGridSize() * 0.5f;
		c.y += m_pEntity->GetTestEnvironment()->GetGridSize() * 0.5f;

		// Determine what side of the obstacle will be hit
		//(Bx - Ax) * (Cy - Ay) - (By - Ay) * (Cx - Ax)
		float result = (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);


		XMFLOAT3 toCamera(0.0f, 0.0f, 1.0f);
		XMFLOAT3 avoidDirection;
		XMStoreFloat3(&avoidDirection, XMVector3Cross(ahead, XMLoadFloat3(&toCamera)));
		
		avoidanceForce.x = avoidDirection.x;
		avoidanceForce.y = avoidDirection.y;
		
		if(result >= 0)
		{
			XMStoreFloat2(&avoidanceForce, XMVector2Normalize(XMLoadFloat2(&avoidanceForce)) * g_kMaxCollisionAvoidanceForce);
		}else
		{
			XMStoreFloat2(&avoidanceForce, XMVector2Normalize(XMLoadFloat2(&avoidanceForce)) * -1.0f * g_kMaxCollisionAvoidanceForce);
		}
		*/
		//

		
		if(pCollisionObject->GetType() == CoverSpot)
		{
			XMStoreFloat2(&avoidanceForce, XMVector2Normalize(ahead - XMLoadFloat2(&pCollisionObject->GetPosition())) * g_kMaxCollisionAvoidanceForce);
		}else
		{
			XMStoreFloat2(&avoidanceForce, XMVector2Normalize(ahead - XMLoadFloat2(&pCollisionObject->GetPosition())) * g_kMaxCollisionAvoidanceForce * 2.0f);
		}

		// Add the collision avoidance force to the accumulated steering force
		XMStoreFloat2(&m_steeringForce, XMLoadFloat2(&m_steeringForce) + XMLoadFloat2(&avoidanceForce));
	}
}

//--------------------------------------------------------------------------------------
// Calculate the separation force and add it to the total force.
// Param1: The entity will try to move away from entities that are close than this distance.
//--------------------------------------------------------------------------------------
void MovementManager::Separate(float separationRadius)
{
	// Use square distances
	float squareRadius = separationRadius * separationRadius;

    XMFLOAT2     separationForce(0.0f, 0.0f); // The separation force to prevent overlapping of moving entities
    unsigned int numberOfNeighbours = 0;      // The number of moving entities in close proximity

	// Find the moving entities that are in close proximity to this one (check both teams)

	for(std::list<Soldier>::const_iterator it = m_pEntity->GetTestEnvironment()->GetTeamA().begin(); it !=  m_pEntity->GetTestEnvironment()->GetTeamA().end(); ++it)
	{
		if(it->GetId() != m_pEntity->GetId())
		{
			// Calculate the distance between the entity and other entities
			XMVECTOR distanceVector = XMLoadFloat2(&it->GetPosition()) - XMLoadFloat2(&m_pEntity->GetPosition());

			float squareDistance = 0.0f;
			XMStoreFloat(&squareDistance, XMVector2Dot(distanceVector, distanceVector));
			
			if(squareDistance < squareRadius)
			{
				XMStoreFloat2(&separationForce, XMLoadFloat2(&separationForce) + (distanceVector / -squareDistance));
				++numberOfNeighbours;
			}
		}
	}

	for(std::list<Soldier>::const_iterator it = m_pEntity->GetTestEnvironment()->GetTeamB().begin(); it !=  m_pEntity->GetTestEnvironment()->GetTeamB().end(); ++it)
	{
		if(it->GetId() != m_pEntity->GetId())
		{
			// Calculate the distance between the entity and other entities
			XMVECTOR distanceVector = XMLoadFloat2(&it->GetPosition()) - XMLoadFloat2(&m_pEntity->GetPosition());

			float squareDistance = 0.0f;
			XMStoreFloat(&squareDistance, XMVector2Dot(distanceVector, distanceVector));
			
			if(squareDistance < squareRadius)
			{
				XMStoreFloat2(&separationForce, XMLoadFloat2(&separationForce) + (distanceVector / -squareDistance));
				++numberOfNeighbours;
			}
		}
	}

	// Divide the force by the number of neighbours and truncate it according to the maximally allowed
	// separation force.
	if(numberOfNeighbours > 0)
	{
		XMVECTOR separationVector =  XMLoadFloat2(&separationForce) / static_cast<float>(numberOfNeighbours);
		separationVector = XMVector2Normalize(separationVector);
		separationVector *= g_kMaxSeparationForce;

		// Add the separation force to the accumulated steering force
		XMStoreFloat2(&m_steeringForce, XMLoadFloat2(&m_steeringForce) + separationVector);
	}

}