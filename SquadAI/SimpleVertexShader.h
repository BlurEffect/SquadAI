/* 
*  Kevin Meergans, SquadAI, 2014
*  SimpleVertexShader.h
*  Wrapper for the "VS_Simple.FX" vertex shader.
*  Performs standard transformation of the vertex position and forwards
*  the result together with the object colour to the next stage.
*/

#ifndef SIMPLE_VERTEX_SHADER_H
#define SIMPLE_VERTEX_SHADER_H

// Includes
#include <d3d11.h>
#include <DirectXMath.h>
#include "VS_SimpleCompiled.h"
#include "VertexShader.h"
#include "ShaderParameters.h"

using namespace DirectX;

class SimpleVertexShader : public VertexShader
{
public:
	SimpleVertexShader(void);
	~SimpleVertexShader(void);
	bool Initialise(ID3D11Device* pDevice);
	void Cleanup(void);
	bool UpdatePerFrameData(ID3D11DeviceContext* pContext, const PerFrameData& perFrameData);
	bool UpdatePerObjectData(ID3D11DeviceContext* pContext, const PerObjectData& perObjectData);

private:

	// The input layout required by this shader
	const static D3D11_INPUT_ELEMENT_DESC m_sInputLayoutDescription[1];

	//--------------------------------------------------------------------------------------
	// Constant buffer used by the shader. To be updated for each object.
	//--------------------------------------------------------------------------------------
	struct ConstBufferPerObject
	{
		XMFLOAT4X4 m_worldViewProjection; // The combined world, view and projection matrices
		XMFLOAT4   m_colour;			  // The colour that the object should be coloured in
	};
};

#endif // SIMPLE_VERTEX_SHADER_H