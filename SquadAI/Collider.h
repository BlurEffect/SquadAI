/* 
*  Kevin Meergans, SquadAI, 2014
*  Collider.h
*  Abstract base class for colliders that can be attached to entities in order
*  to detect collisions.
*/

#ifndef COLLIDER_H
#define COLLIDER_H

// Includes
#include <DirectXMath.h>

using namespace DirectX;

//--------------------------------------------------------------------------------------
// Contains identifiers for the available collider types that can be used on the entities.
//--------------------------------------------------------------------------------------
enum ColliderType
{
	CircleColliderType,
	AxisAlignedRectangleColliderType,
};

class Collider
{
public:
	Collider(ColliderType type, const XMFLOAT2& centre);
	virtual ~Collider(void);

	virtual bool CheckLineCollision(const XMFLOAT2& lineStart, const XMFLOAT2& lineEnd) const = 0;
	virtual bool CheckPointCollision(const XMFLOAT2& point) const = 0;

	// Data access functions
	const XMFLOAT2& GetCentre(void) const;
	ColliderType	GetType(void) const;
	void SetCentre(const XMFLOAT2& centre);

private:
	ColliderType m_type;
	XMFLOAT2     m_centre;
};

#endif // COLLIDER_H