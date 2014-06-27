/* 
*  Kevin Meergans, SquadAI, 2014
*  Shader.h
*  This is an abstract base class for vertex and pixel shaders that will
*  inherit from this class. Used as public interface.
*/

#ifndef SHADER_H
#define SHADER_H

// Includes
#include <D3D11.h>

// Forward declaration
struct PerFrameData;
struct PerObjectData;

class Shader
{
public:
	Shader(void);
	virtual ~Shader(void);

	virtual bool Initialise (ID3D11Device* pDevice) = 0;
	virtual void Cleanup(void) = 0;
	virtual bool SetFrameData(ID3D11DeviceContext* pContext, const PerFrameData& perFrameData);
	virtual bool SetObjectData(ID3D11DeviceContext* pContext, const PerObjectData& perObjectData);
	virtual bool SetTexture(UINT index, ID3D11DeviceContext* pContext, ID3D11ShaderResourceView* pTexture, ID3D11SamplerState* pSamplerState);
	virtual void Activate(ID3D11DeviceContext* pContext) = 0;
};

#endif // SHADER_H
