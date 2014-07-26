/* 
*  Kevin Meergans, SquadAI, 2014
*  AxisAlignedRectangleCollider.h
*  A rectangle-shaped collider whose edges are aligned to world axes.
*/

#ifndef AXIS_ALIGNED_RECTANGLE_COLLIDER_H
#define AXIS_ALIGNED_RECTANGLE_COLLIDER_H

// Includes
#include <DirectXMath.h>
#include <algorithm>
#include "Collider.h"

using namespace DirectX;

//--------------------------------------------------------------------------------------
// Bundles data required for the creation of an axis-aligned rectangle collider.
//--------------------------------------------------------------------------------------
struct AxisAlignedRectangleColliderData
{
	AxisAlignedRectangleColliderData(const XMFLOAT2& centre, float width, float height) : m_centre(centre),
																						  m_width(width),
																						  m_height(height)
	{}

	XMFLOAT2 m_centre;
	float m_width;
	float m_height;
};

class AxisAlignedRectangleCollider : public Collider
{
public:
	AxisAlignedRectangleCollider(const XMFLOAT2& centre, float width, float height);
	~AxisAlignedRectangleCollider(void);

	bool CheckLineCollision(const XMFLOAT2& lineStart, const XMFLOAT2& lineEnd) const;
	bool CheckPointCollision(const XMFLOAT2& point) const;

	// Data access functions
	float GetWidth(void) const;
	float GetHeight(void) const;
	void SetWidth(float width);
	void SetHeight(float height);

private:

	bool CheckLineSegmentsIntersection(const XMFLOAT2& line1Start, const XMFLOAT2& line1End, const XMFLOAT2& line2Start, const XMFLOAT2& line2End) const;

	float	 m_width;		// The width of the rectangle making up the collider
	float	 m_height;		// The height of the rectangle making up the collider

	XMFLOAT2 m_topLeft;     // The upper left corner of the rectangle
	XMFLOAT2 m_topRight;	// The upper right corner of the rectangle
	XMFLOAT2 m_bottomLeft;	// The lower left corner of the rectangle
	XMFLOAT2 m_bottomRight;	// The lower right corner of the rectangle
};

#endif // AXIS_ALIGNED_RECTANGLE_COLLIDER_H