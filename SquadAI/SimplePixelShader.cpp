/* 
*  Kevin Meergans, SquadAI, 2014
*  SimplePixelShader.cpp
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
	Cleanup();
}

//--------------------------------------------------------------------------------------
// Initialise the shader's member variables.
// Param1: The device used for initialisation
// Returns true if the initialisation of the shader was successful, false otherwise.
//--------------------------------------------------------------------------------------
bool SimplePixelShader::Initialise(ID3D11Device* pDevice)
{
	// Create the shader
	return(SUCCEEDED(pDevice->CreatePixelShader(g_PS_SimpleCompiled, sizeof(g_PS_SimpleCompiled), nullptr, &m_pPixelShader)));
}
