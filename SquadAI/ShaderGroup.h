/* 
*  Kevin Meergans, SquadAI, 2014
*  ShaderGroup.h
*  Encapsulates a couple of shader of a certain type to be used in
*  conjunction with each other.
*/

#ifndef SHADER_GROUP_H
#define SHADER_GROUP_H

// Includes
#include "VertexShader.h"
#include "PixelShader.h"

//--------------------------------------------------------------------------------------
// Identifies a specific type of shader mostly considering the effect they have.
//--------------------------------------------------------------------------------------
enum ShaderType
{
	SimpleUnlit,
	NumberOfShaderTypes
};

//--------------------------------------------------------------------------------------
// Determines whether operations by the shader group class will be performed on only
// certain associated shaders.
//--------------------------------------------------------------------------------------
enum ShaderRestriction
{
	OnlyBasic,
	OnlyInstanced,
	Both
};

class ShaderGroup
{
public:
	ShaderGroup(void);
	~ShaderGroup(void);

	bool Initialise(ID3D11Device* pDevice, ShaderType shaderType);
	void Cleanup(void);
	bool SetFrameData(ID3D11DeviceContext* pContext, const PerFrameData& perFrameData, ShaderRestriction restriction = Both);
	bool SetObjectData(ID3D11DeviceContext* pContext, const PerObjectData& perObjectData, ShaderRestriction restriction = Both);
	void Activate(ID3D11DeviceContext* pContext, bool doActivateInstancedVersion);
private:
	VertexShader* m_pVertexShader;
	VertexShader* m_pInstancedVertexShader;
	PixelShader*  m_pPixelShader;
	PixelShader*  m_pInstancedPixelShader;

	ShaderType    m_type;
	bool		  m_isInitialised;
};

#endif // SHADER_GROUP_H