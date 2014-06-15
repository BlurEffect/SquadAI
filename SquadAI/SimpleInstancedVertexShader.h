/* 
*  Kevin Meergans, SquadAI, 2014
*  SimpleInstancedVertexShader.h
*  Wrapper for the "VS_SimpleInstanced.FX" vertex shader.
*  Alternate version to VS_Simple.fx allowing for hardware instancing.
*/

#ifndef SIMPLE_INSTANCED_VERTEX_SHADER_H
#define SIMPLE_INSTANCED_VERTEX_SHADER_H

// Includes
#include <d3d11.h>
#include <DirectXMath.h>
#include "VS_SimpleInstancedCompiled.h"
#include "VertexShader.h"
#include "ShaderParameters.h"
#include "Buffer.h"

using namespace DirectX;

class SimpleInstancedVertexShader : public VertexShader
{
public:
	SimpleInstancedVertexShader(void);
	~SimpleInstancedVertexShader(void);
	bool Initialise(ID3D11Device* pDevice);
	void Cleanup(void);
	bool SetFrameData(ID3D11DeviceContext* pContext, const PerFrameData& perFrameData);
	bool SetObjectData(ID3D11DeviceContext* pContext, const PerObjectData& perObjectData);

private:

	//--------------------------------------------------------------------------------------
	// Constant buffer used by this shader. To be updated once per frame.
	//--------------------------------------------------------------------------------------
	struct ConstBufferPerFrame
	{
		XMFLOAT4X4 m_viewProjection; // The combined view and projection matrices
	};

	// The input layout required by this shader
	const static D3D11_INPUT_ELEMENT_DESC m_sInputLayoutDescription[6];

	ConstBufferPerFrame	        m_cbPerFrame;		 // Stores the per per frame data
	Buffer<ConstBufferPerFrame> m_pCbPerFrameBuffer; // The constant buffer used by the shader
};

#endif // SIMPLE_INSTANCED_VERTEX_SHADER_H