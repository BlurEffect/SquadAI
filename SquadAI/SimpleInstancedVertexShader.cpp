/* 
*  Kevin Meergans, SquadAI, 2014
*  SimpleInstancedVertexShader.h
*  Wrapper for the "VS_SimpleInstanced.FX" vertex shader.
*  Alternate version to VS_Simple.fx allowing for hardware instancing.
*/

// Includes
#include "SimpleInstancedVertexShader.h"

// The vertex input layout expected by this vertex shader
const D3D11_INPUT_ELEMENT_DESC SimpleVertexShader::m_sInputLayoutDescription[] = 
{
	// Vertex data
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0,  0, D3D11_INPUT_PER_VERTEX_DATA,   0 },  
	{ "NORMAL"  , 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, 12, D3D11_INPUT_PER_VERTEX_DATA,   0 },
	{ "AVGNORMAL"  , 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, 24, D3D11_INPUT_PER_VERTEX_DATA,   0 },
	{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT   ,    0, 36, D3D11_INPUT_PER_VERTEX_DATA,   0 },  
	
	// Instance data
	{ "WORLD",	  0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1,  0, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
	{ "WORLD",    1, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
	{ "WORLD",    2, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 32, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
	{ "WORLD",    3, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 48, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
	{ "COLOUR",   0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 64, D3D11_INPUT_PER_INSTANCE_DATA, 1 }
};

SimpleVertexShader::SimpleVertexShader( void ) : VertexShader(), m_pCbPerFrameBuffer( nullptr)
{
	// Initialise constant buffer data

	XMStoreFloat4x4( &m_cbPerFrame.m_viewProjection, XMMatrixIdentity() );
}

//--------------------------------------------------------------------------------------
// Initialise the shader's member variables.
//--------------------------------------------------------------------------------------
HRESULT SimpleVertexShader::Initialise( ID3D11Device* pDevice )
{
	HRESULT hr;

	// Create the shader

	hr = pDevice -> CreateVertexShader( g_simpleVertexShader, sizeof( g_simpleVertexShader ), nullptr, &m_pVertexShader );
	if( FAILED ( hr ) )
	{
		return hr;
	}

	// Create the vertex input layout

	UINT numElements = ARRAYSIZE( m_sInputLayoutDescription );
	hr = pDevice -> CreateInputLayout( m_sInputLayoutDescription, numElements, g_simpleVertexShader, sizeof( g_simpleVertexShader ), &m_pInputLayout );
	if( FAILED ( hr ) )
	{
		return hr;
	}

	// Create constant buffers

	D3D11_BUFFER_DESC cbPerFrameDesc;	
	ZeroMemory( &cbPerFrameDesc, sizeof( D3D11_BUFFER_DESC ) );

	cbPerFrameDesc.Usage = D3D11_USAGE_DYNAMIC;
	cbPerFrameDesc.ByteWidth = sizeof( m_cbPerFrame );
	cbPerFrameDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbPerFrameDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cbPerFrameDesc.MiscFlags = 0;

	hr = pDevice -> CreateBuffer( &cbPerFrameDesc, NULL, &m_pCbPerFrameBuffer );
	if( FAILED ( hr ) )
	{
		return hr;
	}

	return S_OK;
}

//--------------------------------------------------------------------------------------
// Free all allocated resources.
//--------------------------------------------------------------------------------------
HRESULT SimpleVertexShader::Cleanup( void )
{
	if( m_pCbPerFrameBuffer )
	{
		m_pCbPerFrameBuffer -> Release();
		m_pCbPerFrameBuffer = nullptr;
	}
	
	return VertexShader::Cleanup();
}

//--------------------------------------------------------------------------------------
// Update the per-scene constant buffer of the shader.
//--------------------------------------------------------------------------------------
HRESULT SimpleVertexShader::UpdatePerSceneData( ID3D11DeviceContext* pContext, const PerSceneData& perSceneData)
{
	// Buffer not used in this shader
	return E_FAIL;
}

//--------------------------------------------------------------------------------------
// Update the per-frame constant buffer of the shader.
//--------------------------------------------------------------------------------------
HRESULT SimpleVertexShader::UpdatePerFrameData( ID3D11DeviceContext* pContext, const PerFrameData& perFrameData)
{
	HRESULT hr;

	m_cbPerFrame.m_viewProjection = perFrameData.m_viewProjection;

	D3D11_MAPPED_SUBRESOURCE mappedResource;
	hr = pContext -> Map( m_pCbPerFrameBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource );
	if( FAILED( hr ) )
	{
		return hr;
	}

	memcpy( mappedResource.pData, &m_cbPerFrame, sizeof( ConstBufferPerFrame ) );
	pContext -> Unmap( m_pCbPerFrameBuffer, 0 );

	pContext -> VSSetConstantBuffers( 0, 1, &m_pCbPerFrameBuffer );

	return S_OK;
}

//--------------------------------------------------------------------------------------
// Update the per-object constant buffer of the shader.
//--------------------------------------------------------------------------------------
HRESULT SimpleVertexShader::UpdatePerObjectData( ID3D11DeviceContext* pContext, const PerObjectData& perObjectData)
{
	// Not used in this shader
	return E_FAIL;
}

//--------------------------------------------------------------------------------------
// Update the texture and corresponding sample state being used by the shader.
//--------------------------------------------------------------------------------------
HRESULT SimpleVertexShader::UpdateTextureResource( int index, ID3D11DeviceContext* pContext, ID3D11ShaderResourceView* pTexture, ID3D11SamplerState* pSamplerState )
{
	// Not used in this shader
	return E_FAIL;
}
