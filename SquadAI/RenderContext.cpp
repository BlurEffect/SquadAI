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
void RenderContext::AddInstance(EntityType type, const XMFLOAT4X4& transform)
{
	m_entityInstances[type].push_back(Instance(transform));
}

//--------------------------------------------------------------------------------------
// Deletes the current instance data in preparation for a new frame.
//--------------------------------------------------------------------------------------
void RenderContext::Reset(void)
{
	for(int i = 0; i < NumberOfEntityTypes; ++i)
	{
		m_entityInstances[i].clear();
	}
}

// Data access functions

int RenderContext::GetEntityCount(EntityType type) const
{
	return m_entityInstances[type].size();
}

const Instance* RenderContext::GetInstances(EntityType type) const
{
	return &m_entityInstances[type][0];
}


