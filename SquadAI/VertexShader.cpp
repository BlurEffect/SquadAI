/* 
*  Kevin Meergans, SquadAI, 2014
*  VertexShader.cpp
*  Abstract base class for vertex shader classes. Inherits from Shader.
*  It provides default implementations for some functions of the Shader class.
*/

// Includes
#include "VertexShader.h"


VertexShader::VertexShader(void) : m_pVertexShader(nullptr),
								   m_pInputLayout(nullptr)								 
{
}

VertexShader::~VertexShader(void)
{
	Cleanup();
}

//--------------------------------------------------------------------------------------
// Free resources used by the shader.
//--------------------------------------------------------------------------------------
void VertexShader::Cleanup(void)
{
	if(m_pVertexShader)
	{
		m_pVertexShader->Release();
		m_pVertexShader = nullptr;
	}
	
	if(m_pInputLayout)
	{
		m_pInputLayout->Release();
		m_pInputLayout = nullptr;
	}
}

//--------------------------------------------------------------------------------------
// Set this vertex shader as the active one.
// Param1: The device context to set the shader on.
//--------------------------------------------------------------------------------------
void VertexShader::Activate(ID3D11DeviceContext* pContext)
{
	// Set the vertex shader
	pContext->VSSetShader(m_pVertexShader, 0, 0);

	// Set the input layout
	pContext->IASetInputLayout(m_pInputLayout);
}