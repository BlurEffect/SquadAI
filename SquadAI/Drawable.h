/* 
*  Kevin Meergans, SquadAI, 2014
*  Drawable.h
*  Abstract base class for drawable objects providing some default implementations.
*  The class is templated to allow for all kinds of different vertex types.
*/

#ifndef DRAWABLE_H
#define DRAWABLE_H

// Includes
#include <d3d11.h>
#include "VertexData.h"
#include "Buffer.h"

template <class VertexType>
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
	Buffer<VertexType>		 m_vertexBuffer;			// Holds the vertices for this drawable object
	Buffer<unsigned long>    m_indexBuffer;				// Holds the indices for this drawable object
	UINT					 m_vertexCount;				// The number of vertices making up this drawable
	UINT				     m_indexCount;				// The number of indices for this drawable
	D3D11_PRIMITIVE_TOPOLOGY m_primitiveTopology;		// The promitive topology to use for rendering
};

// Implementations of the template class functions

//--------------------------------------------------------------------------------------
// Default constructor.
//--------------------------------------------------------------------------------------
template <class VertexType>
Drawable<VertexType>::Drawable(void) : m_vertexCount(0),
						               m_indexCount(0),
						               m_primitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST)
{
}

//--------------------------------------------------------------------------------------
// Destructor.
//--------------------------------------------------------------------------------------
template <class VertexType>
Drawable<VertexType>::~Drawable(void)
{
	Cleanup();
}

//--------------------------------------------------------------------------------------
// Pure virtual initialisation function to be implemented by derived classes in order to
// set up specific meshes.
// Param1: A pointer to the device that should be used for the creation of this Drawable.
// Returns true if the initialisation succeeded, false otherwise.
//--------------------------------------------------------------------------------------
// template <class VertexType>
// bool Initialise<vertexType>(ID3D11Device* pDevice);

//--------------------------------------------------------------------------------------
// Renders the Drawable.
// Param1: The device context to use for rendering.
//--------------------------------------------------------------------------------------
template <class VertexType>
void Drawable<VertexType>::Draw(ID3D11DeviceContext* pDeviceContext)
{
	DrawPart(pDeviceContext, m_indexCount);
}

//--------------------------------------------------------------------------------------
// Renders a part of the Drawable specified by the number of indices passed in.
// Param1: The number of indices to draw for the Drawable.
//--------------------------------------------------------------------------------------
template <class VertexType>
void Drawable<VertexType>::DrawPart(ID3D11DeviceContext* pDeviceContext, UINT indicesToDraw)
{
	// Set the vertex buffer to active in the input assembler so it can be rendered.
	pDeviceContext->IASetVertexBuffers(0, 1, m_vertexBuffer.GetBuffer(), m_vertexBuffer.GetStride(), m_vertexBuffer.GetOffset());

	// Set the index buffer to active in the input assembler so it can be rendered.
	pDeviceContext->IASetIndexBuffer(*m_indexBuffer.GetBuffer(), DXGI_FORMAT_R32_UINT, 0);

	// Set the type of primitive that should be rendered from this vertex buffer.
	pDeviceContext->IASetPrimitiveTopology(m_primitiveTopology);

	pDeviceContext->DrawIndexed(indicesToDraw, 0, 0);
}

//--------------------------------------------------------------------------------------
// Releases resources.
//--------------------------------------------------------------------------------------
template <class VertexType>
void Drawable<VertexType>::Cleanup(void)
{
	// Release all the buffers associated to this Drawable.

	m_vertexBuffer.Cleanup();
	m_indexBuffer.Cleanup();
}

#endif // DRAWABLE_H