/* 
*  Kevin Meergans, SquadAI, 2014
*  CollidableObject.cpp
*  Abstract base class for objects that can collide with others in the
*  test environment.
*/

// Includes
#include "CollidableObject.h"

CollidableObject::CollidableObject(void) : Object(),
								           m_category(ObjectCategory(0)),
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
// Param1: A unique identifier for the object.
// Param2: The position of the object.
// Param3: The rotation of the object along the world z-axis.
// Param4: The uniform scale of the object.
// Param5: The category the object belongs to.
// Param6: The type of the collider that should be created.
// Param7: A pointer to the initialisation data for the collider.
// Returns true if the collidable object was initialised successfully, false otherwise.
//--------------------------------------------------------------------------------------
bool CollidableObject::Initialise(unsigned long id, const XMFLOAT2& position, float rotation, float uniformScale, ObjectCategory category, ColliderType colliderType, void* pColliderData)
{
	if(!Object::Initialise(id, position, rotation, uniformScale))
	{
		return false;
	}

	m_category  = category;
	m_pCollider = ColliderFactory::CreateCollider(colliderType, pColliderData);

	if(!m_pCollider)
	{
		return false;
	}

	return true;
}

//--------------------------------------------------------------------------------------
// Updates the position of the collider. Should be called each time the position of the 
// objects is changed.
// Param1: The new position of the collider.
//--------------------------------------------------------------------------------------
void CollidableObject::UpdateColliderPosition(const XMFLOAT2& position)
{
	m_pCollider->SetCentre(position);
}

// Data access functions

const Collider* CollidableObject::GetCollider(void) const
{
	return m_pCollider;
}

ObjectCategory CollidableObject::GetCategory(void) const
{
	return m_category;
}

void CollidableObject::SetCategory(ObjectCategory category)
{
	m_category = category;
}
