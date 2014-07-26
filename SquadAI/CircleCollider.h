/* 
*  Kevin Meergans, SquadAI, 2014
*  CircleCollider.h
*  A circle-shaped collider.
*/

#ifndef CIRCLE_COLLIDER_H
#define CIRCLE_COLLIDER_H

// Includes
#include <DirectXMath.h>
#include "Collider.h"

using namespace DirectX;

//--------------------------------------------------------------------------------------
// Bundles data required for the creation of a circle collider.
//--------------------------------------------------------------------------------------
struct CircleColliderData
{
	CircleColliderData(const XMFLOAT2& centre, float radius) : m_centre(centre),
															   m_radius(radius)
	{}

	XMFLOAT2 m_centre;
	float    m_radius;
};

class CircleCollider : public Collider
{
public:
	CircleCollider(const XMFLOAT2& centre, float radius);
	~CircleCollider(void);

	bool CheckLineCollision(const XMFLOAT2& lineStart, const XMFLOAT2& lineEnd) const;
	bool CheckPointCollision(const XMFLOAT2& point) const;

	// Data access functions
	float GetRadius(void) const;
	void SetRadius(float radius);

private:
	float m_radius;
};

#endif // CIRCLE_COLLIDER_H