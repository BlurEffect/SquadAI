/* 
*  Kevin Meergans, SquadAI, 2014
*  SquareDrawable.cpp
*  Drawable that encapsulates a square-shaped mesh.
*/

// Includes
#include "SquareDrawable.h"

SquareDrawable::SquareDrawable(float length) : Drawable(),
									           m_length(length)
{
}

SquareDrawable::~SquareDrawable(void)
{
}

//--------------------------------------------------------------------------------------
// Initialise the Square Drawable.
// Param1: A pointer to the device that should be used for the creation of this Drawable.
// Returns true if the initialisation succeeded, false otherwise.
//--------------------------------------------------------------------------------------
bool SquareDrawable::Initialise(ID3D11Device* pDevice)
{
	// Set the vertex and index count for this Drawable

	m_vertexCount = 4;
	m_indexCount  = 6;

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

	pVertices[0].m_position = XMFLOAT3(-m_length * 0.5f, -m_length * 0.5f, 0.0f);
	pVertices[1].m_position = XMFLOAT3(-m_length * 0.5f, m_length * 0.5f, 0.0f);
	pVertices[2].m_position = XMFLOAT3(m_length * 0.5f, -m_length * 0.5f, 0.0f);
	pVertices[3].m_position = XMFLOAT3(m_length * 0.5f, m_length * 0.5f, 0.0f);

	pIndices[0] = 0;
	pIndices[1] = 1;
	pIndices[2] = 2;
	pIndices[3] = 2;
	pIndices[4] = 1;
	pIndices[5] = 3;

	// Create and initialise the vertex and index buffers

	m_vertexBuffer.Initialise(VertexBuffer, pDevice, D3D11_USAGE_IMMUTABLE, pVertices, m_vertexCount);
	m_indexBuffer.Initialise(IndexBuffer, pDevice, D3D11_USAGE_IMMUTABLE, pIndices, m_indexCount);

	// Release the temporary arrays
	delete [] pVertices;
	pVertices = nullptr;

	delete [] pIndices;
	pIndices = nullptr;

	return true;
}