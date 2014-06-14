/* 
*  Kevin Meergans, SquadAI, 2014
*  VertexShader.h
*  Abstract base class for vertex shader classes. Inherits from Shader.
*  It provides default implementations for some functions of the Shader class.
*/

#ifndef VERTEX_SHADER_H
#define VERTEX_SHADER_H

// Includes
#include <Windows.h>
#include <D3D11.h>
#include "Shader.h"

class VertexShader : public Shader
{
public:
	VertexShader(void);
	virtual ~VertexShader(void);
	virtual void Cleanup(void);
	virtual void Activate(ID3D11DeviceContext* pContext);

protected:
	ID3D11VertexShader*	m_pVertexShader; // Pointer to the underlying pixel shader
	ID3D11InputLayout*  m_pInputLayout;
};

#endif // VERTEX_SHADER_H