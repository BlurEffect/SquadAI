/* 
*  Kevin Meergans, SquadAI, 2014
*  ShaderGroup.cpp
*  Encapsulates a couple of shader of a certain type to be used in
*  conjunction with each other.
*/ 

// Includes
#include "ShaderGroup.h"

ShaderGroup::ShaderGroup(void) : m_pVertexShader(nullptr),
								 m_pInstancedVertexShader(nullptr),
								 m_pPixelShader(nullptr),
								 m_pInstancedPixelShader(nullptr),
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

	m_pVertexShader = ShaderFactory::CreateVertexShader(shaderType, false);
	if(!m_pVertexShader)
	{
		return false;
	}

	m_pInstancedVertexShader = ShaderFactory::CreateVertexShader(shaderType, true);
	if(!m_pInstancedVertexShader)
	{
		return false;
	}

	m_pPixelShader = ShaderFactory::CreatePixelShader(shaderType, false);
	if(!m_pPixelShader)
	{
		return false;
	}

	m_pInstancedPixelShader = ShaderFactory::CreatePixelShader(shaderType, true);
	if(!m_pInstancedPixelShader)
	{
		return false;
	}

	// Initialise the shaders

	return m_pVertexShader->Initialise(pDevice) &&
		   m_pInstancedVertexShader->Initialise(pDevice) &&
		   m_pPixelShader->Initialise(pDevice) &&
		   m_pInstancedPixelShader->Initialise(pDevice);
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

	if(m_pInstancedVertexShader)
	{
		m_pInstancedVertexShader->Cleanup();
		delete m_pInstancedVertexShader;
		m_pInstancedVertexShader = nullptr;
	}

	if(m_pPixelShader)
	{
		m_pPixelShader->Cleanup();
		delete m_pPixelShader;
		m_pPixelShader = nullptr;
	}

	if(m_pInstancedPixelShader)
	{
		m_pInstancedPixelShader->Cleanup();
		delete m_pInstancedPixelShader;
		m_pInstancedPixelShader = nullptr;
	}
}

//--------------------------------------------------------------------------------------
// Update the per-frame parameters of the shaders of this group.
// Param1: The device context used to update the constant buffer.
// Param2: The structure holding the shader parameters for the current frame.
// Param3: Determines whether to update the "normal" version of the shader, the one using hardware instancing or both
// Returns true if the per-frame parameters of the shader were updated successfully (that is also true
// in the case that the shader doesn't actually use per-frame parameters), false if the update failed.
//--------------------------------------------------------------------------------------
bool ShaderGroup::SetFrameData(ID3D11DeviceContext* pContext, const PerFrameData& perFrameData, ShaderRestriction restriction = Both)
{
	bool result = false;

	switch(restriction)
	{
	case OnlyBasic:
		result = (m_pVertexShader->SetFrameData(pContext, perFrameData)) && 
			     (m_pPixelShader->SetFrameData(pContext, perFrameData));
		break;
	case OnlyInstanced:
		result = (m_pInstancedVertexShader->SetFrameData(pContext, perFrameData)) && 
			     (m_pInstancedPixelShader->SetFrameData(pContext, perFrameData));
		break;
	case Both:
		result = (m_pVertexShader->SetFrameData(pContext, perFrameData)) && 
			     (m_pInstancedVertexShader->SetFrameData(pContext, perFrameData)) && 
			     (m_pPixelShader->SetFrameData(pContext, perFrameData)) &&
		         (m_pInstancedPixelShader->SetFrameData(pContext, perFrameData));
		break;
	}

	return result;
}

//--------------------------------------------------------------------------------------
// Update the per-object parameters of the shaders associated to this group.
// Param1: The device context used to update the constant buffer.
// Param2: The structure holding the shader parameters for the current object.
// Param3: Determines whether to update the "normal" version of the shader, the one using hardware instancing or both
// Returns true if the per-frame parameters of the shader were updated successfully (that is also true
// in the case that the shader doesn't actually use per-object parameters), false if the update failed.
//--------------------------------------------------------------------------------------
bool ShaderGroup::SetObjectData(ID3D11DeviceContext* pContext, const PerObjectData& perObjectData, ShaderRestriction restriction = Both)
{
	bool result = false;

	switch(restriction)
	{
	case OnlyBasic:
		result = (m_pVertexShader->SetObjectData(pContext, perObjectData)) && 
			     (m_pVertexShader->SetObjectData(pContext, perObjectData));
		break;
	case OnlyInstanced:
		result = (m_pInstancedVertexShader->SetObjectData(pContext, perObjectData)) && 
			     (m_pInstancedPixelShader->SetObjectData(pContext, perObjectData));
		break;
	case Both:
		result = (m_pVertexShader->SetObjectData(pContext, perObjectData)) && 
			     (m_pInstancedVertexShader->SetObjectData(pContext, perObjectData)) && 
			     (m_pPixelShader->SetObjectData(pContext, perObjectData)) &&
		         (m_pInstancedPixelShader->SetObjectData(pContext, perObjectData));
		break;
	}

	return result;
}

//--------------------------------------------------------------------------------------
// Activate shaders associated to this group.
// Param1: The device context to set the shader on.
// Param2: Determines which version of the shader to activate, normal or instanced
//--------------------------------------------------------------------------------------
void ShaderGroup::Activate(ID3D11DeviceContext* pContext, bool doActivateInstancedVersion)
{
	if(doActivateInstancedVersion)
	{
		m_pVertexShader->Activate(pContext);
		m_pPixelShader->Activate(pContext);
	}else
	{
		m_pInstancedVertexShader->Activate(pContext);
		m_pInstancedPixelShader->Activate(pContext);
	}
}