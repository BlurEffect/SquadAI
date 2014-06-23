/* 
*  Kevin Meergans, SquadAI, 2014
*  ShaderFactory.h
*  Class with a couple of static member function all serving as
*  factory functions for different types of shaders.
*/

#ifndef SHADER_FACTORY_H
#define SHADER_FACTORY_H

// Includes
#include "VertexShader.h"
#include "PixelShader.h"
#include "ShaderGroup.h"
#include "SimpleVertexShader.h"
#include "SimpleInstancedVertexShader.h"
#include "SimplePixelShader.h"

class ShaderFactory
{
public:
	static VertexShader* CreateVertexShader(ShaderType, bool doCreateInstancedVersion);
	static PixelShader*  CreatePixelShader(ShaderType, bool doCreateInstancedVersion);
};

#endif // SHADER_FACTORY_H
