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
#include "ObjectTypes.h"

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

	//--------------------------------------------------------------------------------------
	// Functor used to find a basic object within a container based on the id of the
	// grid field it is placed on.
	//--------------------------------------------------------------------------------------
	class FindEditModeObjectByGridId
	{
	public:
		FindEditModeObjectByGridId(unsigned long gridId) : m_gridId(gridId){}
		bool operator()(const EditModeObject& basicObject)
		{
			return basicObject.GetGridId() == m_gridId;
		}
	private:
		unsigned long m_gridId;
	};

private:
	unsigned long m_gridId;			// An id that identifies the grid field that this object is placed on (multiple objects per field are allowed)
	ObjectType    m_type;			// The type of this object
};

#endif // EDIT_MODE_OBJECT_H