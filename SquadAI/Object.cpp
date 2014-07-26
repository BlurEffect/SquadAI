/* 
*  Kevin Meergans, SquadAI, 2014
*  Object.cpp
*  Abstract base class for objects in the test environment.
*/

// Includes
#include "Object.h"

Object::Object(void) : m_position(0.0f, 0.0f),
					   m_rotation(0.0f),
					   m_uniformScale(1.0f)
{
}

Object::~Object(void)
{
}

//--------------------------------------------------------------------------------------
// Initialises the object.
// Param1: The position of the object.
// Param2: The rotation of the object along the world z-axis.
// Param3: The uniform scale of the object.
// Returns true if the object was initialised successfully, false otherwise.
//--------------------------------------------------------------------------------------
bool Object::Initialise(const XMFLOAT2& position, float rotation, float uniformScale)
{
	m_position = position;
	m_rotation = rotation;
	m_uniformScale = uniformScale;

	return true;
}

// Data access functions

const XMFLOAT2& Object::GetPosition(void) const
{
	return m_position;
}

float Object::GetRotation(void) const
{
	return m_rotation;
}

float Object::GetUniformScale(void) const
{
	return m_uniformScale;
}

void Object::SetPosition(const XMFLOAT2& position)
{
	m_position = position;
}

void Object::SetRotation(float rotation)
{
	m_rotation = rotation;
}

void Object::SetUniformScale(float uniformScale)
{
	m_uniformScale = uniformScale;
}
