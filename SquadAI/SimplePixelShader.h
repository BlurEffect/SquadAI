/* 
*  Kevin Meergans, SquadAI, 2014
*  SimplePixelShader.h
*  Wrapper for the "PS_Simple.FX" pixel shader.
*  Simple pixel shader that returns the vertex colour passed through from
*  the vertex shader as pixel colour.
*/

#ifndef SIMPLE_PIXEL_SHADER_H
#define SIMPLE_PIXEL_SHADER_H

// Includes
#include <d3d11.h>
#include <DirectXMath.h>
#include "PS_SimpleCompiled.h"
#include "PixelShader.h"
#include "ShaderParameters.h"

class SimplePixelShader : public PixelShader
{
public:
	SimplePixelShader(void);
	~SimplePixelShader(void);
	bool Initialise(ID3D11Device* pDevice);
};

#endif // SIMPLE_PIXEL_SHADER_H