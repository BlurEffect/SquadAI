/* 
*  Kevin Meergans, SquadAI, 2014
*  EditModeObject.cpp
*  Encapsulates a basic object placed in edit mode.
*/

// Includes
#include "EditModeObject.h"

EditModeObject::EditModeObject(void) : Object(),
									   m_gridId(0),
									   m_type(ObjectType(0))
{
}

EditModeObject::~EditModeObject(void)
{
}

//--------------------------------------------------------------------------------------
// Initialises the edit mode object.
// Param1: A unique identifier for the object.
// Param2: The position of the object.
// Param3: The rotation of the object along the world z-axis.
// Param4: The uniform scale of the object.
// Param5: An id identifying the grid field that the object was placed on.
// Param6: The type of object that is represented by this object.
// Returns true if the edit mode object was initialised successfully, false otherwise.
//--------------------------------------------------------------------------------------
bool EditModeObject::Initialise(unsigned long id, const XMFLOAT2& position, float rotation, float uniformScale, unsigned long gridId, ObjectType type)
{
	if(!Object::Initialise(id, position, rotation, uniformScale))
	{
		return false;
	}

	m_gridId = gridId;
	m_type = type;

	return true;
}

// Data access functions

unsigned long EditModeObject::GetGridId(void) const
{
	return m_gridId;
}

ObjectType EditModeObject::GetType(void) const
{
	return m_type;
}

void EditModeObject::SetGridId(unsigned long id)
{
	m_gridId = id;
}

void EditModeObject::SetType(ObjectType type)
{
	m_type = type;
}

