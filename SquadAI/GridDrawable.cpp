/* 
*  Kevin Meergans, SquadAI, 2014
*  GridDrawable.h
*  Drawable that encapsulates a grid made up of single lines.
*/

// Includes
#include "GridDrawable.h"

GridDrawable::GridDrawable(float width, float height, int horizontalPartitions, int verticalPartitions) : Drawable(),
																										  m_width(width),
																										  m_height(height),
																										  m_horizontalPartitions(horizontalPartitions),
																									      m_verticalPartitions(verticalPartitions)
{
	m_primitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_LINELIST;
}

GridDrawable::~GridDrawable(void)
{
}

//--------------------------------------------------------------------------------------
// Initialise the Grid Drawable.
// Param1: A pointer to the device that should be used for the creation of this Drawable.
// Returns true if the initialisation succeeded, false otherwise.
//--------------------------------------------------------------------------------------
bool GridDrawable::Initialise(ID3D11Device* pDevice)
{
	// Set the vertex and index count for this Drawable

	// Calculate the vertex and index count:
	m_vertexCount = (m_horizontalPartitions + 1) * 2 + (m_verticalPartitions + 1) * 2;
	m_indexCount  = m_vertexCount;

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

	// Determine the extents of a grid field
	float horizontalSpacing = m_width / static_cast<float>(m_horizontalPartitions);
	float verticalSpacing   = m_height / static_cast<float>(m_verticalPartitions);
		
	// Minimal and maximal positions of vertices forming the grid
	XMFLOAT2 gridMin(-m_width / 2.0f, -m_height / 2.0f);
	XMFLOAT2 gridMax(m_width / 2.0f, m_height / 2.0f);

	// Define vertices and indices for the Drawable

	// x-axis position of the first vertex
	float vertexPos = gridMin.y;

	int   index = 0;		   

	// Place the vertices forming lines parallel to the x-axis
	while(vertexPos <= gridMax.y)
	{
		pVertices[index].m_position.x = gridMin.x;
		pVertices[index].m_position.y = vertexPos;
		pVertices[index].m_position.z = 0.0f;
		pIndices[index] = index;
		++index;

		pVertices[index].m_position.x = gridMax.x;
		pVertices[index].m_position.y = vertexPos;
		pVertices[index].m_position.z = 0.0f;
		pIndices[index] = index;
		++index;

		vertexPos += verticalSpacing;
	}

	vertexPos = gridMin.x;

	// Place the vertices forming lines parallel to the y-axis
	while(vertexPos <= gridMax.x)
	{
		pVertices[index].m_position.x = vertexPos;
		pVertices[index].m_position.y = gridMin.y;
		pVertices[index].m_position.z = 0.0f;
		pIndices[index] = index;
		++index;

		pVertices[index].m_position.x = vertexPos;
		pVertices[index].m_position.y = gridMax.y;
		pVertices[index].m_position.z = 0.0f;
		pIndices[index] = index;
		++index;

		vertexPos += horizontalSpacing;
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