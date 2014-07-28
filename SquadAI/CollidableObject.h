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

//--------------------------------------------------------------------------------------
// Used to quickly distinguish between different sorts of collidable objects.
//--------------------------------------------------------------------------------------
enum ObjectCategory
{
	CategoryObstacle,  // All obstacles blocking paths
	CategoryEntity,    // All AI-controlled entities
	CategoryObjective, // Game objectives (flags)
	CategoryProjectile // Bullets and other projectiles
};

class CollidableObject : public Object
{
public:
	CollidableObject(void);
	virtual ~CollidableObject(void) = 0;

	bool Initialise(unsigned long id, const XMFLOAT2& position, float rotation, float uniformScale, ObjectCategory, ColliderType colliderType, void* pColliderData);

	void UpdateColliderPosition(const XMFLOAT2& position);

	// Data access functions
	const Collider* GetCollider(void) const;
	ObjectCategory  GetCategory(void) const;

	void SetCategory(ObjectCategory category);

private:
	ObjectCategory m_category;  // The category the object belongs to
	Collider*      m_pCollider;	// The collider associated to the object
};

#endif // COLLIDABLE_OBJECT_H