/* 
*  Kevin Meergans, SquadAI, 2014
*  SimpleVertexShader.h
*  Wrapper for the "VS_Simple.FX" vertex shader.
*  Performs standard transformation of the vertex position and forwards
*  the result together with the object colour to the next stage.
*/

// Includes
#include "SimpleVertexShader.h"

// The vertex input layout expected by this vertex shader
const D3D11_INPUT_ELEMENT_DESC EditorGeometryVertexShader::m_sInputLayoutDescription[] = 
{ 
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT   , 0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 } 
};

SimpleVertexShader::SimpleVertexShader( void ) : m_pCbPerObjectBuffer( nullptr)
{
	// Initialise constant buffer data

	XMStoreFloat4x4( &m_cbPerObject.m_WVP, XMMatrixIdentity() );
	m_cbPerObject.m_colour = XMFLOAT4( 0.0f, 0.0f, 0.0f, 1.0f );
}

//--------------------------------------------------------------------------------------
// Initialise the shader's member variables.
// Param1: The device used for initialisation
// Returns true if the initialisation of the shader was successful, false otherwise.
//--------------------------------------------------------------------------------------
bool EditorGeometryVertexShader::Initialise( ID3D11Device* pDevice )
{
	HRESULT hr;

	// Create the shader

	hr = pDevice -> CreateVertexShader( g_editorGeometryVertexShader, sizeof( g_editorGeometryVertexShader ), nullptr, &m_pVertexShader );
	if( FAILED ( hr ) )
	{
		return hr;
	}

	// Create the vertex input layout

	UINT numElements = ARRAYSIZE( m_sInputLayoutDescription );
	hr = pDevice -> CreateInputLayout( m_sInputLayoutDescription, numElements, g_editorGeometryVertexShader, sizeof( g_editorGeometryVertexShader ), &m_pInputLayout );
	if( FAILED ( hr ) )
	{
		return hr;
	}

	// Create constant buffers

	D3D11_BUFFER_DESC cbPerObjectDesc;	
	ZeroMemory( &cbPerObjectDesc, sizeof( D3D11_BUFFER_DESC ) );

	cbPerObjectDesc.Usage = D3D11_USAGE_DYNAMIC;
	cbPerObjectDesc.ByteWidth = sizeof( m_cbPerObject );
	cbPerObjectDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbPerObjectDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cbPerObjectDesc.MiscFlags = 0;

	hr = pDevice -> CreateBuffer( &cbPerObjectDesc, NULL, &m_pCbPerObjectBuffer );
	if( FAILED ( hr ) )
	{
		return hr;
	}

	return S_OK;
}

//--------------------------------------------------------------------------------------
// Free all allocated resources.
//--------------------------------------------------------------------------------------
HRESULT EditorGeometryVertexShader::Cleanup( void )
{
	if( m_pCbPerObjectBuffer )
	{
		m_pCbPerObjectBuffer -> Release();
		m_pCbPerObjectBuffer = nullptr;
	}
	
	return VertexShader::Cleanup();
}

//--------------------------------------------------------------------------------------
// Update the per-scene constant buffer of the shader.
//--------------------------------------------------------------------------------------
HRESULT EditorGeometryVertexShader::UpdatePerSceneData( ID3D11DeviceContext* pContext, const PerSceneData& perSceneData)
{
	// Buffer not used in this shader
	return E_FAIL;
}

//--------------------------------------------------------------------------------------
// Update the per-frame constant buffer of the shader.
//--------------------------------------------------------------------------------------
HRESULT EditorGeometryVertexShader::UpdatePerFrameData( ID3D11DeviceContext* pContext, const PerFrameData& perFrameData)
{
	// Buffer not used in this shader
	return E_FAIL;
}

//--------------------------------------------------------------------------------------
// Update the per-object constant buffer of the shader.
//--------------------------------------------------------------------------------------
HRESULT EditorGeometryVertexShader::UpdatePerObjectData( ID3D11DeviceContext* pContext, const PerObjectData& perObjectData)
{
	HRESULT hr;

	m_cbPerObject.m_WVP = perObjectData.m_WVP;
	m_cbPerObject.m_colour = perObjectData.m_colour;

	D3D11_MAPPED_SUBRESOURCE mappedResource;
	hr = pContext -> Map( m_pCbPerObjectBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource );
	if( FAILED( hr ) )
	{
		return hr;
	}

	memcpy( mappedResource.pData, &m_cbPerObject, sizeof( ConstBufferPerObject ) );
	pContext -> Unmap( m_pCbPerObjectBuffer, 0 );

	pContext -> VSSetConstantBuffers( 0, 1, &m_pCbPerObjectBuffer );

	return S_OK;
}

//--------------------------------------------------------------------------------------
// Update the texture and corresponding sample state being used by the shader.
//--------------------------------------------------------------------------------------
HRESULT EditorGeometryVertexShader::UpdateTextureResource( int index, ID3D11DeviceContext* pContext, ID3D11ShaderResourceView* pTexture, ID3D11SamplerState* pSamplerState )
{
	// Not used in this shader
	return E_FAIL;
}
