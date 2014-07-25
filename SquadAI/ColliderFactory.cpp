/* 
*  Kevin Meergans, SquadAI, 2014
*  ColliderFactory.cpp
*  Class with a static member function serving as factory function 
*  for different types of colliders.
*/

// Includes
#include "ColliderFactory.h"

//--------------------------------------------------------------------------------------
// Dynamically allocates a collider as an identical copy to the collider passed into the function.
// Param1: A pointer to the collider that serves as blueprint for the new one.
// Returns true if the flag was successfully picked up, false otherwise.
//--------------------------------------------------------------------------------------
Collider* ColliderFactory::CreateCollider(Collider* pCollider)
{
	if(!pCollider)
	{
		return nullptr;
	}

	switch(pCollider->GetType())
	{
	case CircleColliderType:
		return new CircleCollider(*(reinterpret_cast<CircleCollider*>(pCollider)));
		break;
	case AxisAlignedRectangleColliderType:
		return new AxisAlignedRectangleCollider(*(reinterpret_cast<AxisAlignedRectangleCollider*>(pCollider)));
		break;
	default:
		return nullptr;
	}
}