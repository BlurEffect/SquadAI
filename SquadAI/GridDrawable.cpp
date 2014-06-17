/* 
*  Kevin Meergans, SquadAI, 2014
*  GridDrawable.h
*  Drawable that encapsulates a grid made up of single lines.
*/

// Includes
#include "GridDrawable.h"

GridDrawable::GridDrawable(void) : Drawable()
{
	m_primitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_POINTLIST;
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
	m_vertexCount = (g_cNumberOfHorizontalPartitions + 1) * 2 + (g_cNumberOfVerticalPartitions + 1) * 2;
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
	float horizontalSpacing = g_cGridWidth / static_cast<float>(g_cNumberOfHorizontalPartitions);
	float verticalSpacing   = g_cGridHeight / static_cast<float>(g_cNumberOfVerticalPartitions);
		
	// Minimal and maximal positions of vertices forming the grid
	XMFLOAT2 gridMin(-g_cGridWidth / 2.0f, -g_cGridHeight / 2.0f);
	XMFLOAT2 gridMax(g_cGridWidth / 2.0f, g_cGridHeight / 2.0f);

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
		pVertices[index].m_position.x = gridMin.y;
		pVertices[index].m_position.y = vertexPos;
		pVertices[index].m_position.z = 0.0f;
		pIndices[index] = index;
		++index;

		pVertices[index].m_position.x = gridMax.y;
		pVertices[index].m_position.y = vertexPos;
		pVertices[index].m_position.z = 0.0f;
		pIndices[index] = index;
		++index;

		vertexPos += horizontalSpacing;
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