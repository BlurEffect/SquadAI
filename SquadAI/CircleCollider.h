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

class CircleCollider : public Collider
{
public:
	CircleCollider(const XMFLOAT2& centre, float radius);
	~CircleCollider(void);

	bool CheckLineCollision(const XMFLOAT2& lineStart, const XMFLOAT2& lineEnd) const;

	// Data access functions
	float GetRadius(void) const;
	void SetRadius(float radius);

private:
	float m_radius;
};

#endif // CIRCLE_COLLIDER_H