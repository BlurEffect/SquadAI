/* 
*  Kevin Meergans, SquadAI, 2014
*  RenderContext.h
*  Accumulates and prepares the render data.
*/

// Includes
#include "RenderContext.h"

RenderContext::RenderContext(void)
{
}

RenderContext::~RenderContext(void)
{
}

//--------------------------------------------------------------------------------------
// Adds a new instance of a certain entity type to be rendered.
// Param1: The type of the entity to be rendered.
// Param2: The matrix to transform the entity instance to world space.
//--------------------------------------------------------------------------------------
void RenderContext::AddInstance(ObjectType type, const XMFLOAT4X4& transform)
{
	m_objectInstances[type].push_back(Instance(transform));
}

//--------------------------------------------------------------------------------------
// Deletes the current instance data in preparation for a new frame.
//--------------------------------------------------------------------------------------
void RenderContext::Reset(void)
{
	for(int i = 0; i < NumberOfObjectTypes; ++i)
	{
		m_objectInstances[i].clear();
	}
}

// Data access functions

int RenderContext::GetObjectCount(ObjectType type) const
{
	return m_objectInstances[type].size();
}

const Instance* RenderContext::GetInstances(ObjectType type) const
{
	return &m_objectInstances[type][0];
}


