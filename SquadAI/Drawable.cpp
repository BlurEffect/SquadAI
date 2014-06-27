/* 
*  Kevin Meergans, SquadAI, 2014
*  Drawable.cpp
*  Abstract base class for drawable objects providing some default implementations.
*/

// Includes
#include "Drawable.h"

//--------------------------------------------------------------------------------------
// Default constructor.
//--------------------------------------------------------------------------------------
Drawable::Drawable(void) : m_vertexCount(0),
						   m_indexCount(0),
						   m_primitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST)
{
}

//--------------------------------------------------------------------------------------
// Destructor.
//--------------------------------------------------------------------------------------
Drawable::~Drawable(void)
{
	Cleanup();
}

//--------------------------------------------------------------------------------------
// Pure virtual initialisation function to be implemented by derived classes in order to
// set up specific meshes.
// Param1: A pointer to the device that should be used for the creation of this Drawable.
// Returns true if the initialisation succeeded, false otherwise.
//--------------------------------------------------------------------------------------
//bool Initialise(ID3D11Device* pDevice);

//--------------------------------------------------------------------------------------
// Renders the Drawable.
// Param1: The device context to use for rendering.
//--------------------------------------------------------------------------------------
void Drawable::Draw(ID3D11DeviceContext* pDeviceContext)
{
	DrawPart(pDeviceContext, m_indexCount);
}

//--------------------------------------------------------------------------------------
// Renders a part of the Drawable specified by the number of indices passed in.
// Param1: The number of indices to draw for the Drawable.
//--------------------------------------------------------------------------------------
void Drawable::DrawPart(ID3D11DeviceContext* pDeviceContext, UINT indicesToDraw)
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
void Drawable::Cleanup(void)
{
	// Release all the buffers associated to this Drawable.

	m_vertexBuffer.Cleanup();
	m_indexBuffer.Cleanup();
}