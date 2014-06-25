/* 
*  Kevin Meergans, SquadAI, 2014
*  ShaderGroup.cpp
*  Encapsulates a couple of shader of a certain type to be used in
*  conjunction with each other.
*/ 

// Includes
#include "ShaderGroup.h"
#include "ShaderFactory.h"

ShaderGroup::ShaderGroup(void) : m_pVertexShader(nullptr),
								 m_pPixelShader(nullptr),
								 m_type(ShaderType(0)),
								 m_isInitialised(false)
{
}

ShaderGroup::~ShaderGroup(void)
{
	Cleanup();
}

//--------------------------------------------------------------------------------------
// Creates and initialises all shaders associated to this group.
// Param1: The device to use for initialisation of the shaders.
// Param2: Denotes the type of shaders that should be associated to this group.
// Returns true if all associated shaders of the requested type were created and
// initialised successfully, false otherwise.
//--------------------------------------------------------------------------------------
bool ShaderGroup::Initialise(ID3D11Device* pDevice, ShaderType shaderType)
{
	// Create the shaders

	m_pVertexShader = ShaderFactory::CreateVertexShader(shaderType);
	if(!m_pVertexShader)
	{
		return false;
	}

	m_pPixelShader = ShaderFactory::CreatePixelShader(shaderType);
	if(!m_pPixelShader)
	{
		return false;
	}

	// Initialise the shaders

	return m_pVertexShader->Initialise(pDevice) &&
		   m_pPixelShader->Initialise(pDevice);
}

//--------------------------------------------------------------------------------------
// Free all allocated resources.
//--------------------------------------------------------------------------------------
void ShaderGroup::Cleanup(void)
{
	if(m_pVertexShader)
	{
		m_pVertexShader->Cleanup();
		delete m_pVertexShader;
		m_pVertexShader = nullptr;
	}

	if(m_pPixelShader)
	{
		m_pPixelShader->Cleanup();
		delete m_pPixelShader;
		m_pPixelShader = nullptr;
	}
}

//--------------------------------------------------------------------------------------
// Update the per-frame parameters of the shaders of this group.
// Param1: The device context used to update the constant buffer.
// Param2: The structure holding the shader parameters for the current frame.
// Returns true if the per-frame parameters of the shader were updated successfully (that is also true
// in the case that the shader doesn't actually use per-frame parameters), false if the update failed.
//--------------------------------------------------------------------------------------
bool ShaderGroup::SetFrameData(ID3D11DeviceContext* pContext, const PerFrameData& perFrameData)
{
	return (m_pVertexShader->SetFrameData(pContext, perFrameData)) && 
		   (m_pPixelShader->SetFrameData(pContext, perFrameData));
}

//--------------------------------------------------------------------------------------
// Update the per-object parameters of the shaders associated to this group.
// Param1: The device context used to update the constant buffer.
// Param2: The structure holding the shader parameters for the current object.
// Returns true if the per-frame parameters of the shader were updated successfully (that is also true
// in the case that the shader doesn't actually use per-object parameters), false if the update failed.
//--------------------------------------------------------------------------------------
bool ShaderGroup::SetObjectData(ID3D11DeviceContext* pContext, const PerObjectData& perObjectData)
{
	return (m_pVertexShader->SetObjectData(pContext, perObjectData)) && 
		   (m_pPixelShader->SetObjectData(pContext, perObjectData));
}


//--------------------------------------------------------------------------------------
// Activate shaders associated to this group.
// Param1: The device context to set the shader on.
//--------------------------------------------------------------------------------------
void ShaderGroup::Activate(ID3D11DeviceContext* pContext)
{
	m_pVertexShader->Activate(pContext);
	m_pPixelShader->Activate(pContext);
}