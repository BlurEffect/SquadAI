/* 
*  Kevin Meergans, SquadAI, 2014
*  BasicObject.cpp
*  Encapsulates a basic object present in the test environment.
*/

// Includes
#include "BasicObject.h"

BasicObject::BasicObject(unsigned long gridId, ObjectType type, const XMFLOAT2& position, float rotation, float uniformScale)
		: m_gridId(gridId),
		  m_type(type),
		  m_position(position),
		  m_rotation(rotation),
		  m_uniformScale(uniformScale)
{
}

BasicObject::~BasicObject(void)
{
}

// Data access functions

unsigned long BasicObject::GetGridId(void) const
{
	return m_gridId;
}

ObjectType BasicObject::GetType(void) const
{
	return m_type;
}

const XMFLOAT2& BasicObject::GetPosition(void) const
{
	return m_position;
}

float BasicObject::GetRotation(void) const
{
	return m_rotation;
}

float BasicObject::GetUniformScale(void) const
{
	return m_uniformScale;
}

void BasicObject::SetGridId(unsigned long id)
{
	m_gridId = id;
}

void BasicObject::SetType(ObjectType type)
{
	m_type = type;
}

void BasicObject::SetPosition(const XMFLOAT2& position)
{
	m_position = position;
}

void BasicObject::SetRotation(float rotation)
{
	m_rotation = rotation;
}

void BasicObject::SetUniformScale(float uniformScale)
{
	m_uniformScale = uniformScale;
}
