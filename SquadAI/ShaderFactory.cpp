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
	VertexShader* pVertexShader = nullptr;

	if(doCreateInstancedVersion)
	{
		switch(type)
		{
		case SimpleUnlit:
			pVertexShader = new SimpleInstancedVertexShader();
			break;
		}
	}else
	{
		switch(type)
		{
		case SimpleUnlit:
			pVertexShader = new SimpleVertexShader();
			break;
		}
	}

	return pVertexShader;
}

//--------------------------------------------------------------------------------------
// Factory function to create new pixel shaders. Dynamically creates new instances of the 
// requested shader.
// Param1: The type of pixel shader requested.
// Param2: Determines whether the version of the shader set up for hardware instancing will be returned.
// Returns a pointer to the new shader. When the creation of the shader failed or the passed
// in shader type is invalid, a null pointer will be returned.
//--------------------------------------------------------------------------------------
PixelShader* ShaderFactory::CreatePixel(ShaderType type, bool doCreateInstancedVersion)
{
	PixelShader* pPixelShader = nullptr;

	if(doCreateInstancedVersion)
	{
		switch(type)
		{
		case SimpleUnlit:
			pPixelShader = new SimplePixelShader(); // same as non-instanced version
			break;
		}
	}else
	{
		switch(type)
		{
		case SimpleUnlit:
			pPixelShader = new SimplePixelShader();
			break;
		}
	}

	return pPixelShader;
}