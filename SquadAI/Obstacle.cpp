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
// Param1: A unique identifier for the object.
// Param2: The position of the object.
// Param3: The rotation of the object along the world z-axis.
// Param4: The uniform scale of the object.
// Param5: The category the object belongs to.
// Param6: The type of the collider that should be created.
// Param7: A pointer to the initialisation data for the collider.
// Returns true if the obstacle was initialised successfully, false otherwise.
//--------------------------------------------------------------------------------------
bool Obstacle::Initialise(unsigned long id, const XMFLOAT2& position, float rotation, float uniformScale, ObjectCategory category, ColliderType colliderType, void* pColliderData)
{
	return CollidableObject::Initialise(id, position, rotation, uniformScale, category, colliderType, pColliderData);
}

