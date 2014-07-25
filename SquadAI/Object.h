/* 
*  Kevin Meergans, SquadAI, 2014
*  Object.h
*  Abstract base class for objects in the test environment.
*/

#ifndef OBJECT_H
#define OBJECT_H

// Includes
#include <DirectXMath.h>
#include "EntityData.h"

using namespace DirectX;

class Object
{
public:
	Object(void);
	Object(const XMFLOAT2& position, float rotation, float uniformScale);
	virtual ~Object(void) = 0;

	// Data access functions

	const XMFLOAT2&	 GetPosition(void) const;
	float			 GetRotation(void) const;
	float	         GetUniformScale(void) const;

	void SetPosition(const XMFLOAT2& position);
	void SetRotation(float rotation);
	void SetUniformScale(float uniformScale);

private:

	XMFLOAT2              m_position;		// The world position of this object
	float                 m_rotation;		// The rotation of this object along the world z-axis
	float                 m_uniformScale;	// The uniform scale to apply to this object
};

#endif // OBJECT_H