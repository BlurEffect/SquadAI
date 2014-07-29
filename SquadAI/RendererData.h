/* 
*  Kevin Meergans, SquadAI, 2014
*  RendererData.h
*  Contains data structures used by the renderer and some constants determining
*  the mapping between objects in the test environment and the drawable and colour
*  that is used by the renderer to display them.
*/

#ifndef RENDERER_DATA_H
#define RENDERER_DATA_H

// Includes
#include <DirectXMath.h>
#include "ObjectTypes.h"

using namespace DirectX;

//--------------------------------------------------------------------------------------
// Identifies the Drawable, that is the mesh, to use to render an object.
//--------------------------------------------------------------------------------------
enum DrawableType
{
	TriangleType,
	SquareType,
	CircleType,
	GridType,
	NumberOfDrawableTypes
};

//--------------------------------------------------------------------------------------
// Bundles information that is required to render a entity of the test application.
//--------------------------------------------------------------------------------------
struct ObjectRenderData
{
	ObjectRenderData(void) : m_drawableType(DrawableType(0)),
							 m_colour(1.0f, 1.0f, 1.0f, 1.0f),
						     m_name(""),
							 m_baseZ(0.0f)
	{}

	DrawableType m_drawableType; // Identifies the drawable representing this entity
	XMFLOAT4     m_colour;       // The colour, in which to render the entity
	const char*  m_name;         // The name of the entity, for display on the GUI
	float        m_baseZ;        // Used to place some entities above other when it comes to intersections
};

#endif // RENDERER_DATA_H