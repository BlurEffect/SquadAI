/* 
*  Kevin Meergans, SquadAI, 2014
*  EditModeObject.h
*  Encapsulates a basic object placed in edit mode.
*/

#ifndef EDIT_MODE_OBJECT_H
#define EDIT_MODE_OBJECT_H

// Includes
#include <DirectXMath.h>
#include "Object.h"
#include "EntityData.h"

using namespace DirectX;

class EditModeObject : public Object
{
public:
	EditModeObject(void);
	~EditModeObject(void);

	bool Initialise(unsigned long id, const XMFLOAT2& position, float rotation, float uniformScale, unsigned long gridId, ObjectType type);

	// Data access functions

	unsigned long	 GetGridId(void) const;
	ObjectType		 GetType(void) const;

	void SetGridId(unsigned long id);
	void SetType(ObjectType type);

private:
	unsigned long		  m_gridId;			// An id that identifies the grid field that this object is placed on (multiple objects per field are allowed)
	ObjectType			  m_type;			// The type of this object
};

#endif // EDIT_MODE_OBJECT_H