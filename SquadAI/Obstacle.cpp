/* 
*  Kevin Meergans, SquadAI, 2014
*  Obstacle.cpp
*  Represents an obstacle in the test environment.
*/

// Includes
#include "Obstacle.h"


Obstacle::Obstacle(void) : CollidableObject()
{
}

Obstacle::~Obstacle(void)
{
}

//--------------------------------------------------------------------------------------
// Initialises the obstacle.
// Param1: The position of the object.
// Param2: The rotation of the object along the world z-axis.
// Param3: The uniform scale of the object.
// Param4: The type of the collider that should be created.
// Param5: A pointer to the initialisation data for the collider.
// Returns true if the obstacle was initialised successfully, false otherwise.
//--------------------------------------------------------------------------------------
bool Obstacle::Initialise(const XMFLOAT2& position, float rotation, float uniformScale, ColliderType colliderType, void* pColliderData)
{
	return CollidableObject::Initialise(position, rotation, uniformScale, colliderType, pColliderData);
}

