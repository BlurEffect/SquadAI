/* 
*  Kevin Meergans, SquadAI, 2014
*  ColliderFactory.h
*  Class with a static member function serving as factory function 
*  for different types of colliders.
*/

#ifndef COLLIDER_FACTORY_H
#define COLLIDER_FACTORY_H

// Includes
#include "Collider.h"
#include "CircleCollider.h"
#include "AxisAlignedRectangleCollider.h"

class ColliderFactory
{
public:
	static Collider* CreateCollider(ColliderType type, void* pColliderData);

};

#endif // COLLIDER_FACTORY_H