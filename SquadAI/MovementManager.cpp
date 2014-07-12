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

	return true;
}

//--------------------------------------------------------------------------------------
// Updates the position and velocity of the entity associated to this movement manager
// using the accumulated sum of all forces impacting the entity.
//--------------------------------------------------------------------------------------
void MovementManager::Update(void)
{
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
	XMStoreFloat2(&newPosition, XMLoadFloat2(&m_pEntity->GetPosition()) + XMLoadFloat2(&newVelocity));

	m_pEntity->SetPosition(newPosition);

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
		XMStoreFloat(&distance, XMVector2Length(XMLoadFloat2(&m_pEntity->GetPosition()) - XMLoadFloat2(&target)));

		if(distance <= nodeReachedRadius)
		{
			// The entity has reached the node
			++m_currentNode;

			if(m_currentNode >= m_path.size())
			{
				// final destination reached
				// clear path?
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
}
/*
//--------------------------------------------------------------------------------------
// Calculate the seek force and add it to the total force.
//--------------------------------------------------------------------------------------
void MovementManager::UpdateSeekForce(void)
{
	/*
	var force :Vector3D;
    var distance :Number;
 
    desired = target.subtract(host.getPosition());
 
    distance = desired.length;
    desired.normalize();
 
    if (distance <= slowingRadius) {
        desired.scaleBy(host.getMaxVelocity() * distance/slowingRadius);
    } else {
        desired.scaleBy(host.getMaxVelocity());
    }
 
    force = desired.subtract(host.getVelocity());
 
    return force;
	*/
//}
