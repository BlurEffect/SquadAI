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

#include "Drawable.h"
#include "DrawableFactory.h"
#include "ObjectRenderData.h"
#include "ShaderParameters.h"

using namespace DirectX;

class Renderer
{
public:
	Renderer(void);
	~Renderer(void);
	bool Initialise(HWND hWnd, int windowWidth, int windowHeight);
	void Update(const XMFLOAT4X4& viewMatrix, const XMFLOAT4X4& projMatrix, const XMFLOAT4X4& orthoProjMatrix, const EditorData& editorData, int numberOfBricks, int numberOfStuds, const PerformanceData& performanceData, const XMFLOAT3& cameraPosition);
	void Cleanup(void);
	
private:
	bool		InitialiseD3D( HWND hwnd, int windowWidth, int windowHeight  );
	bool		InitialiseRenderStates();
	HRESULT		InitialiseScene( int windowWidth, int windowHeight );
	HRESULT		InitialiseShaders();
	HRESULT		InitialiseImageBasedCelShading( int windowWidth, int windowHeight );
	HRESULT     Load3dModels( void );
	void 		UpdateScene( void );
	void		DrawScene( const XMFLOAT4X4& viewMatrix, const XMFLOAT4X4& projMatrix, const XMFLOAT4X4& orthoProjMatrix, const EditorData& editorData, int numberOfBricks, int numberOfStuds, const PerformanceData& performanceData, const XMFLOAT3& cameraPosition );
	void		RenderBricks( XMFLOAT4X4 const * viewMatrix, XMFLOAT4X4 const * projMatrix, const XMFLOAT3& cameraPosition, const EditorData& editorData );
	void		RenderEditorGeometry( XMFLOAT4X4 const * viewMatrix, XMFLOAT4X4 const * projMatrix, const EditorData& editorData );

	bool InitialiseDrawables(void);
	void SetDefaultRenderStates(void);


	void		Prepare3DRendering();
	void		Prepare2DRendering();

	Shader*		CreateShader( Shaders shaderIdentifier );
	void		SwitchShader( ShaderGroups newShaderGroup );
	void		SwitchPass( int newPass );

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

	//RenderContext			m_renderContext;
	//TextRenderer			m_textRenderer;
	//ShaderGroups			m_selectedBrickShaderGroup;
	//ShaderGroups			m_activeShaderGroup;

	//Shader*					m_shaders[NumberOfShaders];
	//ShaderGroup				m_shaderGroups[NumberOfShaderGroups];
	
	Drawable*				m_drawableObjects[NumberOfDrawableTypes];
	PerFrameData			m_perFrameData;
	PerObjectData			m_perObjectData;

	
};

#endif // RENDERER_H