/* 
*  Kevin Meergans, SquadAI, 2014
*  RenderContext.h
*  Accumulates and prepares the render data.
*/

#ifndef RENDER_CONTEXT_H
#define RENDER_CONTEXT_H

// Includes
#include <vector>
#include "RendererData.h"
#include "VertexData.h"

class RenderContext
{
public:
	RenderContext(void);
	~RenderContext(void);

	void AddInstance(EntityType type, const XMFLOAT4X4& transform);
	void Reset(void);

	// Data access functions

	int             GetDrawableCount(DrawableType type) const;
	const Instance* GetInstances(DrawableType type) const;		

private:
	std::vector<Instance> m_drawableInstances[NumberOfDrawableTypes]; // Contains the instances for each drawable type to be
																	  // rendered this frame
};

#endif // RENDER_CONTEXT_H