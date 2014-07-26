/* 
*  Kevin Meergans, SquadAI, 2014
*  CollidableObject.h
*  Abstract base class for objects that can collide with others in the
*  test environment.
*/

#ifndef COLLIDABLE_OBJECT_H
#define COLLIDABLE_OBJECT_H

// Includes
#include <DirectXMath.h>
#include "Object.h"
#include "ColliderFactory.h"

using namespace DirectX;

class CollidableObject : public Object
{
public:
	CollidableObject(void);
	virtual ~CollidableObject(void) = 0;

	bool Initialise(const XMFLOAT2& position, float rotation, float uniformScale, ColliderType colliderType, void* pColliderData);

	// Data access functions
	const Collider* GetCollider(void) const;

private:
	Collider*  m_pCollider;			// The collider associated to the object
};

#endif // COLLIDABLE_OBJECT_H