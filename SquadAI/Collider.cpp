/* 
*  Kevin Meergans, SquadAI, 2014
*  Collider.cpp
*  Abstract base class for colliders that can be attached to entities in order
*  to detect collisions.
*/

// Includes
#include "Collider.h"

Collider::Collider(ColliderType type, const XMFLOAT2& centre) : m_type(type),
																m_centre(centre)
{
}

Collider::~Collider(void)
{
}

// Data access functions

const XMFLOAT2& Collider::GetCentre(void) const
{
	return m_centre;
}

ColliderType Collider::GetType(void) const
{
	return m_type;
}

void Collider::SetCentre(const XMFLOAT2& centre)
{
	m_centre = centre;
}
