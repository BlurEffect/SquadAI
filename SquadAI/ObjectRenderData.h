/* 
*  Kevin Meergans, SquadAI, 2014
*  ObjectRenderData.h
*  Contains data structures holding information required by the renderer
*  to correctly draw objects.
*/

#ifndef OBJECT_RENDER_DATA_H
#define OBJECT_RENDER_DATA_H

// Includes
#include <DirectXMath.h>

using namespace DirectX;

//--------------------------------------------------------------------------------------
// Identifies the Drawable, that is the mesh, to use to render an object.
//--------------------------------------------------------------------------------------
enum DrawableType
{
	TriangleType,
	SquareType,
	CircleType,
	NumberOfDrawableTypes
};

//--------------------------------------------------------------------------------------
// Identifies the colour to draw an object in.
//--------------------------------------------------------------------------------------
enum ColourID
{
	White,
	Yellow,
	Red,
	Blue,
	Green,
	Black,
	NumberOfColourIDs
};

//--------------------------------------------------------------------------------------
// Contains all the data required by the renderer to properly draw an object.
//--------------------------------------------------------------------------------------
struct ObjectRenderData
{
	DrawableType m_drawableType;	// The drawable to use to display this object
	ColourID     m_colourID;		// The colour, in which to colour this object's mesh
	XMFLOAT4X4   m_transform;		// The transform matrix to apply to the base Drawable
};

#endif // OBJECT_RENDER_DATA_H
