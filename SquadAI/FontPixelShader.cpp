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
	return(SUCCEEDED(pDevice->CreatePixelShader(g_PS_FontCompiled, sizeof(g_PS_FontCompiled), nullptr, &m_pPixelShader)));
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
