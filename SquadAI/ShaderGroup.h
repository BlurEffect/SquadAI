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
	Font,
	NumberOfShaderTypes
};

class ShaderGroup
{
public:
	ShaderGroup(void);
	~ShaderGroup(void);

	bool Initialise(ID3D11Device* pDevice, ShaderType shaderType);
	void Cleanup(void);
	bool SetFrameData(ID3D11DeviceContext* pContext, const PerFrameData& perFrameData);
	bool SetObjectData(ID3D11DeviceContext* pContext, const PerObjectData& perObjectData);
	void Activate(ID3D11DeviceContext* pContext);
private:
	VertexShader* m_pVertexShader;
	PixelShader*  m_pPixelShader;

	ShaderType    m_type;
	bool		  m_isInitialised;
};

#endif // SHADER_GROUP_H