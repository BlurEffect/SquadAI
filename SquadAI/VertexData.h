/* 
*  Kevin Meergans, SquadAI, 2014
*  VertexData.h
*  Defines the structure of vertices and instances.
*/

#ifndef VERTEX_DATA_H
#define VERTEX_DATA_H

// Includes
#include <DirectXMath.h>

using namespace DirectX;

//--------------------------------------------------------------------------------------
// The structure of a vertex.
//--------------------------------------------------------------------------------------
struct Vertex
{
	Vertex() : m_position(0.0f, 0.0f, 0.0f){}
	Vertex(float posX, float posY, float posZ) : m_position( posX, posY, posZ){}

	XMFLOAT3 m_position;	// position of the vertex
};

//--------------------------------------------------------------------------------------
// The structure of the instance data used when instancing is used to render objects.
//--------------------------------------------------------------------------------------
struct Instance
{
	Instance(const XMFLOAT4X4& world, const XMFLOAT4& colour) : m_world( world ), m_colour( colour ){}

	XMFLOAT4X4 m_world;
	XMFLOAT4   m_colour;
};

#endif // VERTEX_DATA_H