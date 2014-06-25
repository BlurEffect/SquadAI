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
#include "SimplePixelShader.h"
#include "FontVertexShader.h"
#include "FontPixelShader.h"

class ShaderFactory
{
public:
	static VertexShader* CreateVertexShader(ShaderType);
	static PixelShader*  CreatePixelShader(ShaderType);
};

#endif // SHADER_FACTORY_H
