/* 
*  Kevin Meergans, SquadAI, 2014
*  CollidableObject.cpp
*  Abstract base class for objects that can collide with others in the
*  test environment.
*/

// Includes
#include "CollidableObject.h"

CollidableObject::CollidableObject(void) : Object(),
										   m_pCollider(nullptr)
{
}

CollidableObject::~CollidableObject(void)
{
	if(m_pCollider)
	{
		delete m_pCollider;
		m_pCollider = nullptr;
	}
}

//--------------------------------------------------------------------------------------
// Initialises the collidable object.
// Param1: The position of the object.
// Param2: The rotation of the object along the world z-axis.
// Param3: The uniform scale of the object.
// Param4: The type of the collider that should be created.
// Param5: A pointer to the initialisation data for the collider.
// Returns true if the collidable object was initialised successfully, false otherwise.
//--------------------------------------------------------------------------------------
bool CollidableObject::Initialise(const XMFLOAT2& position, float rotation, float uniformScale, ColliderType colliderType, void* pColliderData)
{
	if(!Object::Initialise(position, rotation, uniformScale))
	{
		return false;
	}

	m_pCollider = ColliderFactory::CreateCollider(colliderType, pColliderData);

	if(!m_pCollider)
	{
		return false;
	}

	return true;
}

// Data access functions

const Collider* CollidableObject::GetCollider(void) const
{
	return m_pCollider;
}
