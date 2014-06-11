/* 
*  Kevin Meergans, SquadAI, 2014
*  Drawable.h
*  Abstract base class for drawable objects providing some default implementations.
*/

#ifndef DRAWABLE_H
#define DRAWABLE_H

// Includes
#include <d3d11.h>
#include "VertexData.h"
#include "Buffer.h"


class Drawable
{
public:
	Drawable(void);
	virtual ~Drawable(void);
	virtual bool Initialise(ID3D11Device* pDevice) = 0;
	virtual bool InitialiseInstancing(ID3D11Device* pDevice, D3D11_USAGE usage, Instance* pInstances, UINT maxNumberOfInstances);
	virtual void Draw(ID3D11DeviceContext* pDeviceContext);
	virtual bool DrawInstanced(ID3D11DeviceContext* pDeviceContext, Instance* pInstances, UINT instanceCount, UINT offset);
	virtual void Cleanup(void);
protected:
	Buffer<Vertex>		     m_vertexBuffer;			// Holds the vertices for this drawable object
	Buffer<unsigned long>    m_indexBuffer;				// Holds the indices for this drawable object
	Buffer<Instance>	     m_instanceBuffer;			// Holds the instance data for this drawable object (only used for instanced drawing)
	int					     m_vertexCount;				// The number of vertices making up this drawable
	int					     m_indexCount;				// The number of indices for this drawable
	int					     m_maxNumberOfInstances;	// The maximal number of instances for this Drawable
	bool				     m_instancingSetUp;			// Specifies whether instancing was set up properly for this Drawable
	D3D11_PRIMITIVE_TOPOLOGY m_primitiveTopology;		// The promitive topology to use for rendering
};

#endif // DRAWABLE_H