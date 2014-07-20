/* 
*  Kevin Meergans, SquadAI, 2014
*  AxisAlignedRectangleCollider.h
*  A square-shaped collider whose edges are aligned to world axes.
*/

// Includes
#include "AxisAlignedRectangleCollider.h"

AxisAlignedRectangleCollider::AxisAlignedRectangleCollider(const XMFLOAT2& centre, float width, float height) : Collider(AxisAlignedRectangleColliderType, centre),
																								                m_width(width),
																												m_height(height)
{
	// Define the four corners of the rectangle
	m_topLeft	  = XMFLOAT2(GetCentre().x - GetWidth() * 0.5f, GetCentre().y + GetHeight() * 0.5f);
	m_topRight	  = XMFLOAT2(GetCentre().x + GetWidth() * 0.5f, GetCentre().y + GetHeight() * 0.5f);
	m_bottomLeft  = XMFLOAT2(GetCentre().x - GetWidth() * 0.5f, GetCentre().y - GetHeight() * 0.5f);
	m_bottomRight = XMFLOAT2(GetCentre().x + GetWidth() * 0.5f, GetCentre().y - GetHeight() * 0.5f);
}

AxisAlignedRectangleCollider::~AxisAlignedRectangleCollider(void)
{
}

//--------------------------------------------------------------------------------------
// Checks for collision of a line with this collider.
// Param1: The start point of the line.
// Param2: The end point of the line.
// Returns true if the line intersects with the collider, that includes touching it and
// being fully encompassed by it, false otherwise.
//--------------------------------------------------------------------------------------
bool AxisAlignedRectangleCollider::CheckLineCollision(const XMFLOAT2& lineStart, const XMFLOAT2& lineEnd) const
{
	// Determine intersections of the four lines making up the rectangle with the passed in line

	// Check if the line is fully encompassed by the rectangle (if so the tests conducted below won't
	// register an intersection).
	if(CheckPointCollision(lineStart) && CheckPointCollision(lineEnd))
	{
		// Both line segment end points are encompassed by the rectangle
		return true;
	}

	// Check the sides of the rectangle

	return (CheckLineSegmentsIntersection(lineStart, lineEnd, m_bottomLeft, m_topLeft) ||     // left side
			CheckLineSegmentsIntersection(lineStart, lineEnd, m_topLeft, m_topRight) ||       // top
			CheckLineSegmentsIntersection(lineStart, lineEnd, m_bottomRight, m_topRight) ||   // right
			CheckLineSegmentsIntersection(lineStart, lineEnd, m_bottomLeft, m_bottomRight));  // bottom

}

//--------------------------------------------------------------------------------------
// Checks for collision of a point with this collider.
// Param1: The point to test.
// Returns true if the point is within the collider, that includes touching its outer line,
// false otherwise.
//--------------------------------------------------------------------------------------
bool AxisAlignedRectangleCollider::CheckPointCollision(const XMFLOAT2& point) const
{
	return point.x >= m_bottomLeft.x && point.y >= m_bottomLeft.y && point.x <= m_bottomRight.x && point.y <= m_topLeft.y;
}

//--------------------------------------------------------------------------------------
// Checks for intersection between two line segments.
// Param1: The start point of the first line.
// Param2: The end point of the first line.
// Param3: The start point of the second line.
// Param4: The end point of the second line.
// Returns true if the two line segments intersect, inluding coincidence, false otherwise
//--------------------------------------------------------------------------------------
bool AxisAlignedRectangleCollider::CheckLineSegmentsIntersection(const XMFLOAT2& line1Start, const XMFLOAT2& line1End, const XMFLOAT2& line2Start, const XMFLOAT2& line2End) const
{
	// Get the intersection point
	XMVECTOR intersection = XMVector2IntersectLine(XMLoadFloat2(&line1Start), XMLoadFloat2(&line1End), XMLoadFloat2(&line2Start), XMLoadFloat2(&line2End));  

	// The intersection vector is NaN if the lines are parallel
	if(!XMVector2IsNaN(intersection))
	{
		// The intersection vector is infinited when the lines are coincident (this counts as collision)
		if(!XMVector2IsInfinite(intersection))
		{
			XMFLOAT2 intersectionPoint;
			XMStoreFloat2(&intersectionPoint, intersection);

			// Check if the intersection point lies within both segments.
			// Checking the x value is sufficient as it is already guaranteed that the point lies on the lines.
			std::pair<float, float> line1XMinMax = std::minmax<float>(line1Start.x, line1End.x);
			std::pair<float, float> line2XMinMax = std::minmax<float>(line2Start.x, line2End.x);
			std::pair<float, float> line1YMinMax = std::minmax<float>(line1Start.y, line1End.y);
			std::pair<float, float> line2YMinMax = std::minmax<float>(line2Start.y, line2End.y);
			if((line1XMinMax.first <= intersectionPoint.x  && intersectionPoint.x <= line1XMinMax.second) &&
			   (line2XMinMax.first <= intersectionPoint.x  && intersectionPoint.x <= line2XMinMax.second) &&
			   (line1YMinMax.first <= intersectionPoint.y  && intersectionPoint.y <= line1YMinMax.second) &&
			   (line2YMinMax.first <= intersectionPoint.y  && intersectionPoint.y <= line2YMinMax.second))
			{
				return true;
			}
		}else
		{
			// The lines are coincident, check if the segments overlap.
			// Checking the x values is sufficient.
			std::pair<float, float> line1MinMax = std::minmax<float>(line1Start.x, line1End.x);
			if((line1MinMax.first <= line2Start.x  && line2Start.x <= line1MinMax.second) ||
			   (line1MinMax.first <= line2End.x  && line2End.x <= line1MinMax.second))
			{
				// The segments overlap
				return true;
			}
		}
	}

	return false;
}

// Data access functions

float AxisAlignedRectangleCollider::GetWidth(void) const
{
	return m_width;
}

float AxisAlignedRectangleCollider::GetHeight(void) const
{
	return m_height;
}

void AxisAlignedRectangleCollider::SetWidth(float width)
{
	m_width = width;
}

void AxisAlignedRectangleCollider::SetHeight(float height)
{
	m_height = height;
}
