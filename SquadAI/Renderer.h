/* 
*  Kevin Meergans, SquadAI, 2014
*  Renderer.h
*  Sets up and manages DirectX 11 for use by the application and is 
*  responsible for drawing the scene.
*/

#ifndef RENDERER_H
#define RENDERER_H

#include <windows.h>
#include <d3d11.h>
#include <DirectXMath.h>
//#include <DirectXPackedVector.h>
//#include <xnamath.h>
//#include <d3dx11.h>
//#include <d3dx10.h>

using namespace DirectX;

class Renderer
{
public:
	Renderer();
	
	HRESULT		Initialise( HWND hWnd, int windowWidth, int windowHeight, const XMFLOAT4X4& baseViewMatrix );
	HRESULT		Cleanup( void );
	void		Update( const XMFLOAT4X4& viewMatrix, const XMFLOAT4X4& projMatrix, const XMFLOAT4X4& orthoProjMatrix, const EditorData& editorData, int numberOfBricks, int numberOfStuds, const PerformanceData& performanceData, const XMFLOAT3& cameraPosition );

	RenderContext* GetRenderContext( void );

private:
	HRESULT		InitialiseDevice( HWND hwnd, int windowWidth, int windowHeight  );
	HRESULT		InitialiseScene( int windowWidth, int windowHeight );
	HRESULT		InitialiseShaders();
	HRESULT		InitialiseImageBasedCelShading( int windowWidth, int windowHeight );
	HRESULT     Load3dModels( void );
	void 		UpdateScene( void );
	void		DrawScene( const XMFLOAT4X4& viewMatrix, const XMFLOAT4X4& projMatrix, const XMFLOAT4X4& orthoProjMatrix, const EditorData& editorData, int numberOfBricks, int numberOfStuds, const PerformanceData& performanceData, const XMFLOAT3& cameraPosition );
	void		RenderBricks( XMFLOAT4X4 const * viewMatrix, XMFLOAT4X4 const * projMatrix, const XMFLOAT3& cameraPosition, const EditorData& editorData );
	void		RenderEditorGeometry( XMFLOAT4X4 const * viewMatrix, XMFLOAT4X4 const * projMatrix, const EditorData& editorData );

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

	ID3D11DepthStencilState* m_depthStencilState;
	ID3D11DepthStencilState* m_depthDisabledStencilState;

	ID3D11RasterizerState*	 m_pRasterStateCullBackfaces;
	ID3D11RasterizerState*	 m_pRasterStateCullFrontfaces;
	
	ID3D11BlendState*		 m_alphaEnableBlendingState;
	ID3D11BlendState*		 m_alphaDisableBlendingState;

	// Initialise scene

	/*
	ID3D11VertexShader*		m_pVertexShader;
	ID3D11PixelShader*		m_pPixelShader;
	ID3D11VertexShader*		m_pSimpleVertexShader;
	ID3D11PixelShader*		m_pSimplePixelShader;
	ID3D10Blob*				m_pVertexShaderBuffer;
	ID3D10Blob*				m_pPixelShaderBuffer;
	ID3D10Blob*				m_pSimpleVertexShaderBuffer;
	ID3D10Blob*				m_pSimplePixelShaderBuffer;
	*/
	//ID3D11InputLayout*		m_pVertexLayout;
	//ID3D11InputLayout*		m_pSimpleVertexLayout;
	
	/*
	ConstBufferPerSimpleObject	m_cbPerObjSimple;
	ConstBufferPerLightedObject	m_cbPerObjLighted;
	ConstBufferPerSceneColour			m_cbPerSceneColour;
	ConstBufferPerSceneEdges			m_cbPerSceneEdges;
	ConstBufferPerFrame			m_cbPerFrame;

	ID3D11Buffer*			m_pCbPerObjectBufferSimple;
	ID3D11Buffer*			m_pCbPerObjectBufferLighted;
	ID3D11Buffer*			m_pCbPerSceneBufferColour;
	ID3D11Buffer*			m_pCbPerSceneBufferEdges;
	ID3D11Buffer*			m_pCbPerFrameBuffer;
	*/
	ID3D11ShaderResourceView*	m_pToneTexture;
	ID3D11SamplerState*			m_pToneTexSamplerState;

	RenderContext			m_renderContext;
	TextRenderer			m_textRenderer;

	// Keeps track of the number of instances to be drawn of every brick type
	// int		m_instanceCounts[NumberOfBrickTypes];
	// The instances to be drawn for every brick type
	// Instance		m_instances[NumberOfBrickTypes][NUMBER_OF_BRICKS_PER_TYPE];
	int						m_brickRenderCount;
	int						m_studRenderCount;
	Grid					m_grid;
	Cursor					m_cursor;
	Mesh					m_legoBrickMeshes[NumberOfBrickTypes];
	Mesh					m_legoStudMesh;
		
	ShaderGroups			m_selectedBrickShaderGroup;
	ShaderGroups			m_activeShaderGroup;

	Shader*					m_shaders[NumberOfShaders];
	ShaderGroup				m_shaderGroups[NumberOfShaderGroups];

	PerSceneData			m_perSceneData;
	PerFrameData			m_perFrameData;
	PerObjectData			m_perObjectData;

	/*
	ID3D11VertexShader*		m_vertexShaders[NumberOfShaders];
	ID3D11PixelShader*		m_pixelShaders[NumberOfShaders];
	ID3D11InputLayout*		m_vertexInputLayouts[NumberOfShaders];
	*/

	// for image-based cel shading

	// The texture to hold the cel-shaded colour information
	ID3D11Texture2D*			m_pColourMapRenderTargetTexture;
	ID3D11RenderTargetView*		m_pColourMapRenderTargetView;
	ID3D11ShaderResourceView*	m_pColourMapShaderResourceView;
	ID3D11SamplerState*			m_pColourMapSamplerState;

	// The texture to hold the normal and depth information of the scene
	ID3D11Texture2D*			m_pNormalDepthMapRenderTargetTexture;
	ID3D11RenderTargetView*		m_pNormalDepthMapRenderTargetView;
	ID3D11ShaderResourceView*	m_pNormalDepthMapShaderResourceView;
	ID3D11SamplerState*			m_pNormalDepthMapSamplerState;

	ID3D11RenderTargetView*		m_pCelShadingRenderTargets[2];

	ScreenQuad					m_screenQuad;
};

#endif // RENDERER_H