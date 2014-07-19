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

class AxisAlignedRectangleCollider : public Collider
{
public:
	AxisAlignedRectangleCollider(const XMFLOAT2& centre, float width, float height);
	~AxisAlignedRectangleCollider(void);

	bool CheckLineCollision(const XMFLOAT2& lineStart, const XMFLOAT2& lineEnd) const;

	// Data access functions
	float GetWidth(void) const;
	float GetHeight(void) const;
	void SetWidth(float width);
	void SetHeight(float height);

private:

	bool CheckLineSegmentsIntersection(const XMFLOAT2& line1Start, const XMFLOAT2& line1End, const XMFLOAT2& line2Start, const XMFLOAT2& line2End) const;

	float m_width;
	float m_height;
};

#endif // AXIS_ALIGNED_RECTANGLE_COLLIDER_H