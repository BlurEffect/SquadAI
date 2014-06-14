/* 
*  Kevin Meergans, SquadAI, 2014
*  PixelShader.h
*  Abstract base class for pixel shader classes. Inherits from Shader.
*  It provides default implementations for some functions of the Shader class.
*/

// Includes
#include "PixelShader.h"


PixelShader::PixelShader(void) : m_pPixelShader(nullptr)
{
}

PixelShader::~PixelShader(void)
{
}

//--------------------------------------------------------------------------------------
// Free resources used by the shader.
//--------------------------------------------------------------------------------------
void PixelShader::Cleanup(void)
{
	if(m_pPixelShader)
	{
		m_pPixelShader -> Release();
		m_pPixelShader = nullptr;
	}
}

//--------------------------------------------------------------------------------------
// Set this pixel shader as the active one.
// Param1: The device context to set the shader on.
//--------------------------------------------------------------------------------------
void PixelShader::Activate(ID3D11DeviceContext* pContext)
{
	pContext -> PSSetShader(m_pPixelShader, 0, 0);
}