/* 
*  Kevin Meergans, SquadAI, 2014
*  Shader.cpp
*  This is an abstract base class for vertex and pixel shaders that will
*  inherit from this class. Used as public interface.
*/

// Includes
#include "Shader.h"

Shader::Shader(void)
{
}

Shader::~Shader(void)
{
}

//--------------------------------------------------------------------------------------
// Update the per-frame constant buffer of the shader.
// Param1: The device context used to update the constant buffer.
// Param2: The structure holding the shader parameters for the current frame.
// Returns true if the per-frame parameters of the shader were updated successfully (that is also true
// in the case that the shader doesn't actually use per-frame parameters), false if the update failed.
//--------------------------------------------------------------------------------------
bool Shader::SetFrameData(ID3D11DeviceContext* pContext, const PerFrameData& perFrameData)
{
	return true;
}

//--------------------------------------------------------------------------------------
// Update the per-object constant buffer of the shader.
// Param1: The device context used to update the constant buffer.
// Param2: The structure holding the shader parameters for the current object.
// Returns true if the per-frame parameters of the shader were updated successfully (that is also true
// in the case that the shader doesn't actually use per-object parameters), false if the update failed.
//--------------------------------------------------------------------------------------
bool Shader::SetObjectData(ID3D11DeviceContext* pContext, const PerObjectData& perObjectData)
{
	return true;
}

//--------------------------------------------------------------------------------------
// Updates the textures and corresponding sampler states used by the shader.
// Param1: The index of the texture resource of the shader that is to be updated.
// Param2: The device context used to update the texture resource.
// Param3: A pointer to the texture that should be set as resource for the shader.
// Param4: A pointer to the sampler state that should be used with the passed in texture.
// Returns true if the texture resource was set successfully, false otherwise.
//--------------------------------------------------------------------------------------
bool Shader::SetTexture(UINT index, ID3D11DeviceContext* pContext, ID3D11ShaderResourceView* pTexture, ID3D11SamplerState* pSamplerState)
{
	// Note: For more flexibility, there should be separate functions to set ShaderResourceViews and SamplerStates respectively.

	return false;
}
