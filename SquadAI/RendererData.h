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
#include "EntityData.h"

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

// Maps drawables to entities and thus determines which mesh
// is used to display these objects.
const DrawableType g_cDrawableMappings[NumberOfEntities] = 
{
	GridType,     // Grid
	TriangleType, // SoldierTeamA
	TriangleType, // DeadSoldierTeamA
	TriangleType, // SoldierTeamB
	TriangleType, // DeadSoldierTeamB
	CircleType,   // Projectile
	SquareType    // Cover
};

// Maps colour values to entities.
const XMFLOAT4 g_cColourMappings[NumberOfEntities] = 
{
	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), // Grid
	XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f), // SoldierTeamA
	XMFLOAT4(0.2f, 0.0f, 0.0f, 1.0f), // DeadSoldierTeamA
	XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f), // SoldierTeamB
	XMFLOAT4(0.0f, 0.2f, 0.0f, 1.0f), // DeadSoldierTeamB
	XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f), // Projectile
	XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f), // Cover
};

#endif // RENDERER_DATA_H