/* 
*  Kevin Meergans, SquadAI, 2014
*  RenderContext.h
*  Accumulates and prepares the render data.
*/

#ifndef RENDER_CONTEXT_H
#define RENDER_CONTEXT_H

// Includes
#include <vector>
#include "EntityData.h"
#include "VertexData.h"

class RenderContext
{
public:
	RenderContext(void);
	~RenderContext(void);

	void AddInstance(ObjectType type, const XMFLOAT4X4& transform);
	void Reset(void);

	// Data access functions

	int             GetObjectCount(ObjectType type) const;
	const Instance* GetInstances(ObjectType type) const;		

private:
	std::vector<Instance> m_objectInstances[NumberOfObjectTypes]; // Contains the instances for each drawable type to be
																  // rendered this frame
};

#endif // RENDER_CONTEXT_H