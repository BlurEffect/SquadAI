/* 
*  Kevin Meergans, SquadAI, 2014
*  CircleDrawable.cpp
*  Drawable that encapsulates a circle-shaped mesh.
*/

// Includes
#include "CircleDrawable.h"

CircleDrawable::CircleDrawable(float radius, unsigned int segments) : Drawable(),
																	  m_radius(radius),
																	  m_segments(segments)
{
	m_primitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;
}

CircleDrawable::~CircleDrawable(void)
{
}

//--------------------------------------------------------------------------------------
// Initialise the Circle Drawable.
// Param1: A pointer to the device that should be used for the creation of this Drawable.
// Returns true if the initialisation succeeded, false otherwise.
//--------------------------------------------------------------------------------------
bool CircleDrawable::Initialise(ID3D11Device* pDevice)
{
	// Set the vertex and index count for this Drawable

	m_vertexCount = m_segments + 2;
	m_indexCount  = m_segments + 2;

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

	float step = XMConvertToRadians(180.0f / (static_cast<float>(m_segments) / 2.0f));

	pVertices[0].m_position = XMFLOAT3(-0.5f, 0.0f, 0.0f);

	int index = 1;
	float x = 0.0f;
	float y = 0.0f;

	float angle = -XM_PIDIV2; 
	for(unsigned int i = 0; i < static_cast<unsigned int>(m_segments/2); ++i)
	{
		x = m_radius * sin(angle);
		y = m_radius * cos(angle);

		pVertices[index++].m_position = XMFLOAT3(x, y, 0.0f);
		pVertices[index++].m_position = XMFLOAT3(x, -y, 0.0f);

		angle += step;
	}

	pVertices[index].m_position = XMFLOAT3(0.5f, 0.0f, 0.0f);

	for(unsigned int i = 0; i < m_indexCount; ++i)
	{
		pIndices[i] = i;
	}

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