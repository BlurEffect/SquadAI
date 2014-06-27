/* 
*  Kevin Meergans, SquadAI, 2014
*  FontPixelShader.h
*  Wrapper for the "PS_Font.FX" pixel shader.
*  Pixel shader used for text rendering.
*/

#ifndef FONT_PIXEL_SHADER_H
#define FONT_PIXEL_SHADER_H

// Includes
#include <d3d11.h>
#include <DirectXMath.h>
#include "PS_FontCompiled.h"
#include "PixelShader.h"
#include "ShaderParameters.h"
#include "Buffer.h"

class FontPixelShader : public PixelShader
{
public:
	FontPixelShader(void);
	~FontPixelShader(void);
	bool Initialise(ID3D11Device* pDevice);
	void Cleanup(void);
	bool SetObjectData(ID3D11DeviceContext* pContext, const PerObjectData& perObjectData);
	bool SetTexture(UINT index, ID3D11DeviceContext* pContext, ID3D11ShaderResourceView* pTexture, ID3D11SamplerState* pSamplerState);
private:

	//--------------------------------------------------------------------------------------
	// Constant buffer used by the shader. To be updated for each object.
	//--------------------------------------------------------------------------------------
	struct ConstBufferPerObjectData
	{
		XMFLOAT4   m_colour;			  // The colour that the object should be coloured in
	};

	ConstBufferPerObjectData	     m_cbPerObjectData; // Stores the per object data
	Buffer<ConstBufferPerObjectData> m_cbPerObject;     // The constant buffer used by the shader
};

#endif // FONT_PIXEL_SHADER_H