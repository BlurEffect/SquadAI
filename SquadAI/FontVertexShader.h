/* 
*  Kevin Meergans, SquadAI, 2014
*  FontVertexShader.h
*  Wrapper for the "VS_Font.FX" vertex shader.
*  Vertex shader for text rendering.
*/

#ifndef FONT_VERTEX_SHADER_H
#define FONT_VERTEX_SHADER_H

// Includes
#include <d3d11.h>
#include <DirectXMath.h>
#include "VS_FontCompiled.h"
#include "VertexShader.h"
#include "ShaderParameters.h"
#include "Buffer.h"

using namespace DirectX;

class FontVertexShader : public VertexShader
{
public:
	FontVertexShader(void);
	~FontVertexShader(void);
	bool Initialise(ID3D11Device* pDevice);
	void Cleanup(void);
	bool SetFrameData(ID3D11DeviceContext* pContext, const PerFrameData& perFrameData);

private:

	//--------------------------------------------------------------------------------------
	// Constant buffer used by the shader. To be updated for each frame.
	//--------------------------------------------------------------------------------------
	struct ConstBufferPerFrameData
	{
		XMFLOAT4X4 m_viewProjection; // The combined view and projection matrices
	};
	
	// The input layout required by this shader
	const static D3D11_INPUT_ELEMENT_DESC m_sInputLayoutDescription[2];

	ConstBufferPerFrameData	        m_cbPerFrameData; // Stores the per frame data
	Buffer<ConstBufferPerFrameData> m_cbPerFrame;     // The constant buffer used by the shader
};

#endif // FONT_VERTEX_SHADER_H