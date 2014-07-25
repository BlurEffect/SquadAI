/* 
*  Kevin Meergans, SquadAI, 2014
*  BasicObject.h
*  Encapsulates a basic object present in the test environment.
*/

#ifndef BASIC_OBJECT_H
#define BASIC_OBJECT_H

// Includes
#include <DirectXMath.h>
#include "EntityData.h"

using namespace DirectX;

class BasicObject
{
public:
	BasicObject(unsigned long gridId, ObjectType type, const XMFLOAT2& position, float rotation, float uniformScale);
	~BasicObject(void);

	// Data access functions

	unsigned long	 GetGridId(void) const;
	ObjectType		 GetType(void) const;
	const XMFLOAT2&	 GetPosition(void) const;
	float			 GetRotation(void) const;
	float	         GetUniformScale(void) const;

	void SetGridId(unsigned long id);
	void SetType(ObjectType type);
	void SetPosition(const XMFLOAT2& position);
	void SetRotation(float rotation);
	void SetUniformScale(float uniformScale);

private:
	unsigned long		  m_gridId;			// An id that identifies the grid field that this object is placed on (multiple objects per field are allowed)
	ObjectType			  m_type;			// The type of this object
	XMFLOAT2              m_position;		// The world position of this object
	float                 m_rotation;		// The rotation of this object along the world z-axis
	float                 m_uniformScale;	// The uniform scale to apply to this object
};

#endif // BASIC_OBJECT_H