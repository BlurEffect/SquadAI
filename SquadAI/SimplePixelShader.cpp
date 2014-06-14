/* 
*  Kevin Meergans, SquadAI, 2014
*  SimplePixelShader.h
*  Wrapper for the "PS_Simple.FX" pixel shader.
*  Simple pixel shader that returns the vertex colour passed through from
*  the vertex shader as pixel colour.
*/

// Includes
#include "SimplePixelShader.h"


SimplePixelShader::SimplePixelShader( void )
{
}

SimplePixelShader::~SimplePixelShader( void )
{
}

//--------------------------------------------------------------------------------------
// Initialise the shader's member variables.
// Param1: The device used for initialisation
// Returns true if the initialisation of the shader was successful, false otherwise.
//--------------------------------------------------------------------------------------
bool SimplePixelShader::Initialise(ID3D11Device* pDevice)
{
	HRESULT hr;

	// Create the shader

	hr = pDevice -> CreatePixelShader( g_simplePixelShader, sizeof( g_simplePixelShader ), nullptr, &m_pPixelShader );
	if( FAILED ( hr ) )
	{
		return hr;
	}

	return S_OK;
}

//--------------------------------------------------------------------------------------
// Free all allocated resources.
//--------------------------------------------------------------------------------------
HRESULT SimplePixelShader::Cleanup( void )
{
	return PixelShader::Cleanup();
}

//--------------------------------------------------------------------------------------
// Update the per-scene constant buffer of the shader.
//--------------------------------------------------------------------------------------
HRESULT SimplePixelShader::UpdatePerSceneData( ID3D11DeviceContext* pContext, const PerSceneData& perSceneData)
{
	// Buffer not used in this shader
	return E_FAIL;
}

//--------------------------------------------------------------------------------------
// Update the per-frame constant buffer of the shader.
//--------------------------------------------------------------------------------------
HRESULT SimplePixelShader::UpdatePerFrameData( ID3D11DeviceContext* pContext, const PerFrameData& perFrameData)
{
	// Buffer not used in this shader
	return E_FAIL;
}

//--------------------------------------------------------------------------------------
// Update the per-object constant buffer of the shader.
//--------------------------------------------------------------------------------------
HRESULT SimplePixelShader::UpdatePerObjectData( ID3D11DeviceContext* pContext, const PerObjectData& perObjectData)
{
	// not used in this shader
	return E_FAIL;
}

//--------------------------------------------------------------------------------------
// Update the texture and corresponding sample state being used by the shader.
//--------------------------------------------------------------------------------------
HRESULT SimplePixelShader::UpdateTextureResource( int index, ID3D11DeviceContext* pContext, ID3D11ShaderResourceView* pTexture, ID3D11SamplerState* pSamplerState )
{
	// Not used in this shader
	return E_FAIL;
}

