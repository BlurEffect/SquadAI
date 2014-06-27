/* 
*  Kevin Meergans, SquadAI, 2014
*  FontPixelShader.cpp
*  Wrapper for the "PS_Font.FX" pixel shader.
*  Pixel shader used for text rendering.
*/

// Includes
#include "FontPixelShader.h"


FontPixelShader::FontPixelShader(void)
{
	m_cbPerObjectData.m_colour = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
}

FontPixelShader::~FontPixelShader(void)
{
	Cleanup();
}

//--------------------------------------------------------------------------------------
// Initialise the shader's member variables.
// Param1: The device used for initialisation
// Returns true if the initialisation of the shader was successful, false otherwise.
//--------------------------------------------------------------------------------------
bool FontPixelShader::Initialise(ID3D11Device* pDevice)
{
	// Create the shader
	if(FAILED(pDevice->CreatePixelShader(g_PS_FontCompiled, sizeof(g_PS_FontCompiled), nullptr, &m_pPixelShader)))
	{
		return false;
	}

	return m_cbPerObject.Initialise(ConstantBuffer, pDevice, D3D11_USAGE_DYNAMIC, nullptr, 1);
}

//--------------------------------------------------------------------------------------
// Free all allocated resources.
//--------------------------------------------------------------------------------------
void FontPixelShader::Cleanup(void)
{
	m_cbPerObject.Cleanup();
	PixelShader::Cleanup();
}

//--------------------------------------------------------------------------------------
// Update the per-object constant buffer of the shader.
// Param1: The device context used to update the constant buffer.
// Param2: The structure holding the shader parameters for the current object.
// Returns true if the per-frame parameters of the shader were updated successfully (that is also true
// in the case that the shader doesn't actually use per-object parameters), false if the update failed.
//--------------------------------------------------------------------------------------
bool FontPixelShader::SetObjectData(ID3D11DeviceContext* pContext, const PerObjectData& perObjectData)
{
	m_cbPerObjectData.m_colour = perObjectData.m_colour;

	if(!m_cbPerObject.Update(pContext, &m_cbPerObjectData, 1))
	{
		return false;
	}

	pContext -> PSSetConstantBuffers( 0, 1, m_cbPerObject.GetBuffer() );

	return true;
}

//--------------------------------------------------------------------------------------
// Updates the textures and corresponding sampler states used by the shader.
// Param1: The index of the texture resource of the shader that is to be updated.
// Param2: The device context used to update the texture resource.
// Param3: A pointer to the texture that should be set as resource for the shader.
// Param4: A pointer to the sampler state that should be used with the passed in texture.
// Returns true if the texture resource was set successfully, false otherwise.
//--------------------------------------------------------------------------------------
bool FontPixelShader::SetTexture(UINT index, ID3D11DeviceContext* pContext, ID3D11ShaderResourceView* pTexture, ID3D11SamplerState* pSamplerState)
{
	// This shader only takes one texture resource at index 0.
	if( index != 0 )
	{
		return false;
	}

	// Set the texture and sampler state
	pContext->PSSetShaderResources(index, 1, &pTexture);
	pContext->PSSetSamplers(index, 1, &pSamplerState);

	return true;
}
