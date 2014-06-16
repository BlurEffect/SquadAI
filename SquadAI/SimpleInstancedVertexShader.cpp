/* 
*  Kevin Meergans, SquadAI, 2014
*  SimpleInstancedVertexShader.cpp
*  Wrapper for the "VS_SimpleInstanced.FX" vertex shader.
*  Alternate version to VS_Simple.fx allowing for hardware instancing.
*/

// Includes
#include "SimpleInstancedVertexShader.h"

// The vertex input layout expected by this vertex shader
const D3D11_INPUT_ELEMENT_DESC SimpleInstancedVertexShader::m_sInputLayoutDescription[] = 
{
	// Vertex data
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0,  0, D3D11_INPUT_PER_VERTEX_DATA,   0 },  
	
	// Instance data
	{ "WORLD",	  0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1,  0, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
	{ "WORLD",    1, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
	{ "WORLD",    2, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 32, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
	{ "WORLD",    3, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 48, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
	{ "COLOUR",   0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 64, D3D11_INPUT_PER_INSTANCE_DATA, 1 }
};

SimpleInstancedVertexShader::SimpleInstancedVertexShader(void)
{
	// Initialise constant buffer data
	XMStoreFloat4x4(&m_cbPerFrame.m_viewProjection, XMMatrixIdentity());
}

SimpleInstancedVertexShader::~SimpleInstancedVertexShader(void)
{
	Cleanup();
}

//--------------------------------------------------------------------------------------
// Initialise the shader's member variables.
// Param1: The device used for initialisation
// Returns true if the initialisation of the shader was successful, false otherwise.
//--------------------------------------------------------------------------------------
bool SimpleInstancedVertexShader::Initialise(ID3D11Device* pDevice)
{
	// Create the underlying shader
	if(FAILED(pDevice->CreateVertexShader(g_VS_SimpleInstancedCompiled, sizeof(g_VS_SimpleInstancedCompiled), nullptr, &m_pVertexShader)))
	{
		return false;
	}
	
	// Create the vertex input layout
	UINT numElements = ARRAYSIZE(m_sInputLayoutDescription);
	if(FAILED(pDevice->CreateInputLayout(m_sInputLayoutDescription, numElements, g_VS_SimpleInstancedCompiled, sizeof(g_VS_SimpleInstancedCompiled), &m_pInputLayout)))
	{
		return false;
	}

	// Initialise the constant buffer
	return m_pCbPerFrameBuffer.Initialise(ConstantBuffer, D3D11_USAGE_DYNAMIC, nullptr, 1);
}

//--------------------------------------------------------------------------------------
// Free all allocated resources.
//--------------------------------------------------------------------------------------
void SimpleInstancedVertexShader::Cleanup(void)
{
	m_pCbPerFrameBuffer.Cleanup();
	VertexShader::Cleanup();
}

//--------------------------------------------------------------------------------------
// Update the per-frame constant buffer of the shader.
// Param1: The device context used to update the constant buffer.
// Param2: The structure holding the shader parameters for the current frame.
// Returns true if the per-frame parameters of the shader were updated successfully (that is also true
// in the case that the shader doesn't actually use per-frame parameters), false if the update failed.
//--------------------------------------------------------------------------------------
bool SimpleInstancedVertexShader::SetFrameData(ID3D11DeviceContext* pContext, const PerFrameData& perFrameData)
{
	m_cbPerFrame.m_viewProjection = perFrameData.m_viewProjection;

	if(!m_pCbPerFrameBuffer.Update(pContext, &m_cbPerFrame, 1, 0))
	{
		return false;
	}

	pContext -> VSSetConstantBuffers( 0, 1, m_pCbPerFrameBuffer.GetBuffer() );

	return true;
}
