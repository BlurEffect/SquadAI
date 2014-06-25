/* 
*  Kevin Meergans, SquadAI, 2014
*  FontVertexShader.h
*  Wrapper for the "VS_Font.FX" vertex shader.
*  Vertex shader for text rendering.
*/

// Includes
#include "FontVertexShader.h"

// The vertex input layout expected by this vertex shader
const D3D11_INPUT_ELEMENT_DESC FontVertexShader::m_sInputLayoutDescription[] = 
{ 
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
};

FontVertexShader::FontVertexShader(void)
{
	// Initialise constant buffer data
	XMStoreFloat4x4( &m_cbPerFrameData.m_viewProjection, XMMatrixIdentity() );
}

FontVertexShader::~FontVertexShader(void)
{
	Cleanup();
}

//--------------------------------------------------------------------------------------
// Initialise the shader's member variables.
// Param1: The device used for initialisation
// Returns true if the initialisation of the shader was successful, false otherwise.
//--------------------------------------------------------------------------------------
bool FontVertexShader::Initialise(ID3D11Device* pDevice)
{
	// Create the underlying shader
	if(FAILED(pDevice->CreateVertexShader(g_VS_FontCompiled, sizeof(g_VS_FontCompiled), nullptr, &m_pVertexShader)))
	{
		return false;
	}
	
	// Create the vertex input layout
	UINT numElements = ARRAYSIZE(m_sInputLayoutDescription);
	if(FAILED(pDevice->CreateInputLayout(m_sInputLayoutDescription, numElements, g_VS_FontCompiled, sizeof(g_VS_FontCompiled), &m_pInputLayout)))
	{
		return false;
	}

	// Initialise the constant buffer
	return m_cbPerFrame.Initialise(ConstantBuffer, pDevice, D3D11_USAGE_DYNAMIC, nullptr, 1);
}

//--------------------------------------------------------------------------------------
// Free all allocated resources.
//--------------------------------------------------------------------------------------
void FontVertexShader::Cleanup(void)
{
	m_cbPerFrame.Cleanup();
	VertexShader::Cleanup();
}

//--------------------------------------------------------------------------------------
// Update the per-frame constant buffer of the shader.
// Param1: The device context used to update the constant buffer.
// Param2: The structure holding the shader parameters for the current frame.
// Returns true if the per-frame parameters of the shader were updated successfully (that is also true
// in the case that the shader doesn't actually use per-frame parameters), false if the update failed.
//--------------------------------------------------------------------------------------
bool FontVertexShader::SetFrameData(ID3D11DeviceContext* pContext, const PerFrameData& perFrameData)
{
	m_cbPerFrameData.m_viewProjection = perFrameData.m_viewProjectionText;

	if(!m_cbPerFrame.Update(pContext, &m_cbPerFrameData, 1))
	{
		return false;
	}

	pContext -> VSSetConstantBuffers( 0, 1, m_cbPerFrame.GetBuffer() );

	return true;
}