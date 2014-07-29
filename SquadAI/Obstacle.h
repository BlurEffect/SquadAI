/* 
*  Kevin Meergans, SquadAI, 2014
*  Obstacle.h
*  Represents an obstacle in the test environment.
*/

#ifndef OBSTACLE_H
#define OBSTACLE_H

// Includes
#include <DirectXMath.h>
#include "CollidableObject.h"

using namespace DirectX;

class Obstacle : public CollidableObject
{
public:
	Obstacle(void);
	~Obstacle(void);

	bool Initialise(unsigned long id, const XMFLOAT2& position, float rotation, float uniformScale, ObjectCategory category, ColliderType colliderType, void* pColliderData);
};

#endif // OBSTACLE_H