/* 
*  Kevin Meergans, SquadAI, 2014
*  Circle.cpp
*  Drawable that encapsulates a circle-shaped mesh.
*/

// Includes
#include "Circle.h"

Circle::Circle(void) : Drawable()
{
	m_primitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;
}

Circle::~Circle(void)
{
}

//--------------------------------------------------------------------------------------
// Initialise the Circle Drawable.
// Param1: A pointer to the device that should be used for the creation of this Drawable.
// Returns true if the initialisation succeeded, false otherwise.
//--------------------------------------------------------------------------------------
bool Circle::Initialise(ID3D11Device* pDevice)
{
	// Set the vertex and index count for this Drawable

	// Use this many slices for the circle
	int slices = 24;

	m_vertexCount = slices + 2;
	m_indexCount  = slices + 2;

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

	float step = XMConvertToRadians(180.0f / static_cast<float>(slices) / 2.0f);

	pVertices[0].m_position = XMFLOAT3(-0.5f, 0.0f, 0.0f);

	int index = 1;
	float x = 0.0f;
	float z = 0.0f;

	float angle = -XM_PIDIV2; // -PI/2
	for(unsigned int i = 0; i < slices/2; ++i)
	{
		x = 0.5f * sin(angle);
		z = 0.5f * cos(angle);

		pVertices[index++].m_position = XMFLOAT3(x, 0.0f, z);
		pVertices[index++].m_position = XMFLOAT3(x, 0.0f, -z);

		angle += step;
	}

	pVertices[index].m_position = XMFLOAT3(0.5f, 0.0f, 0.0f);

	for(unsigned int i = 0; i < m_indexCount; ++i)
	{
		pIndices[i] = i;
	}

	// Create and initialise the vertex and index buffers

	m_vertexBuffer.Initialise(VertexBuffer, D3D11_USAGE_IMMUTABLE, pVertices, m_vertexCount);
	m_indexBuffer.Initialise(IndexBuffer, D3D11_USAGE_IMMUTABLE, pIndices, m_indexCount);

	// Release the temporary arrays
	delete [] pVertices;
	pVertices = nullptr;

	delete [] pIndices;
	pIndices = nullptr;
}