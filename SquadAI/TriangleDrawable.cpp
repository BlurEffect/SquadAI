/* 
*  Kevin Meergans, SquadAI, 2014
*  TriangleDrawable.cpp
*  Drawable that encapsulates a triangle-shaped mesh.
*/

// Includes
#include "TriangleDrawable.h"

TriangleDrawable::TriangleDrawable(void) : Drawable()
{
}

TriangleDrawable::~TriangleDrawable(void)
{
}

//--------------------------------------------------------------------------------------
// Initialise the Triangle Drawable.
// Param1: A pointer to the device that should be used for the creation of this Drawable.
// Returns true if the initialisation succeeded, false otherwise.
//--------------------------------------------------------------------------------------
bool TriangleDrawable::Initialise(ID3D11Device* pDevice)
{
	// Set the vertex and index count for this Drawable

	m_vertexCount = 3;
	m_indexCount  = 3;

	// Create temporary vertex and index arrays

	Vertex* pVertices = new Vertex[m_vertexCount];
	if(!pVertices)
	{
		return false;
	}

	unsigned long* pIndices = new unsigned long[m_indexCount];
	if(!pIndices)
	{
		return false;
	}

	// Define vertices and indices for the Drawable

	pVertices[0].m_position = XMFLOAT3(-0.5f, -0.5f, 0.0f);
	pVertices[1].m_position = XMFLOAT3(0.0f, 0.5f, 0.0f);
	pVertices[2].m_position = XMFLOAT3(0.5f, -0.5f, 0.0f);

	pIndices[0] = 0;
	pIndices[1] = 1;
	pIndices[2] = 2;

	// Create and initialise the vertex and index buffers

	m_vertexBuffer.Initialise(VertexBuffer, D3D11_USAGE_IMMUTABLE, pVertices, m_vertexCount);
	m_indexBuffer.Initialise(IndexBuffer, D3D11_USAGE_IMMUTABLE, pIndices, m_indexCount);

	// Release the temporary arrays
	delete [] pVertices;
	pVertices = nullptr;

	delete [] pIndices;
	pIndices = nullptr;
}