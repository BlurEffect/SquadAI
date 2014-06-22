/* 
*  Kevin Meergans, SquadAI, 2014
*  Renderer.h
*  Sets up and manages DirectX 11 for use by the application and is 
*  responsible for drawing the scene.
*/

#ifndef RENDERER_H
#define RENDERER_H

// Includes
#include <windows.h>
#include <d3d11.h>
#include <DirectXMath.h>
#include <vector>

#include "Drawable.h"
#include "DrawableFactory.h"
#include "RendererData.h"
#include "ShaderParameters.h"
#include "ShaderGroup.h"
#include "RenderContext.h"
#include "RendererSettings.h"

using namespace DirectX;

class Renderer
{
public:
	Renderer(void);
	~Renderer(void);
	bool Initialise(HWND hWnd, int windowWidth, int windowHeight);
	void RenderScene(const XMFLOAT4X4& viewMatrix, const XMFLOAT4X4& projectionMatrix);
	void Cleanup(void);
	
	// Data access functions

	RenderContext& GetRenderContext(void);

private:
	bool		InitialiseD3D( HWND hwnd, int windowWidth, int windowHeight  );
	bool		InitialiseRenderStates();
	bool		InitialiseDrawables(void);
	bool		InitialiseShaders();
	void        RenderTestEnvironment(const XMFLOAT4X4& viewMatrix, const XMFLOAT4X4& projectionMatrix);
	
	void	    SetDefaultRenderStates(void);
	void	    SetShaderGroup(ShaderType type);

	// Initialise D3D
	IDXGISwapChain*			m_pSwapChain;
	ID3D11Device*			m_pD3d11Device;
	ID3D11DeviceContext*	m_pD3d11DeviceContext;
	ID3D11RenderTargetView* m_pRenderTargetView;
	ID3D11DepthStencilView* m_pDepthStencilView;
	ID3D11Texture2D*		m_pDepthStencilBuffer;

	// Render states
	ID3D11DepthStencilState* m_pDepthEnabledStencilState;
	ID3D11DepthStencilState* m_pDepthDisabledStencilState;
	ID3D11RasterizerState*	 m_pRasterStateCullBackfaces;
	ID3D11BlendState*		 m_pBlendingEnabledBlendingState;
	ID3D11BlendState*		 m_pBlendingDisabledBlendingState;

	// Shader stuff
	ShaderGroup*			 m_pCurrentShaderGroup;
	ShaderGroup				 m_shaderGroups[NumberOfShaderTypes]; // Shader groups available to the renderer
	Drawable*				 m_drawableObjects[NumberOfDrawableTypes];
	PerFrameData			 m_perFrameData; 
	PerObjectData			 m_perObjectData;

	// Other
	RenderContext            m_renderContext;

	//RenderContext			m_renderContext;
	//TextRenderer			m_textRenderer;
	//ShaderGroups			m_selectedBrickShaderGroup;
	//ShaderGroups			m_activeShaderGroup;

	//Shader*					m_shaders[NumberOfShaders];
	//ShaderGroup				m_shaderGroups[NumberOfShaderGroups];
};

#endif // RENDERER_H