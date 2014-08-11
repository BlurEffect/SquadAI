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
#include <stdio.h>

#include "Drawable.h"
#include "TriangleDrawable.h"
#include "SquareDrawable.h"
#include "CircleDrawable.h"
#include "GridDrawable.h"
#include "RendererData.h"
#include "ShaderParameters.h"
#include "ShaderGroup.h"
#include "RenderContext.h"
#include "ApplicationSettings.h"
#include "Font.h"
#include "SentenceDrawable.h"
#include "TestEnvironmentData.h"
#include "AppData.h"

using namespace DirectX;

// Includes
#include "GameContext.h"

class Renderer
{
public:
	Renderer(void);
	~Renderer(void);
	bool Initialise(HWND hWnd, UINT windowWidth, UINT windowHeight, const XMFLOAT4X4& viewMatrix, const XMFLOAT4X4& projectionMatrix);
	void RenderScene(const XMFLOAT4X4& viewMatrix, const XMFLOAT4X4& projectionMatrix, const AppData& appData, const GameContext* pGameContext);
	void Cleanup(void);
	
	bool SetupGrid(float gridSize, unsigned int numberOfGridPartitions);

	// Data access functions

	RenderContext& GetRenderContext(void);

private:
	bool		InitialiseD3D(HWND hwnd);
	bool		InitialiseRenderStates(void);
	bool		InitialiseDrawables(void);
	bool        InitialiseObjectRenderData(void);
	bool		InitialiseShaders(void);
	bool        InitialiseTextRendering(HWND hwnd, const XMFLOAT4X4& baseViewMatrix, const XMFLOAT4X4& baseProjectionMatrix);
	bool        InitialiseSentences(void);

	void        RenderTestEnvironment(const XMFLOAT4X4& viewMatrix, const XMFLOAT4X4& projectionMatrix);
	void        RenderText(const AppData& appData, const GameContext* pGameContext);
	void        UpdateSentences(const AppData& appData, const GameContext* pGameContext);

	void	    PrepareDefaultGeometryRendering(void);
	void	    PrepareTextRendering(void);

	void        SetShaderGroup(ShaderType type);

	// Initialise D3D
	IDXGISwapChain*			 m_pSwapChain;							   // The swap chain used by the renderer 
	ID3D11Device*			 m_pD3d11Device;						   // The device used by the renderer
	ID3D11DeviceContext*	 m_pD3d11DeviceContext;					   // The device context used by the renderer
	ID3D11RenderTargetView*  m_pRenderTargetView;					   // The render target view used by the renderer
	ID3D11DepthStencilView*  m_pDepthStencilView;					   // The depth stencil view used by the renderer
	ID3D11Texture2D*		 m_pDepthStencilBuffer;					   // The depth stencil buffer used by the renderer

	// Render states
	ID3D11DepthStencilState* m_pDepthEnabledStencilState;              // Depth-Stencil state with enabled depth
	ID3D11DepthStencilState* m_pDepthDisabledStencilState;             // Depth-Stencil state with disabled depth
	ID3D11RasterizerState*	 m_pRasterStateCullBackfaces;              // Raster state with enabled backface culling
	ID3D11BlendState*		 m_pBlendingEnabledBlendingState;          // Blending state with enabled blending
	ID3D11BlendState*		 m_pBlendingDisabledBlendingState;         // Blending state with disabled blending

	// Shader stuff
	ShaderType               m_currentShaderGroup;					   // Identifies the currently active shader group
	ShaderGroup			     m_shaderGroups[NumberOfShaderTypes];      // An array containing all shader groups available to the renderer
	PerFrameData			 m_perFrameData;                           // Stores all shader parameters that have to be updated on a per-frame basis
	PerObjectData			 m_perObjectData;                          // Stores all shader parameters that have to be updated on a per-object basis

	// Text rendering
	ID3D11SamplerState*      m_pFontSamplerState;                      // The sampler state used to sample the font texture
	XMFLOAT4X4				 m_baseViewMatrix;                         // The original view matrix, used to transform text vertices
	XMFLOAT4X4				 m_baseProjectionMatrix;				   // The original projection matrix, used to transform text vertices
	Font					 m_font;								   // The font used for text rendering
	SentenceDrawable*		 m_pPermanentSentences[NumberOfSentences]; // An array of text drawables that are permanently displayed on the screen

	// Other
	Drawable<Vertex>*		 m_drawableObjects[NumberOfDrawableTypes]; // An array of simple drawables 
	RenderContext            m_renderContext;                          // The render context is used collects render instance data of drawables to be rendered 
	UINT                     m_windowWidth;                            // The width of the window the application is running in
	UINT                     m_windowHeight;						   // The height of the window the application is running in
	ObjectRenderData         m_objectRenderData[NumberOfObjectTypes];  // Contains information on how to render the different entities
};

#endif // RENDERER_H