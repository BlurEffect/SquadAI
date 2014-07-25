/* 
*  Kevin Meergans, SquadAI, 2014
*  BasicObject.h
*  Encapsulates a basic object present in the test environment.
*/

#ifndef BASIC_OBJECT_H
#define BASIC_OBJECT_H

// Includes
#include <DirectXMath.h>
#include "Object.h"
#include "EntityData.h"

using namespace DirectX;

class BasicStaticObject : public Object
{
public:
	BasicStaticObject(unsigned long gridId, ObjectType type, const XMFLOAT2& position, float rotation, float uniformScale);
	~BasicStaticObject(void);

	// Data access functions

	unsigned long	 GetGridId(void) const;
	ObjectType		 GetType(void) const;

	void SetGridId(unsigned long id);
	void SetType(ObjectType type);

private:
	unsigned long		  m_gridId;			// An id that identifies the grid field that this object is placed on (multiple objects per field are allowed)
	ObjectType			  m_type;			// The type of this object
};

#endif // BASIC_OBJECT_H