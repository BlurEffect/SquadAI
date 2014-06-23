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
	m_drawableInstances[g_cDrawableMappings[type]].push_back(Instance(transform, g_cColourMappings[type]));
}

//--------------------------------------------------------------------------------------
// Deletes the current instance data in preparation for a new frame.
//--------------------------------------------------------------------------------------
void RenderContext::Reset(void)
{
	for(int i = 0; i < NumberOfDrawableTypes; ++i)
	{
		m_drawableInstances[i].clear();
	}
}

// Data access functions

int RenderContext::GetDrawableCount(DrawableType type) const
{
	return m_drawableInstances[type].size();
}

const Instance* RenderContext::GetInstances(DrawableType type) const
{
	return &m_drawableInstances[type][0];
}


