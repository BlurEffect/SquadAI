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
// Param1: The type of the collider that should be created.
// Param2: A pointer to the initialisation data for the collider.
// Returns true if the flag was successfully picked up, false otherwise.
//--------------------------------------------------------------------------------------
Collider* ColliderFactory::CreateCollider(ColliderType type, void* pColliderData)
{
	if(!pColliderData)
	{
		return nullptr;
	}

	switch(type)
	{
	case CircleColliderType:
		{
		CircleColliderData* pData = reinterpret_cast<CircleColliderData*>(pColliderData);
		return new CircleCollider(pData->m_centre, pData->m_radius);
		break;
		}
	case AxisAlignedRectangleColliderType:
		{
		AxisAlignedRectangleColliderData* pData = reinterpret_cast<AxisAlignedRectangleColliderData*>(pColliderData);
		return new AxisAlignedRectangleCollider(pData->m_centre, pData->m_width, pData->m_height);
		break;
		}
	default:
		return nullptr;
	}
}