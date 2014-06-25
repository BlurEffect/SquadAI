/* 
*  Kevin Meergans, SquadAI, 2014
*  SimpleVertexShader.cpp
*  Wrapper for the "VS_Simple.FX" vertex shader.
*  Performs standard transformation of the vertex position and forwards
*  the result together with the object colour to the next stage.
*/

// Includes
#include "SimpleVertexShader.h"

// The vertex input layout expected by this vertex shader
const D3D11_INPUT_ELEMENT_DESC SimpleVertexShader::m_sInputLayoutDescription[] = 
{ 
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 } 
};

SimpleVertexShader::SimpleVertexShader(void)
{
	// Initialise constant buffer data
	XMStoreFloat4x4( &m_cbPerObjectData.m_worldViewProjection, XMMatrixIdentity() );
	m_cbPerObjectData.m_colour = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
}

SimpleVertexShader::~SimpleVertexShader(void)
{
	Cleanup();
}

//--------------------------------------------------------------------------------------
// Initialise the shader's member variables.
// Param1: The device used for initialisation
// Returns true if the initialisation of the shader was successful, false otherwise.
//--------------------------------------------------------------------------------------
bool SimpleVertexShader::Initialise(ID3D11Device* pDevice)
{
	// Create the underlying shader
	if(FAILED(pDevice->CreateVertexShader(g_VS_SimpleCompiled, sizeof(g_VS_SimpleCompiled), nullptr, &m_pVertexShader)))
	{
		return false;
	}
	
	// Create the vertex input layout
	UINT numElements = ARRAYSIZE(m_sInputLayoutDescription);
	if(FAILED(pDevice->CreateInputLayout(m_sInputLayoutDescription, numElements, g_VS_SimpleCompiled, sizeof(g_VS_SimpleCompiled), &m_pInputLayout)))
	{
		return false;
	}

	// Initialise the constant buffer
	return m_cbPerObject.Initialise(ConstantBuffer, pDevice, D3D11_USAGE_DYNAMIC, nullptr, 1);
}

//--------------------------------------------------------------------------------------
// Free all allocated resources.
//--------------------------------------------------------------------------------------
void SimpleVertexShader::Cleanup(void)
{
	m_cbPerObject.Cleanup();
	VertexShader::Cleanup();
}

//--------------------------------------------------------------------------------------
// Update the per-object constant buffer of the shader.
// Param1: The device context used to update the constant buffer.
// Param2: The structure holding the shader parameters for the current object.
// Returns true if the per-frame parameters of the shader were updated successfully (that is also true
// in the case that the shader doesn't actually use per-object parameters), false if the update failed.
//--------------------------------------------------------------------------------------
bool SimpleVertexShader::SetObjectData(ID3D11DeviceContext* pContext, const PerObjectData& perObjectData)
{
	m_cbPerObjectData.m_worldViewProjection = perObjectData.m_worldViewProjection;
	m_cbPerObjectData.m_colour				= perObjectData.m_colour;

	if(!m_cbPerObject.Update(pContext, &m_cbPerObjectData, 1))
	{
		return false;
	}

	pContext -> VSSetConstantBuffers( 0, 1, m_cbPerObject.GetBuffer() );

	return true;
}
