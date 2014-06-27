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
	virtual void Draw(ID3D11DeviceContext* pDeviceContext);
	virtual void DrawPart(ID3D11DeviceContext* pDeviceContext, UINT indicesToDraw);
	virtual void Cleanup(void);
protected:
	Buffer<Vertex>		     m_vertexBuffer;			// Holds the vertices for this drawable object
	Buffer<unsigned long>    m_indexBuffer;				// Holds the indices for this drawable object
	UINT					 m_vertexCount;				// The number of vertices making up this drawable
	UINT				     m_indexCount;				// The number of indices for this drawable
	D3D11_PRIMITIVE_TOPOLOGY m_primitiveTopology;		// The promitive topology to use for rendering
};

#endif // DRAWABLE_H