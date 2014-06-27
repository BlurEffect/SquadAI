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
#include "TriangleDrawable.h"
#include "SquareDrawable.h"
#include "CircleDrawable.h"
#include "GridDrawable.h"
#include "RendererData.h"
#include "ShaderParameters.h"
#include "ShaderGroup.h"
#include "RenderContext.h"
#include "RendererSettings.h"
#include "Font.h"
#include "SentenceDrawable.h"

using namespace DirectX;

class Renderer
{
public:
	Renderer(void);
	~Renderer(void);
	bool Initialise(HWND hWnd, UINT windowWidth, UINT windowHeight, const XMFLOAT4X4& viewMatrix, const XMFLOAT4X4& projectionMatrix);
	void RenderScene(const XMFLOAT4X4& viewMatrix, const XMFLOAT4X4& projectionMatrix);
	void Cleanup(void);
	
	// Data access functions

	RenderContext& GetRenderContext(void);

private:
	bool		InitialiseD3D(HWND hwnd);
	bool		InitialiseRenderStates();
	bool		InitialiseDrawables(void);
	bool		InitialiseShaders(void);
	bool        InitialiseTextRendering(HWND hwnd, const XMFLOAT4X4& baseViewMatrix, const XMFLOAT4X4& baseProjectionMatrix);
	bool        InitialiseSentences(void);

	void        RenderTestEnvironment(const XMFLOAT4X4& viewMatrix, const XMFLOAT4X4& projectionMatrix);
	void        RenderText(const XMFLOAT4X4& orthoMatrix);
	void        UpdateSentences();

	void	    PrepareDefaultGeometryRendering(void);
	void	    PrepareTextRendering(void);

	void        SetShaderGroup(ShaderType type);

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
	ShaderType               m_currentShaderGroup;
	ShaderGroup			     m_shaderGroups[NumberOfShaderTypes];
	PerFrameData			 m_perFrameData; 
	PerObjectData			 m_perObjectData;

	// Text rendering
	ID3D11SamplerState*      m_pFontSamplerState;
	XMFLOAT4X4				 m_baseViewMatrix;
	XMFLOAT4X4				 m_baseProjectionMatrix;
	Font					 m_font;
	SentenceDrawable*		 m_pPermanentSentences[NumberOfSentences];

	// Other
	Drawable*				 m_drawableObjects[NumberOfDrawableTypes];
	RenderContext            m_renderContext;
	UINT                     m_windowWidth;
	UINT                     m_windowHeight;

};

#endif // RENDERER_H