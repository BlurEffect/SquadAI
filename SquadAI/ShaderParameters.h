/* 
*  Kevin Meergans, SquadAI, 2014
*  ShaderParameters.h
*  Defines structures used to update the shaders used by the application.
*  These structures bundle all data that is required by any used shader with the
*  respective shaders picking the data required when these structures are passed
*  to them.
*/

#ifndef SHADER_PARAMETERS_H
#define SHADER_PARAMETERS_H

// Includes
#include <DirectXMath.h>

using namespace DirectX;

//--------------------------------------------------------------------------------------
// Contains all data that could be required by a shader to update its per-frame
// constant buffer.
//--------------------------------------------------------------------------------------
struct PerFrameData
{
	XMFLOAT4X4 m_viewProjection; // The combined view and projection matrices
};

//--------------------------------------------------------------------------------------
// Contains all data that could be required by a shader to update its per-object 
// constant buffer.
//--------------------------------------------------------------------------------------
struct PerObjectData
{
	XMFLOAT4X4 m_worldViewProjection; // The combined world, view and projection matrices
	XMFLOAT4   m_colour;			  // The colour that the object should be coloured in
};

#endif // SHADER_PARAMETERS_H