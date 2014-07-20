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
	if(lineStart.x == lineEnd.x && lineStart.y == lineEnd.y)
	{
		// If the points are identical, check whether that shared point lies within the collider.
		return CheckPointCollision(lineStart);
	}

	XMVECTOR lineSegment = XMLoadFloat2(&lineEnd) - XMLoadFloat2(&lineStart);
	XMVECTOR startToCollider = XMLoadFloat2(&GetCentre()) - XMLoadFloat2(&lineStart);

	float segmentLength = 0.0f;
	XMStoreFloat(&segmentLength, XMVector2Length(lineSegment));

	XMVECTOR normLineSegment = lineSegment / segmentLength;

	float projection = 0.0f;
	XMStoreFloat(&projection, XMVector2Dot(startToCollider, normLineSegment));

	if((projection < 0.0f) || (projection > segmentLength))
	{
		// Projection is beyond the start or end point of the segment
		return false;
	}

	// Get the projected point
	XMVECTOR projectedPoint = XMLoadFloat2(&lineStart) + normLineSegment * projection;

	// Check if the projected point lies within the radius of the collider
	float squareRadius = GetRadius() * GetRadius();
	float squareDistance = 0.0f;
	XMStoreFloat(&squareDistance, XMVector2Dot(XMLoadFloat2(&GetCentre()) - projectedPoint, XMLoadFloat2(&GetCentre()) - projectedPoint));

	return squareRadius >= squareDistance;

}

//--------------------------------------------------------------------------------------
// Checks for collision of a point with this collider.
// Param1: The point to test.
// Returns true if the point is within the collider, that includes touching its outer line,
// false otherwise.
//--------------------------------------------------------------------------------------
bool CircleCollider::CheckPointCollision(const XMFLOAT2& point) const
{
	float squareRadius = GetRadius() * GetRadius();

	float squareDistance = 0.0f;
	XMStoreFloat(&squareDistance, XMVector2LengthSq(XMLoadFloat2(&GetCentre()) - XMLoadFloat2(&point)));

	return squareRadius >= squareDistance;
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