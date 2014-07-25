/* 
*  Kevin Meergans, SquadAI, 2014
*  BasicObject.cpp
*  Encapsulates a basic object present in the test environment.
*/

// Includes
#include "BasicObject.h"

BasicStaticObject::BasicStaticObject(unsigned long gridId, ObjectType type, const XMFLOAT2& position, float rotation, float uniformScale)
		: Object(position, rotation, uniformScale),
		  m_gridId(gridId),
		  m_type(type)
{
}

BasicStaticObject::~BasicStaticObject(void)
{
}

// Data access functions

unsigned long BasicStaticObject::GetGridId(void) const
{
	return m_gridId;
}

ObjectType BasicStaticObject::GetType(void) const
{
	return m_type;
}

void BasicStaticObject::SetGridId(unsigned long id)
{
	m_gridId = id;
}

void BasicStaticObject::SetType(ObjectType type)
{
	m_type = type;
}

