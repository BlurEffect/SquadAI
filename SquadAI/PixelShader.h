/* 
*  Kevin Meergans, SquadAI, 2014
*  PixelShader.h
*  Abstract base class for pixel shader classes. Inherits from Shader.
*  It provides default implementations for some functions of the Shader class.
*/

#ifndef PIXEL_SHADER_H
#define PIXEL_SHADER_H

// Includes
#include <Windows.h>
#include <D3D11.h>
#include "Shader.h"

class PixelShader : public Shader
{
public:
	PixelShader(void);
	virtual ~PixelShader(void);
	virtual void Cleanup(void);
	virtual void Activate(ID3D11DeviceContext* pContext);

protected:
	ID3D11PixelShader*			m_pPixelShader; // Pointer to the underlying pixel shader
};


#endif // PIXEL_SHADER_H