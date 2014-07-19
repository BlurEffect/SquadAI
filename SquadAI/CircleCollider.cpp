/* 
*  Kevin Meergans, SquadAI, 2014
*  CircleCollider.cpp
*  A circle-shaped collider.
*/

// Includes
#include "CircleCollider.h"

CircleCollider::CircleCollider(const XMFLOAT2& centre, float radius) : Collider(centre),
																	   m_radius(radius)
{
}

CircleCollider::~CircleCollider(void)
{
}

//--------------------------------------------------------------------------------------
// Checks for collision of a line with this collider.
// Param1: The start point of the line.
// Param2: The end point of the line.
// Returns true if the line intersects with the collider, that includes touching it and
// being fully encompassed by it, false otherwise.
//--------------------------------------------------------------------------------------
bool CircleCollider::CheckLineCollision(const XMFLOAT2& lineStart, const XMFLOAT2& lineEnd) const
{
	float distance = 0.0f;
	XMStoreFloat(&distance, XMVector2LinePointDistance(XMLoadFloat2(&lineStart), XMLoadFloat2(&lineEnd), XMLoadFloat2(&GetCentre())));

	return GetRadius() >= distance;
}

// Data access functions

float CircleCollider::GetRadius(void) const
{
	return m_radius;
}

void CircleCollider::SetRadius(float radius)
{
	m_radius = radius;
}