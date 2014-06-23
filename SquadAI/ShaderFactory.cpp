/* 
*  Kevin Meergans, SquadAI, 2014
*  ShaderFactory.cpp
*  Class with a couple of static member function all serving as
*  factory functions for different types of shaders.
*/

// Includes
#include "ShaderFactory.h"

//--------------------------------------------------------------------------------------
// Factory function to create new vertex shaders. Dynamically creates new instances of the 
// requested shader.
// Param1: The type of vertex shader requested.
// Param2: Determines whether the version of the shader set up for hardware instancing will be returned.
// Returns a pointer to the new shader. When the creation of the shader failed or the passed
// in shader type is invalid, a null pointer will be returned.
//--------------------------------------------------------------------------------------
VertexShader* ShaderFactory::CreateVertexShader(ShaderType type, bool doCreateInstancedVersion)
{
	if(doCreateInstancedVersion)
	{
		switch(type)
		{
		case SimpleUnlit:
			return new SimpleInstancedVertexShader();
			break;
		}
	}else
	{
		switch(type)
		{
		case SimpleUnlit:
			return new SimpleVertexShader();
			break;
		}
	}

	return nullptr;
}

//--------------------------------------------------------------------------------------
// Factory function to create new pixel shaders. Dynamically creates new instances of the 
// requested shader.
// Param1: The type of pixel shader requested.
// Param2: Determines whether the version of the shader set up for hardware instancing will be returned.
// Returns a pointer to the new shader. When the creation of the shader failed or the passed
// in shader type is invalid, a null pointer will be returned.
//--------------------------------------------------------------------------------------
PixelShader* ShaderFactory::CreatePixelShader(ShaderType type, bool doCreateInstancedVersion)
{
	if(doCreateInstancedVersion)
	{
		switch(type)
		{
		case SimpleUnlit:
			return new SimplePixelShader(); // same as non-instanced version
			break;
		}
	}else
	{
		switch(type)
		{
		case SimpleUnlit:
			return new SimplePixelShader();
			break;
		}
	}

	return nullptr;
}