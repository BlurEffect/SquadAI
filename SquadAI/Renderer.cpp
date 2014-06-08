/* 
*  Kevin Meergans, SquadAI, 2014
*  Renderer.cpp
*  Sets up and manages DirectX 11 for use by the application and is 
*  responsible for drawing the scene.
*/

// Includes
#include "Renderer.h"


Renderer::Renderer() : m_pSwapChain( nullptr ),
												   m_pD3d11Device( nullptr ),
												   m_pD3d11DeviceContext( nullptr ),
												   m_pRenderTargetView( nullptr ),
												   m_pDepthStencilView( nullptr ),
												   m_pDepthStencilBuffer( nullptr ),
												   m_depthStencilState( nullptr ),
												   m_depthDisabledStencilState( nullptr ),
												   m_pRasterStateCullBackfaces( nullptr ),
												   m_pRasterStateCullFrontfaces( nullptr ),
												   m_alphaEnableBlendingState( nullptr ),
												   m_alphaDisableBlendingState( nullptr ),
												   m_pToneTexture( nullptr ),
												   m_pToneTexSamplerState( nullptr ),
												   m_brickRenderCount( 0 ),
												   m_selectedBrickShaderGroup( GroupSimple ),
												   m_activeShaderGroup( GroupSimple ),
												   m_pColourMapRenderTargetTexture( nullptr ),
												   m_pColourMapRenderTargetView( nullptr ),
												   m_pColourMapShaderResourceView( nullptr ),
												   m_pColourMapSamplerState( nullptr ),
												   m_pNormalDepthMapRenderTargetTexture( nullptr ),
												   m_pNormalDepthMapRenderTargetView( nullptr ),
												   m_pNormalDepthMapShaderResourceView( nullptr ),
												   m_pNormalDepthMapSamplerState( nullptr )
{
	for(int i = 0; i < NumberOfShaders; ++i)
	{
		m_shaders[i] = nullptr;
	}

	m_pCelShadingRenderTargets[0] = nullptr;
	m_pCelShadingRenderTargets[1] = nullptr;
}


//--------------------------------------------------------------------------------------
// Initialise the renderer by setting up DirectX 11 
//--------------------------------------------------------------------------------------
HRESULT RendererImplementation::Initialise( HWND hWnd, int windowWidth, int windowHeight, const XMFLOAT4X4& baseViewMatrix )
{

	return SUCCEEDED( InitialiseDevice( hWnd, windowWidth, windowHeight )) &&
		   SUCCEEDED( InitialiseScene( windowWidth, windowHeight )) &&
		   SUCCEEDED( m_renderContext.Initialise() ) &&
		   SUCCEEDED( m_textRenderer.Initialise( m_pD3d11Device, m_pD3d11DeviceContext, hWnd, windowWidth, windowHeight, MAX_SENTENCE_LENGTH, baseViewMatrix ) );
}

//--------------------------------------------------------------------------------------
// Create the device and swap chain 
//--------------------------------------------------------------------------------------
HRESULT RendererImplementation::InitialiseDevice( HWND hWnd, int windowWidth, int windowHeight )
{
	// todo: restructure this part, add more hresult tests for state creations

	HRESULT hr = S_OK;

	// Description of the swap chain buffer

	DXGI_MODE_DESC bufferDesc;

	ZeroMemory(&bufferDesc, sizeof(DXGI_MODE_DESC));

	bufferDesc.Width					= windowWidth;
	bufferDesc.Height					= windowHeight;
	bufferDesc.RefreshRate.Numerator	= 60;
	bufferDesc.RefreshRate.Denominator	= 1;
	bufferDesc.Format					= DXGI_FORMAT_R8G8B8A8_UNORM;
	bufferDesc.ScanlineOrdering			= DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	bufferDesc.Scaling					= DXGI_MODE_SCALING_UNSPECIFIED;

	// Description of the swap chain

	DXGI_SWAP_CHAIN_DESC swapChainDesc; 

	ZeroMemory(&swapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));

	swapChainDesc.BufferDesc			= bufferDesc;
	swapChainDesc.SampleDesc.Count		= 1;
	swapChainDesc.SampleDesc.Quality	= 0;
	swapChainDesc.BufferUsage			= DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferCount			= 1;
	swapChainDesc.OutputWindow			= hWnd; 
	swapChainDesc.Windowed				= TRUE; 
	swapChainDesc.SwapEffect			= DXGI_SWAP_EFFECT_DISCARD;

	// Create the swap chain and the device

	// todo: remove debug
	hr = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL,  /*D3D11_CREATE_DEVICE_DEBUG*/NULL, NULL, NULL, D3D11_SDK_VERSION, &swapChainDesc, &m_pSwapChain, &m_pD3d11Device, NULL, &m_pD3d11DeviceContext );
	if( FAILED( hr ) )
		return hr;

	// Create the back buffer
	
	ID3D11Texture2D* pBackBuffer;
	hr = m_pSwapChain -> GetBuffer( 0, __uuidof( ID3D11Texture2D ), (void**)&pBackBuffer );
	if( FAILED ( hr ) )
		return hr;

	hr =  m_pSwapChain -> GetBuffer( 0, __uuidof( ID3D11Texture2D ), (void**)&pBackBuffer );
	if( FAILED (hr ) )
		return hr;

	// Create the render target
	
	hr = m_pD3d11Device -> CreateRenderTargetView( pBackBuffer, NULL, &m_pRenderTargetView );
	if( FAILED ( hr ) )
		return hr;

	pBackBuffer -> Release();

	//Description of the depth and stencil buffers

	D3D11_TEXTURE2D_DESC depthStencilBufferDesc;

	depthStencilBufferDesc.Width				= windowWidth;
	depthStencilBufferDesc.Height				= windowHeight;
	depthStencilBufferDesc.MipLevels			= 1;
	depthStencilBufferDesc.ArraySize			= 1;
	depthStencilBufferDesc.Format				= DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilBufferDesc.SampleDesc.Count   = 1;
	depthStencilBufferDesc.SampleDesc.Quality = 0;
	depthStencilBufferDesc.Usage				= D3D11_USAGE_DEFAULT;
	depthStencilBufferDesc.BindFlags			= D3D11_BIND_DEPTH_STENCIL;
	depthStencilBufferDesc.CPUAccessFlags		= 0; 
	depthStencilBufferDesc.MiscFlags			= 0;


	// Create the depth and stencil view

	m_pD3d11Device -> CreateTexture2D(&depthStencilBufferDesc, NULL, &m_pDepthStencilBuffer);
	m_pD3d11Device -> CreateDepthStencilView(m_pDepthStencilBuffer, NULL, &m_pDepthStencilView);

	// Set the render target

	m_pD3d11DeviceContext -> OMSetRenderTargets(1, &m_pRenderTargetView, m_pDepthStencilView);

	// Create depth/stencil states

	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	ZeroMemory( &depthStencilDesc, sizeof( D3D11_DEPTH_STENCIL_DESC ) );

	// Set up the description of the stencil state.
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;

	// Stencil operations if pixel is front-facing.
	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Stencil operations if pixel is back-facing.
	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Create the depth stencil state.
	m_pD3d11Device -> CreateDepthStencilState(&depthStencilDesc, &m_depthStencilState);


	D3D11_DEPTH_STENCIL_DESC depthDisabledStencilDesc;
	ZeroMemory( &depthDisabledStencilDesc, sizeof( D3D11_DEPTH_STENCIL_DESC ) );

	// Set up the description of the stencil state.
	depthDisabledStencilDesc.DepthEnable = false;
	depthDisabledStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthDisabledStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

	depthDisabledStencilDesc.StencilEnable = true;
	depthDisabledStencilDesc.StencilReadMask = 0xFF;
	depthDisabledStencilDesc.StencilWriteMask = 0xFF;

	// Stencil operations if pixel is front-facing.
	depthDisabledStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthDisabledStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Stencil operations if pixel is back-facing.
	depthDisabledStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthDisabledStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Create the depth stencil state.
	m_pD3d11Device -> CreateDepthStencilState(&depthDisabledStencilDesc, &m_depthDisabledStencilState);


	// setup the rasterizer

	// Create the description for the rasterizer
	D3D11_RASTERIZER_DESC rasterDescCullBack;
	ZeroMemory(&rasterDescCullBack, sizeof(D3D11_RASTERIZER_DESC));

	rasterDescCullBack.AntialiasedLineEnable = false;
	rasterDescCullBack.CullMode = D3D11_CULL_BACK;
	rasterDescCullBack.DepthBias = 0;
	rasterDescCullBack.DepthBiasClamp = 0.0f;
	rasterDescCullBack.DepthClipEnable = true;
	rasterDescCullBack.FillMode = D3D11_FILL_SOLID;
	rasterDescCullBack.FrontCounterClockwise = false;
	rasterDescCullBack.MultisampleEnable = false;
	rasterDescCullBack.ScissorEnable = false;
	rasterDescCullBack.SlopeScaledDepthBias = 0.0f;

	// Create the rasterizer state from the description we just filled out.
	hr = m_pD3d11Device -> CreateRasterizerState( &rasterDescCullBack, &m_pRasterStateCullBackfaces );
	if( FAILED( hr ) )
	{
		return hr;
	}

	D3D11_RASTERIZER_DESC rasterDescCullFront;
	ZeroMemory(&rasterDescCullFront, sizeof(D3D11_RASTERIZER_DESC));

	rasterDescCullFront.AntialiasedLineEnable = false;
	rasterDescCullFront.CullMode = D3D11_CULL_FRONT;
	rasterDescCullFront.DepthBias = 0;
	rasterDescCullFront.DepthBiasClamp = 0.0f;
	rasterDescCullFront.DepthClipEnable = true;
	rasterDescCullFront.FillMode = D3D11_FILL_SOLID;
	rasterDescCullFront.FrontCounterClockwise = false;
	rasterDescCullFront.MultisampleEnable = false;
	rasterDescCullFront.ScissorEnable = false;
	rasterDescCullFront.SlopeScaledDepthBias = 0.0f;

	// Create the rasterizer state from the description we just filled out.
	hr = m_pD3d11Device -> CreateRasterizerState( &rasterDescCullFront, &m_pRasterStateCullFrontfaces );
	if( FAILED( hr ) )
	{
		return hr;
	}


	// Create blend states

	D3D11_BLEND_DESC alphaEnableBlendDesc;
	ZeroMemory(&alphaEnableBlendDesc, sizeof(D3D11_BLEND_DESC));

	// Create an alpha enabled blend state description.
	alphaEnableBlendDesc.RenderTarget[0].BlendEnable = TRUE;
	alphaEnableBlendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	alphaEnableBlendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	alphaEnableBlendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	alphaEnableBlendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	alphaEnableBlendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	alphaEnableBlendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	alphaEnableBlendDesc.RenderTarget[0].RenderTargetWriteMask = 0x0f;

	// Create the blend state using the description.
	m_pD3d11Device -> CreateBlendState( &alphaEnableBlendDesc, &m_alphaEnableBlendingState );

	D3D11_BLEND_DESC alphaDisableBlendDesc;
	ZeroMemory(&alphaDisableBlendDesc, sizeof(D3D11_BLEND_DESC));

	// Create an alpha enabled blend state description.
	alphaDisableBlendDesc.RenderTarget[0].BlendEnable = FALSE;
	alphaDisableBlendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	alphaDisableBlendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	alphaDisableBlendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	alphaDisableBlendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	alphaDisableBlendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	alphaDisableBlendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	alphaDisableBlendDesc.RenderTarget[0].RenderTargetWriteMask = 0x0f;

	// Create the blend state using the description.
	m_pD3d11Device -> CreateBlendState( &alphaDisableBlendDesc, &m_alphaDisableBlendingState );

	m_pD3d11DeviceContext -> RSSetState( m_pRasterStateCullBackfaces );

	Prepare3DRendering();

	return S_OK;
}

//--------------------------------------------------------------------------------------
// Further more scene-related initialisations (creation of shaders, viewport)
//--------------------------------------------------------------------------------------
HRESULT RendererImplementation::InitialiseScene( int windowWidth, int windowHeight )
{
	HRESULT hr = S_OK;

	// Load the brick models
	hr = Load3dModels();
	if( FAILED ( hr ) )
		return hr;

	// Initialise shaders
	hr = InitialiseShaders();
	if( FAILED ( hr ) )
		return hr;

	hr = InitialiseImageBasedCelShading( windowWidth, windowHeight );
	if( FAILED ( hr ) )
		return hr;

	// Create the Viewport
	D3D11_VIEWPORT viewport;
	ZeroMemory( &viewport, sizeof( D3D11_VIEWPORT ) );

	viewport.TopLeftX	= 0;
	viewport.TopLeftY	= 0;
	viewport.Width		= static_cast<float>( windowWidth );
	viewport.Height		= static_cast<float>( windowHeight );
	viewport.MinDepth	= 0.0f;
	viewport.MaxDepth	= 1.0f;

	// Set the Viewport (Bind viewport to rasterizer stage)
	m_pD3d11DeviceContext -> RSSetViewports( 1, &viewport );

	// Initialise tone texture to be used with the toon colour shader 02
	
	hr = D3DX11CreateShaderResourceViewFromFile(m_pD3d11Device, L"ToneTexture3Shades.png", NULL, NULL, &m_pToneTexture, NULL);
	if( FAILED( hr ) )
	{
		return hr;
	}

	// Description of the Sample State
	D3D11_SAMPLER_DESC sampDesc;
	ZeroMemory(&sampDesc, sizeof(sampDesc));
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
    sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
    sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
    sampDesc.MinLOD = 0;
    sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
   
	// Create the Sample State
	hr = m_pD3d11Device -> CreateSamplerState(&sampDesc, &m_pToneTexSamplerState);
	if( FAILED( hr ) )
	{
		return hr;
	}

	// Setup the per scene data (won't change during execution), setting shader parameters

	m_perSceneData.m_diffuseLightColor = DIRECTIONAL_LIGHT_COLOUR;
	m_perSceneData.m_lightDirection	   = DIRECTIONAL_LIGHT_DIRECTION;
	m_perSceneData.m_edgeColour        = EDGE_COLOUR;

	m_perSceneData.m_vertexOffset01 = 0.1f;
	m_perSceneData.m_vertexOffset02 = 0.002f; //0.001

	m_perSceneData.m_edgeThreshold = 0.2f;		

	m_perSceneData.m_moveBias			 = 0.01f; // 0.5f when scaling is applied

	m_perSceneData.m_outlineWidth		 = 0.005f;

	m_perSceneData.m_viewportWidth  = static_cast<float>( windowWidth );
	m_perSceneData.m_viewportHeight = static_cast<float>( windowHeight );
	m_perSceneData.m_thickness      = 1.0f;
	m_perSceneData.m_thresholdNormalsEdges    = 0.2f;
	m_perSceneData.m_thresholdDepthEdges      = 0.005f;  // was 0.05f
	

	return SUCCEEDED( m_grid.Initialise( m_pD3d11Device, XMFLOAT3( 0, 0, 0 ), GRID_SIZE_X, GRID_SIZE_Z, GRID_UNIT.x, GRID_UNIT.z ) ) &&
		   SUCCEEDED( m_cursor.Initialise( m_pD3d11Device, XMFLOAT3( 0, 0, 0 ), XMFLOAT3( GRID_UNIT.x, GRID_UNIT.y, GRID_UNIT.z ) ) );
}

//--------------------------------------------------------------------------------------
// Initialise shaders that will be used by the application.
//--------------------------------------------------------------------------------------
HRESULT RendererImplementation::InitialiseShaders()
{
	HRESULT hr = S_OK;

	// Create shaders and group them in shader groups to be used in conjunction

	// Create and initialise shaders
	for(int i = 0; i < NumberOfShaders; ++i)
	{
		m_shaders[i] = CreateShader( Shaders( i ) );
		m_shaders[i] -> Initialise( m_pD3d11Device );
	}

	// Initialise shader groups with the correct number of passes
	m_shaderGroups[GroupEditorGeometry].Initialise(1);
	m_shaderGroups[GroupSimple].Initialise(1);
	m_shaderGroups[GroupLighted].Initialise(1);
	m_shaderGroups[GroupToon01].Initialise(2);
	m_shaderGroups[GroupToon02].Initialise(2);
	m_shaderGroups[GroupToon03].Initialise(1);
	m_shaderGroups[GroupToon04].Initialise(2);
	m_shaderGroups[GroupToon05].Initialise(2);
	m_shaderGroups[GroupToon06].Initialise(2);

	// Assign shaders to the passes of the shader groups
	// todo: find a better solution that doesn't require this casting stuff (store vertex/pixel shaders separately, one factory for each?

	m_shaderGroups[GroupEditorGeometry].m_pShaderPasses[0].m_vertexShader  = dynamic_cast<VertexShader*>( m_shaders[EditorGeometryVS] );
	m_shaderGroups[GroupEditorGeometry].m_pShaderPasses[0].m_pixelShader   = dynamic_cast<PixelShader*>( m_shaders[EditorGeometryPS] );
	
	m_shaderGroups[GroupSimple].m_pShaderPasses[0].m_vertexShader  = dynamic_cast<VertexShader*>( m_shaders[SimpleVS] );
	m_shaderGroups[GroupSimple].m_pShaderPasses[0].m_pixelShader   = dynamic_cast<PixelShader*>( m_shaders[SimplePS] );

	m_shaderGroups[GroupLighted].m_pShaderPasses[0].m_vertexShader = dynamic_cast<VertexShader*>( m_shaders[LightedVS] );
	m_shaderGroups[GroupLighted].m_pShaderPasses[0].m_pixelShader  = dynamic_cast<PixelShader*>( m_shaders[LightedPS] );

	m_shaderGroups[GroupToon01].m_pShaderPasses[0].m_vertexShader  = dynamic_cast<VertexShader*>( m_shaders[ToonEdges01VS] );
	m_shaderGroups[GroupToon01].m_pShaderPasses[0].m_pixelShader   = dynamic_cast<PixelShader*>( m_shaders[ToonEdges01PS] );
	m_shaderGroups[GroupToon01].m_pShaderPasses[1].m_vertexShader  = dynamic_cast<VertexShader*>( m_shaders[ToonColour01VS] );
	m_shaderGroups[GroupToon01].m_pShaderPasses[1].m_pixelShader   = dynamic_cast<PixelShader*>( m_shaders[ToonColour01PS] );

	m_shaderGroups[GroupToon02].m_pShaderPasses[0].m_vertexShader  = dynamic_cast<VertexShader*>( m_shaders[ToonEdges02VS] );
	m_shaderGroups[GroupToon02].m_pShaderPasses[0].m_pixelShader   = dynamic_cast<PixelShader*>( m_shaders[ToonEdges02PS] );
	m_shaderGroups[GroupToon02].m_pShaderPasses[1].m_vertexShader  = dynamic_cast<VertexShader*>( m_shaders[ToonColour02VS] );
	m_shaderGroups[GroupToon02].m_pShaderPasses[1].m_pixelShader   = dynamic_cast<PixelShader*>( m_shaders[ToonColour02PS] );
	
	m_shaderGroups[GroupToon03].m_pShaderPasses[0].m_vertexShader  = dynamic_cast<VertexShader*>( m_shaders[Toon03VS] );
	m_shaderGroups[GroupToon03].m_pShaderPasses[0].m_pixelShader   = dynamic_cast<PixelShader*>( m_shaders[Toon03PS] );

	m_shaderGroups[GroupToon04].m_pShaderPasses[0].m_vertexShader  = dynamic_cast<VertexShader*>( m_shaders[ToonColour04VS] );
	m_shaderGroups[GroupToon04].m_pShaderPasses[0].m_pixelShader   = dynamic_cast<PixelShader*>( m_shaders[ToonColour04PS] );
	m_shaderGroups[GroupToon04].m_pShaderPasses[1].m_vertexShader  = dynamic_cast<VertexShader*>( m_shaders[ToonEdges04VS] );
	m_shaderGroups[GroupToon04].m_pShaderPasses[1].m_pixelShader   = dynamic_cast<PixelShader*>( m_shaders[ToonEdges04PS] );
	
	m_shaderGroups[GroupToon05].m_pShaderPasses[0].m_vertexShader  = dynamic_cast<VertexShader*>( m_shaders[ToonColour05VS] );
	m_shaderGroups[GroupToon05].m_pShaderPasses[0].m_pixelShader   = dynamic_cast<PixelShader*>( m_shaders[ToonColour05PS] );
	m_shaderGroups[GroupToon05].m_pShaderPasses[1].m_vertexShader  = dynamic_cast<VertexShader*>( m_shaders[ToonEdges05VS] );
	m_shaderGroups[GroupToon05].m_pShaderPasses[1].m_pixelShader   = dynamic_cast<PixelShader*>( m_shaders[ToonEdges05PS] );
	
	m_shaderGroups[GroupToon06].m_pShaderPasses[0].m_vertexShader  = dynamic_cast<VertexShader*>( m_shaders[CreateMapsToon06VS] );
	m_shaderGroups[GroupToon06].m_pShaderPasses[0].m_pixelShader   = dynamic_cast<PixelShader*>( m_shaders[CreateMapsToon06PS] );
	m_shaderGroups[GroupToon06].m_pShaderPasses[1].m_vertexShader  = dynamic_cast<VertexShader*>( m_shaders[ComposeImageToon06VS] );
	m_shaderGroups[GroupToon06].m_pShaderPasses[1].m_pixelShader   = dynamic_cast<PixelShader*>( m_shaders[ComposeImageToon06PS] );
	
	return S_OK;
}

//--------------------------------------------------------------------------------------
// Initialise the textures and associated views requied for image-based cel shading
//--------------------------------------------------------------------------------------
HRESULT	RendererImplementation::InitialiseImageBasedCelShading( int windowWidth, int windowHeight )
{
	HRESULT hr;
	
	hr = m_screenQuad.Initialise( m_pD3d11Device );
	if( FAILED( hr ) )
	{
		return hr;
	}

	// Initialise textures and views for the colour map

	// Initialize the render target texture description.
	D3D11_TEXTURE2D_DESC colourTextureDesc;
	ZeroMemory( &colourTextureDesc, sizeof( colourTextureDesc ) );

	// Setup the render target texture description.
	colourTextureDesc.Width				= windowWidth;
	colourTextureDesc.Height			= windowHeight;
	colourTextureDesc.MipLevels			= 1;
	colourTextureDesc.ArraySize			= 1;
	colourTextureDesc.Format			= DXGI_FORMAT_R32G32B32A32_FLOAT;
	colourTextureDesc.SampleDesc.Count	= 1;
	colourTextureDesc.Usage				= D3D11_USAGE_DEFAULT;
	colourTextureDesc.BindFlags			= D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	colourTextureDesc.CPUAccessFlags	= 0;
	colourTextureDesc.MiscFlags			= 0;

	// Create the render target texture.
	hr = m_pD3d11Device -> CreateTexture2D( &colourTextureDesc, NULL, &m_pColourMapRenderTargetTexture );
	if( FAILED( hr ) )
	{
		return hr;
	}
	
	D3D11_RENDER_TARGET_VIEW_DESC colourRenderTargetViewDesc;
	ZeroMemory( &colourRenderTargetViewDesc, sizeof( colourRenderTargetViewDesc ) );

	// Setup the description of the render target view.
	colourRenderTargetViewDesc.Format = colourTextureDesc.Format;
	colourRenderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	colourRenderTargetViewDesc.Texture2D.MipSlice = 0;

	// Create the render target view.
	hr = m_pD3d11Device -> CreateRenderTargetView( m_pColourMapRenderTargetTexture, &colourRenderTargetViewDesc, &m_pColourMapRenderTargetView );
	if( FAILED( hr ) )
	{
		return hr;
	}

	D3D11_SHADER_RESOURCE_VIEW_DESC colourShaderResourceViewDesc;
	ZeroMemory( &colourShaderResourceViewDesc, sizeof( colourShaderResourceViewDesc ) );

	// Setup the description of the shader resource view.
	colourShaderResourceViewDesc.Format = colourTextureDesc.Format;
	colourShaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	colourShaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
	colourShaderResourceViewDesc.Texture2D.MipLevels = 1;

	// Create the shader resource view.
	hr = m_pD3d11Device->CreateShaderResourceView( m_pColourMapRenderTargetTexture, &colourShaderResourceViewDesc, &m_pColourMapShaderResourceView );
	if( FAILED( hr ) )
	{
		return hr;
	}

	// Create the samplerstate for the colour map
	// Description of the Sample State
	D3D11_SAMPLER_DESC colourMapSampDesc;
	ZeroMemory( &colourMapSampDesc, sizeof( colourMapSampDesc ) );

	colourMapSampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	colourMapSampDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	colourMapSampDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
    colourMapSampDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
    colourMapSampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
    colourMapSampDesc.MinLOD = 0;
    colourMapSampDesc.MaxLOD = D3D11_FLOAT32_MAX;

	// Create the Sample State
	hr = m_pD3d11Device -> CreateSamplerState( &colourMapSampDesc, &m_pColourMapSamplerState );
	if( FAILED( hr ) )
	{
		return hr;
	}

	// Initialise textures and views for the normal/depth map

	// Initialize the render target texture description.
	D3D11_TEXTURE2D_DESC textureDesc;
	ZeroMemory( &textureDesc, sizeof( textureDesc ) );

	// Setup the render target texture description.
	textureDesc.Width				= windowWidth;
	textureDesc.Height				= windowHeight;
	textureDesc.MipLevels			= 1;
	textureDesc.ArraySize			= 1;
	textureDesc.Format				= DXGI_FORMAT_R32G32B32A32_FLOAT;
	textureDesc.SampleDesc.Count	= 1;
	textureDesc.Usage				= D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags			= D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags		= 0;
	textureDesc.MiscFlags			= 0;

	// Create the render target texture.
	hr = m_pD3d11Device -> CreateTexture2D( &textureDesc, NULL, &m_pNormalDepthMapRenderTargetTexture );
	if( FAILED( hr ) )
	{
		return hr;
	}
	
	D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
	ZeroMemory( &renderTargetViewDesc, sizeof( renderTargetViewDesc ) );

	// Setup the description of the render target view.
	renderTargetViewDesc.Format = textureDesc.Format;
	renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	renderTargetViewDesc.Texture2D.MipSlice = 0;

	// Create the render target view.
	hr = m_pD3d11Device -> CreateRenderTargetView( m_pNormalDepthMapRenderTargetTexture, &renderTargetViewDesc, &m_pNormalDepthMapRenderTargetView );
	if( FAILED( hr ) )
	{
		return hr;
	}

	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
	ZeroMemory( &shaderResourceViewDesc, sizeof( shaderResourceViewDesc ) );

	// Setup the description of the shader resource view.
	shaderResourceViewDesc.Format = textureDesc.Format;
	shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
	shaderResourceViewDesc.Texture2D.MipLevels = 1;

	// Create the shader resource view.
	hr = m_pD3d11Device->CreateShaderResourceView( m_pNormalDepthMapRenderTargetTexture, &shaderResourceViewDesc, &m_pNormalDepthMapShaderResourceView );
	if( FAILED( hr ) )
	{
		return hr;
	}

	// Create the samplerstate for the colour map
	// Description of the Sample State
	D3D11_SAMPLER_DESC normalDepthMapSampDesc;
	ZeroMemory( &normalDepthMapSampDesc, sizeof( normalDepthMapSampDesc ) );
	normalDepthMapSampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	normalDepthMapSampDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	normalDepthMapSampDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
    normalDepthMapSampDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
    normalDepthMapSampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
    normalDepthMapSampDesc.MinLOD = 0;
    normalDepthMapSampDesc.MaxLOD = D3D11_FLOAT32_MAX;
   
	// Create the Sample State
	hr = m_pD3d11Device -> CreateSamplerState( &normalDepthMapSampDesc, &m_pNormalDepthMapSamplerState );
	if( FAILED( hr ) )
	{
		return hr;
	}

	// Prepare the render targets array
	m_pCelShadingRenderTargets[0] = m_pColourMapRenderTargetView;
	m_pCelShadingRenderTargets[1] = m_pNormalDepthMapRenderTargetView;

	return S_OK;
}

//--------------------------------------------------------------------------------------
// Load in the 3d models of the lego bricks from the respective files.
//--------------------------------------------------------------------------------------
HRESULT RendererImplementation::Load3dModels( void )
{
	return SUCCEEDED( m_legoBrickMeshes[Std1x1x1].Initialise( m_pD3d11Device, "Models/LegoBrick_Std1x1x1_NoStud.txt", NUMBER_OF_BRICKS_PER_TYPE ) ) &&
		   SUCCEEDED( m_legoBrickMeshes[Std1x3x1].Initialise( m_pD3d11Device, "Models/LegoBrick_Std1x3x1_NoStud.txt", NUMBER_OF_BRICKS_PER_TYPE  ) ) &&
		   SUCCEEDED( m_legoBrickMeshes[Std1x1x2].Initialise( m_pD3d11Device, "Models/LegoBrick_Std1x1x2_NoStud.txt", NUMBER_OF_BRICKS_PER_TYPE  ) ) &&
		   SUCCEEDED( m_legoBrickMeshes[Std1x3x2].Initialise( m_pD3d11Device, "Models/LegoBrick_Std1x3x2_NoStud.txt", NUMBER_OF_BRICKS_PER_TYPE  ) ) &&
		   SUCCEEDED( m_legoBrickMeshes[Std2x1x2].Initialise( m_pD3d11Device, "Models/LegoBrick_Std2x1x2_NoStud.txt", NUMBER_OF_BRICKS_PER_TYPE  ) ) &&
		   SUCCEEDED( m_legoBrickMeshes[Std2x3x2].Initialise( m_pD3d11Device, "Models/LegoBrick_Std2x3x2_NoStud.txt", NUMBER_OF_BRICKS_PER_TYPE  ) ) &&
		   SUCCEEDED( m_legoBrickMeshes[Std1x1x3].Initialise( m_pD3d11Device, "Models/LegoBrick_Std1x1x3_NoStud.txt", NUMBER_OF_BRICKS_PER_TYPE  ) ) &&
		   SUCCEEDED( m_legoBrickMeshes[Std1x3x3].Initialise( m_pD3d11Device, "Models/LegoBrick_Std1x3x3_NoStud.txt", NUMBER_OF_BRICKS_PER_TYPE  ) ) &&
		   SUCCEEDED( m_legoBrickMeshes[Std2x1x3].Initialise( m_pD3d11Device, "Models/LegoBrick_Std2x1x3_NoStud.txt", NUMBER_OF_BRICKS_PER_TYPE  ) ) &&
		   SUCCEEDED( m_legoBrickMeshes[Std2x3x3].Initialise( m_pD3d11Device, "Models/LegoBrick_Std2x3x3_NoStud.txt", NUMBER_OF_BRICKS_PER_TYPE  ) ) &&
		   SUCCEEDED( m_legoBrickMeshes[Std1x1x4].Initialise( m_pD3d11Device, "Models/LegoBrick_Std1x1x4_NoStud.txt", NUMBER_OF_BRICKS_PER_TYPE  ) ) &&
		   SUCCEEDED( m_legoBrickMeshes[Std1x3x4].Initialise( m_pD3d11Device, "Models/LegoBrick_Std1x3x4_NoStud.txt", NUMBER_OF_BRICKS_PER_TYPE  ) ) &&
		   SUCCEEDED( m_legoBrickMeshes[Std2x1x4].Initialise( m_pD3d11Device, "Models/LegoBrick_Std2x1x4_NoStud.txt", NUMBER_OF_BRICKS_PER_TYPE  ) ) &&
		   SUCCEEDED( m_legoBrickMeshes[Std2x3x4].Initialise( m_pD3d11Device, "Models/LegoBrick_Std2x3x4_NoStud.txt", NUMBER_OF_BRICKS_PER_TYPE  ) ) &&
		   SUCCEEDED( m_legoBrickMeshes[Std4x1x4].Initialise( m_pD3d11Device, "Models/LegoBrick_Std4x1x4_NoStud.txt", NUMBER_OF_BRICKS_PER_TYPE  ) ) &&
		   SUCCEEDED( m_legoBrickMeshes[Std1x1x6].Initialise( m_pD3d11Device, "Models/LegoBrick_Std1x1x6_NoStud.txt", NUMBER_OF_BRICKS_PER_TYPE  ) ) &&
		   SUCCEEDED( m_legoBrickMeshes[Std1x3x6].Initialise( m_pD3d11Device, "Models/LegoBrick_Std1x3x6_NoStud.txt", NUMBER_OF_BRICKS_PER_TYPE  ) ) &&
		   SUCCEEDED( m_legoBrickMeshes[Std2x1x6].Initialise( m_pD3d11Device, "Models/LegoBrick_Std2x1x6_NoStud.txt", NUMBER_OF_BRICKS_PER_TYPE  ) ) &&
		   SUCCEEDED( m_legoBrickMeshes[Std2x3x6].Initialise( m_pD3d11Device, "Models/LegoBrick_Std2x3x6_NoStud.txt", NUMBER_OF_BRICKS_PER_TYPE  ) ) &&
		   SUCCEEDED( m_legoBrickMeshes[Std4x1x6].Initialise( m_pD3d11Device, "Models/LegoBrick_Std4x1x6_NoStud.txt", NUMBER_OF_BRICKS_PER_TYPE  ) ) &&
		   SUCCEEDED( m_legoBrickMeshes[Std1x1x8].Initialise( m_pD3d11Device, "Models/LegoBrick_Std1x1x8_NoStud.txt", NUMBER_OF_BRICKS_PER_TYPE  ) ) &&
		   SUCCEEDED( m_legoBrickMeshes[Std1x3x8].Initialise( m_pD3d11Device, "Models/LegoBrick_Std1x3x8_NoStud.txt", NUMBER_OF_BRICKS_PER_TYPE  ) ) &&
		   SUCCEEDED( m_legoBrickMeshes[Std2x1x8].Initialise( m_pD3d11Device, "Models/LegoBrick_Std2x1x8_NoStud.txt", NUMBER_OF_BRICKS_PER_TYPE  ) ) &&
		   SUCCEEDED( m_legoBrickMeshes[Std2x3x8].Initialise( m_pD3d11Device, "Models/LegoBrick_Std2x3x8_NoStud.txt", NUMBER_OF_BRICKS_PER_TYPE  ) ) &&
		   SUCCEEDED( m_legoBrickMeshes[Std1x1x10].Initialise( m_pD3d11Device, "Models/LegoBrick_Std1x1x10_NoStud.txt", NUMBER_OF_BRICKS_PER_TYPE  ) ) &&
		   SUCCEEDED( m_legoBrickMeshes[Std1x3x10].Initialise( m_pD3d11Device, "Models/LegoBrick_Std1x3x10_NoStud.txt", NUMBER_OF_BRICKS_PER_TYPE  ) ) &&
		   SUCCEEDED( m_legoBrickMeshes[Std2x1x10].Initialise( m_pD3d11Device, "Models/LegoBrick_Std2x1x10_NoStud.txt", NUMBER_OF_BRICKS_PER_TYPE  ) ) &&
		   SUCCEEDED( m_legoBrickMeshes[Std2x3x10].Initialise( m_pD3d11Device, "Models/LegoBrick_Std2x3x10_NoStud.txt", NUMBER_OF_BRICKS_PER_TYPE  ) ) &&
		   SUCCEEDED( m_legoBrickMeshes[Std4x1x10].Initialise( m_pD3d11Device, "Models/LegoBrick_Std4x1x10_NoStud.txt", NUMBER_OF_BRICKS_PER_TYPE  ) ) &&
		   SUCCEEDED( m_legoStudMesh.Initialise( m_pD3d11Device, "Models/LegoBrick_Stud.txt", NUMBER_OF_STUDS ) );
		

		// Brick models with the studs already attached, shouldn't be used anymore
		/*SUCCEEDED( m_legoBrickMeshes[Std1x1x1].Initialise( m_pD3d11Device, "Models/LegoBrick_Std1x1x1.txt" ) ) &&
		   SUCCEEDED( m_legoBrickMeshes[Std1x3x1].Initialise( m_pD3d11Device, "Models/LegoBrick_Std1x3x1.txt" ) ) &&
		   SUCCEEDED( m_legoBrickMeshes[Std1x1x2].Initialise( m_pD3d11Device, "Models/LegoBrick_Std1x1x2.txt" ) ) &&
		   SUCCEEDED( m_legoBrickMeshes[Std1x3x2].Initialise( m_pD3d11Device, "Models/LegoBrick_Std1x3x2.txt" ) ) &&
		   SUCCEEDED( m_legoBrickMeshes[Std2x1x2].Initialise( m_pD3d11Device, "Models/LegoBrick_Std2x1x2.txt" ) ) &&
		   SUCCEEDED( m_legoBrickMeshes[Std2x3x2].Initialise( m_pD3d11Device, "Models/LegoBrick_Std2x3x2.txt" ) ) &&
		   SUCCEEDED( m_legoBrickMeshes[Std1x1x3].Initialise( m_pD3d11Device, "Models/LegoBrick_Std1x1x3.txt" ) ) &&
		   SUCCEEDED( m_legoBrickMeshes[Std1x3x3].Initialise( m_pD3d11Device, "Models/LegoBrick_Std1x3x3.txt" ) ) &&
		   SUCCEEDED( m_legoBrickMeshes[Std2x1x3].Initialise( m_pD3d11Device, "Models/LegoBrick_Std2x1x3.txt" ) ) &&
		   SUCCEEDED( m_legoBrickMeshes[Std2x3x3].Initialise( m_pD3d11Device, "Models/LegoBrick_Std2x3x3.txt" ) ) &&
		   SUCCEEDED( m_legoBrickMeshes[Std1x1x4].Initialise( m_pD3d11Device, "Models/LegoBrick_Std1x1x4.txt" ) ) &&
		   SUCCEEDED( m_legoBrickMeshes[Std1x3x4].Initialise( m_pD3d11Device, "Models/LegoBrick_Std1x3x4.txt" ) ) &&
		   SUCCEEDED( m_legoBrickMeshes[Std2x1x4].Initialise( m_pD3d11Device, "Models/LegoBrick_Std2x1x4.txt" ) ) &&
		   SUCCEEDED( m_legoBrickMeshes[Std2x3x4].Initialise( m_pD3d11Device, "Models/LegoBrick_Std2x3x4.txt" ) ) &&
		   SUCCEEDED( m_legoBrickMeshes[Std4x1x4].Initialise( m_pD3d11Device, "Models/LegoBrick_Std4x1x4.txt" ) ) &&
		   SUCCEEDED( m_legoBrickMeshes[Std1x1x6].Initialise( m_pD3d11Device, "Models/LegoBrick_Std1x1x6.txt" ) ) &&
		   SUCCEEDED( m_legoBrickMeshes[Std1x3x6].Initialise( m_pD3d11Device, "Models/LegoBrick_Std1x3x6.txt" ) ) &&
		   SUCCEEDED( m_legoBrickMeshes[Std2x1x6].Initialise( m_pD3d11Device, "Models/LegoBrick_Std2x1x6.txt" ) ) &&
		   SUCCEEDED( m_legoBrickMeshes[Std2x3x6].Initialise( m_pD3d11Device, "Models/LegoBrick_Std2x3x6.txt" ) ) &&
		   SUCCEEDED( m_legoBrickMeshes[Std4x1x6].Initialise( m_pD3d11Device, "Models/LegoBrick_Std4x1x6.txt" ) ) &&
		   SUCCEEDED( m_legoBrickMeshes[Std1x1x8].Initialise( m_pD3d11Device, "Models/LegoBrick_Std1x1x8.txt" ) ) &&
		   SUCCEEDED( m_legoBrickMeshes[Std1x3x8].Initialise( m_pD3d11Device, "Models/LegoBrick_Std1x3x8.txt" ) ) &&
		   SUCCEEDED( m_legoBrickMeshes[Std2x1x8].Initialise( m_pD3d11Device, "Models/LegoBrick_Std2x1x8.txt" ) ) &&
		   SUCCEEDED( m_legoBrickMeshes[Std2x3x8].Initialise( m_pD3d11Device, "Models/LegoBrick_Std2x3x8.txt" ) ) &&
		   SUCCEEDED( m_legoBrickMeshes[Std1x1x10].Initialise( m_pD3d11Device, "Models/LegoBrick_Std1x1x10.txt" ) ) &&
		   SUCCEEDED( m_legoBrickMeshes[Std1x3x10].Initialise( m_pD3d11Device, "Models/LegoBrick_Std1x3x10.txt" ) ) &&
		   SUCCEEDED( m_legoBrickMeshes[Std2x1x10].Initialise( m_pD3d11Device, "Models/LegoBrick_Std2x1x10.txt" ) ) &&
		   SUCCEEDED( m_legoBrickMeshes[Std2x3x10].Initialise( m_pD3d11Device, "Models/LegoBrick_Std2x3x10.txt" ) ) &&
		   SUCCEEDED( m_legoBrickMeshes[Std4x1x10].Initialise( m_pD3d11Device, "Models/LegoBrick_Std4x1x10.txt" ) );*/
}

//--------------------------------------------------------------------------------------
// Cleanup renderer, free ressources
//--------------------------------------------------------------------------------------
HRESULT RendererImplementation::Cleanup( void )
{
	m_renderContext.Cleanup();
	m_textRenderer.Cleanup();
	m_grid.Cleanup();
	m_cursor.Cleanup();
	
	for( int i = 0; i < NumberOfBrickTypes; ++i )
	{
		m_legoBrickMeshes[i].Cleanup();
	}

	m_legoStudMesh.Cleanup();

	for(int i = 0; i < NumberOfShaders; ++i)
	{
		if( m_shaders[i] )
		{ 
			m_shaders[i] -> Cleanup();
			delete m_shaders[i];
			m_shaders[i] = nullptr; 
		}
	}

	for(int i = 0; i < NumberOfShaderGroups; ++i)
	{
		m_shaderGroups[i].Cleanup();
	}

	if( m_pSwapChain )					{ m_pSwapChain			-> Release(); m_pSwapChain			= nullptr; }
	if( m_pD3d11Device )				{ m_pD3d11Device		-> Release(); m_pD3d11Device		= nullptr; }
	if( m_pD3d11DeviceContext )			{ m_pD3d11DeviceContext	-> Release(); m_pD3d11DeviceContext	= nullptr; }
	if( m_pRenderTargetView )			{ m_pRenderTargetView	-> Release(); m_pRenderTargetView	= nullptr; }
	if( m_pDepthStencilView )			{ m_pDepthStencilView	-> Release(); m_pDepthStencilView	= nullptr; }
	if( m_pDepthStencilBuffer )			{ m_pDepthStencilBuffer -> Release(); m_pDepthStencilBuffer	= nullptr; }

	if( m_depthStencilState )			{ m_depthStencilState		    -> Release(); m_depthStencilState	       = nullptr; }
	if( m_depthDisabledStencilState )	{ m_depthDisabledStencilState	-> Release(); m_depthDisabledStencilState  = nullptr; }
	if( m_pRasterStateCullBackfaces )	{ m_pRasterStateCullBackfaces	-> Release(); m_pRasterStateCullBackfaces  = nullptr; }
	if( m_pRasterStateCullFrontfaces )	{ m_pRasterStateCullFrontfaces	-> Release(); m_pRasterStateCullFrontfaces = nullptr; }
	if( m_alphaEnableBlendingState )	{ m_alphaEnableBlendingState	-> Release(); m_alphaEnableBlendingState   = nullptr; }
	if( m_alphaDisableBlendingState )	{ m_alphaDisableBlendingState	-> Release(); m_alphaDisableBlendingState  = nullptr; }

	if( m_pToneTexture )				{ m_pToneTexture          -> Release(); m_pToneTexture	       = nullptr; }
	if( m_pToneTexSamplerState )		{ m_pToneTexSamplerState  -> Release(); m_pToneTexSamplerState = nullptr; }

	if( m_pColourMapRenderTargetTexture )	{ m_pColourMapRenderTargetTexture  -> Release(); m_pColourMapRenderTargetTexture = nullptr; }
	if( m_pColourMapRenderTargetView )		{ m_pColourMapRenderTargetView     -> Release(); m_pColourMapRenderTargetView	 = nullptr; }
	if( m_pColourMapShaderResourceView )	{ m_pColourMapShaderResourceView   -> Release(); m_pColourMapShaderResourceView	 = nullptr; }
	if( m_pColourMapSamplerState )			{ m_pColourMapSamplerState         -> Release(); m_pColourMapSamplerState	     = nullptr; }

	if( m_pNormalDepthMapRenderTargetTexture )	{ m_pNormalDepthMapRenderTargetTexture -> Release(); m_pNormalDepthMapRenderTargetTexture  = nullptr; }
	if( m_pNormalDepthMapRenderTargetView )		{ m_pNormalDepthMapRenderTargetView    -> Release(); m_pNormalDepthMapRenderTargetView	   = nullptr; }
	if( m_pNormalDepthMapShaderResourceView )	{ m_pNormalDepthMapShaderResourceView  -> Release(); m_pNormalDepthMapShaderResourceView   = nullptr; }
	if( m_pNormalDepthMapSamplerState )			{ m_pNormalDepthMapSamplerState        -> Release(); m_pNormalDepthMapSamplerState	       = nullptr; }


	return S_OK;
}

//--------------------------------------------------------------------------------------
// Update the renderer, draw the scene
//--------------------------------------------------------------------------------------
void RendererImplementation::Update( const XMFLOAT4X4& viewMatrix, const XMFLOAT4X4& projMatrix, const XMFLOAT4X4& orthoProjMatrix, const EditorData& editorData, int numberOfBricks, int numberOfStuds, const PerformanceData& performanceData, const XMFLOAT3& cameraPosition )
{
	UpdateScene();
	DrawScene( viewMatrix, projMatrix, orthoProjMatrix, editorData, numberOfBricks, numberOfStuds, performanceData, cameraPosition );
}

//--------------------------------------------------------------------------------------
// Update the scene, camera and objects
//--------------------------------------------------------------------------------------
void RendererImplementation::UpdateScene( void )
{
	// More of a placeholder at the moment
}

//--------------------------------------------------------------------------------------
// Draw the scene
//--------------------------------------------------------------------------------------
void RendererImplementation::DrawScene( const XMFLOAT4X4& viewMatrix, const XMFLOAT4X4& projMatrix, const XMFLOAT4X4& orthoProjMatrix, const EditorData& editorData, int numberOfBricks, int numberOfStuds, const PerformanceData& performanceData, const XMFLOAT3& cameraPosition )
{
	// Clear the backbuffer
	m_pD3d11DeviceContext -> ClearRenderTargetView( m_pRenderTargetView, BACKGROUND_COLOUR );
	// Clear the other render targets
	m_pD3d11DeviceContext -> ClearRenderTargetView( m_pCelShadingRenderTargets[0], BACKGROUND_COLOUR );
	m_pD3d11DeviceContext -> ClearRenderTargetView( m_pCelShadingRenderTargets[1], BACKGROUND_COLOUR );

	// Refresh the Depth/Stencil view
	m_pD3d11DeviceContext -> ClearDepthStencilView( m_pDepthStencilView, D3D11_CLEAR_DEPTH|D3D11_CLEAR_STENCIL, 1.0f, 0 );

	// Update the per frame data
	XMStoreFloat4x4( &m_perFrameData.m_viewProjection, XMLoadFloat4x4(&viewMatrix) * XMLoadFloat4x4(&projMatrix) );
	XMStoreFloat4x4( &m_perFrameData.m_viewMatrix, XMLoadFloat4x4(&viewMatrix) );
	XMStoreFloat4x4( &m_perFrameData.m_projectionMatrix, XMLoadFloat4x4(&projMatrix) );
	m_perFrameData.m_viewPosition = cameraPosition;

	Prepare3DRendering();

	RenderBricks( &viewMatrix, &projMatrix, cameraPosition, editorData);

	// Only render grid/cursor and text if presentation mode is not active
	if( !editorData.m_isPresentationModeActive)
	{
		RenderEditorGeometry( &viewMatrix, &projMatrix, editorData );
	
		Prepare2DRendering();
	
		XMFLOAT4X4 worldMat;
		XMStoreFloat4x4( &worldMat, XMMatrixIdentity());

		m_textRenderer.Render( m_pD3d11DeviceContext, worldMat, orthoProjMatrix, editorData, numberOfBricks, m_brickRenderCount, numberOfStuds, m_studRenderCount, performanceData );
	}

	// Present the backbuffer to the screen
	m_pSwapChain -> Present(0, 0);
}

//--------------------------------------------------------------------------------------
// Render the lego bricks
//--------------------------------------------------------------------------------------
void RendererImplementation::RenderBricks( XMFLOAT4X4 const * viewMatrix, XMFLOAT4X4 const * projMatrix, const XMFLOAT3& cameraPosition, const EditorData& editorData )
{

	SwitchShader( editorData.m_selectedShaderGroup );

	if( m_shaderGroups[m_selectedBrickShaderGroup].m_numberOfPasses > 1 && m_activeShaderGroup != GroupToon04 && m_activeShaderGroup != GroupToon05 && m_activeShaderGroup != GroupToon06 )
	{
		// Change the cull mode, only draw backfaces
		m_pD3d11DeviceContext -> RSSetState( m_pRasterStateCullFrontfaces );
	}else
	{
		m_pD3d11DeviceContext -> RSSetState( m_pRasterStateCullBackfaces );
	}

	// set the proper render target
	if( m_activeShaderGroup == GroupToon06 )
	{
		m_pD3d11DeviceContext -> OMSetRenderTargets(2, m_pCelShadingRenderTargets, m_pDepthStencilView);
	}else
	{
		m_pD3d11DeviceContext -> OMSetRenderTargets(1, &m_pRenderTargetView, m_pDepthStencilView);
	}

	m_brickRenderCount = 0;
	m_studRenderCount = 0;

	for( int i = 0; i < NumberOfBrickTypes; ++i )
	{
		if( m_renderContext.GetInstanceCount( LegoBrickType(i) ) != 0 )
		{
			m_legoBrickMeshes[i].Render( m_pD3d11DeviceContext, m_renderContext.GetInstances( LegoBrickType(i) ), m_renderContext.GetInstanceCount( LegoBrickType(i) ) );
			m_brickRenderCount += m_renderContext.GetInstanceCount( LegoBrickType(i) );
		}
	}

	// Draw the studs
	m_studRenderCount = m_renderContext.GetStudInstanceCount();
	m_legoStudMesh.Render( m_pD3d11DeviceContext, m_renderContext.GetStudInstances(), m_studRenderCount );

	if( m_shaderGroups[m_selectedBrickShaderGroup].m_numberOfPasses > 1 )
	{
		if( m_activeShaderGroup == GroupToon06)
		{
			m_pD3d11DeviceContext -> OMSetRenderTargets(1, &m_pRenderTargetView, m_pDepthStencilView);
		}

		SwitchPass( 1 );

		if( m_activeShaderGroup != GroupToon04 && m_activeShaderGroup != GroupToon05 )
		{
			m_pD3d11DeviceContext -> RSSetState( m_pRasterStateCullBackfaces );
		}else
		{
			m_pD3d11DeviceContext -> RSSetState( m_pRasterStateCullFrontfaces );
		}

		if( m_activeShaderGroup != GroupToon06 )
		{
			for( int i = 0; i < NumberOfBrickTypes; ++i )
			{
				if( m_renderContext.GetInstanceCount( LegoBrickType(i) ) != 0 )
					m_legoBrickMeshes[i].Render( m_pD3d11DeviceContext, m_renderContext.GetInstances( LegoBrickType(i) ), m_renderContext.GetInstanceCount( LegoBrickType(i) ) );
			}
			m_legoStudMesh.Render( m_pD3d11DeviceContext, m_renderContext.GetStudInstances(), m_studRenderCount );
		}else
		{
			// disable depth buffering
			m_pD3d11DeviceContext -> OMSetDepthStencilState( m_depthDisabledStencilState, 1 );
			
			m_screenQuad.Render( m_pD3d11DeviceContext );
			
			// enable depth buffering
			m_pD3d11DeviceContext -> OMSetDepthStencilState( m_depthStencilState, 1 );

			// Reset the resources
			ID3D11ShaderResourceView* nullS(nullptr);
			m_shaderGroups[m_activeShaderGroup].m_pShaderPasses[1].m_pixelShader -> UpdateTextureResource( 0, m_pD3d11DeviceContext, nullS, m_pColourMapSamplerState );
			m_shaderGroups[m_activeShaderGroup].m_pShaderPasses[1].m_pixelShader -> UpdateTextureResource( 1, m_pD3d11DeviceContext, nullS, m_pNormalDepthMapSamplerState );
		}
	}
	
	// Change the cull mode, only draw frontfaces (back to normal)
	m_pD3d11DeviceContext -> RSSetState( m_pRasterStateCullBackfaces );

	// todo: only do this if somethin changes
	// reset instance counts
	m_renderContext.Reset();
}


//--------------------------------------------------------------------------------------
// Render the editor grid and cursor
//--------------------------------------------------------------------------------------
void RendererImplementation::RenderEditorGeometry( XMFLOAT4X4 const * viewMatrix, XMFLOAT4X4 const * projMatrix, const EditorData& editorData )
{

	SwitchShader( GroupEditorGeometry );
	
	// Draw the grid

	// Update per object data
	XMMATRIX matWorldGrid = XMMatrixTranslation( 0.0f, editorData.m_cursorPosition.m_y * GRID_UNIT.y, 0.0f );
	XMStoreFloat4x4( &m_perObjectData.m_WVP, matWorldGrid * XMLoadFloat4x4(viewMatrix) * XMLoadFloat4x4(projMatrix) );
	m_perObjectData.m_colour = GRID_COLOUR;

	m_shaderGroups[m_activeShaderGroup].m_pShaderPasses[0].m_vertexShader -> UpdatePerObjectData( m_pD3d11DeviceContext, m_perObjectData );

	m_grid.Render( m_pD3d11DeviceContext );

	// Draw the cursor
	
	XMMATRIX matWorldCursor = XMMatrixIdentity();
	
	// Calculate the translation matrix of the vector.

	// Get the cursor position in world space, not in grid coordinates
	XMFLOAT3 actualCursorPosition( editorData.m_cursorPosition.m_x * GRID_UNIT.x, editorData.m_cursorPosition.m_y * GRID_UNIT.y, editorData.m_cursorPosition.m_z * GRID_UNIT.z );
	XMMATRIX matTranslationCursor = XMMatrixTranslationFromVector( XMLoadFloat3( &actualCursorPosition ) );

	// Calculate the rotation matrix of the vector
	
	// the actual rotation of the cursor
	XMMATRIX matPureRotationCursor = XMMatrixRotationAxis( XMVectorSet( 0.0f, 1.0f, 0.0f, 0.0f ), XMConvertToRadians( static_cast<float>( editorData.m_brickRotation ) ) );
	// calculate translation matrices to and from the rotation centre
	XMMATRIX matTranslationRotCentre = XMMatrixTranslation(GRID_UNIT.x/2.0f, 0.0f, GRID_UNIT.z/2.0f);	// Rotation centre (from origin).
	XMVECTOR determinant;
	XMMATRIX matTranslationRotCentreInv = XMMatrixInverse(&determinant, matTranslationRotCentre);
	// Combine the matrices to the completed rotation matrix
	XMMATRIX matRotationCursor = matTranslationRotCentreInv * matPureRotationCursor * matTranslationRotCentre;

	// calculate the scaling matrix for the cursor
	XMMATRIX matScalingCursor = XMMatrixIdentity();
	
	// if deletion mode is active, cursor will be of the size of a single grid unit
	if(!editorData.m_isDeletionModeActive)
		matScalingCursor = XMMatrixScaling( static_cast<float>( editorData.m_cursorExtents.m_x ), static_cast<float>( editorData.m_cursorExtents.m_y ), static_cast<float>( editorData.m_cursorExtents.m_z ) );

	matWorldCursor = matScalingCursor * matRotationCursor * matTranslationCursor;
	
	// Update per object data
	XMStoreFloat4x4( &m_perObjectData.m_WVP, matWorldCursor * XMLoadFloat4x4(viewMatrix) * XMLoadFloat4x4(projMatrix) );

	if(editorData.m_isDeletionModeActive)
	{
		m_perObjectData.m_colour = CURSOR_DELETE_COLOUR;
	}else
	{
		m_perObjectData.m_colour = CURSOR_COLOUR;
	}

	m_shaderGroups[m_activeShaderGroup].m_pShaderPasses[0].m_vertexShader -> UpdatePerObjectData( m_pD3d11DeviceContext, m_perObjectData );

	m_cursor.Render( m_pD3d11DeviceContext );
}


//--------------------------------------------------------------------------------------
// Prepare device for rendering of 3d objects
//--------------------------------------------------------------------------------------
void RendererImplementation::Prepare3DRendering()
{
	// enable depth buffering
	m_pD3d11DeviceContext -> OMSetDepthStencilState( m_depthStencilState, 1 );
	
	// Turn off alpha blending
	float blendFactor[4] = {0.0f, 0.0f, 0.0f, 0.0f};
	m_pD3d11DeviceContext -> OMSetBlendState(m_alphaDisableBlendingState, blendFactor, 0xffffffff);
}

//--------------------------------------------------------------------------------------
// Prepare device for rendering of 2d objects (text, textures)
//--------------------------------------------------------------------------------------
void RendererImplementation::Prepare2DRendering()
{
	// disable depth buffering
	m_pD3d11DeviceContext -> OMSetDepthStencilState( m_depthDisabledStencilState, 1 );

	// Turn on alpha blending
	float blendFactor[4] = {0.0f, 0.0f, 0.0f, 0.0f};
	m_pD3d11DeviceContext -> OMSetBlendState(m_alphaEnableBlendingState, blendFactor, 0xffffffff);
}

RenderContext* RendererImplementation::GetRenderContext( void )
{
	return &m_renderContext;
}

//--------------------------------------------------------------------------------------
// Factory function for creating shaders. Returns a pointer to the dynamically
// allocated shader object.
//--------------------------------------------------------------------------------------
Shader* RendererImplementation::CreateShader( Shaders shaderIdentifier )
{
	Shader* pShader = nullptr;

	switch( shaderIdentifier )
	{
	case EditorGeometryVS:
		pShader = new EditorGeometryVertexShader();
		break;
	case EditorGeometryPS:
		pShader = new EditorGeometryPixelShader();
		break;
	case SimpleVS:
		pShader = new SimpleVertexShader();
		break;
	case SimplePS:
		pShader = new SimplePixelShader();
		break;
	case LightedVS:
		pShader = new LightedVertexShader();
		break;		
	case LightedPS:
		pShader = new LightedPixelShader();
		break;	
	case ToonColour01VS:
		pShader = new ToonColourVertexShader01();
		break;		
	case ToonColour01PS:
		pShader = new ToonColourPixelShader01();
		break;		
	case ToonEdges01VS:
		pShader = new ToonEdgesVertexShader01();
		break;		
	case ToonEdges01PS:
		pShader = new ToonEdgesPixelShader01();
		break;	
	case ToonColour02VS:
		pShader = new ToonColourVertexShader02();
		break;	
	case ToonColour02PS:
		pShader = new ToonColourPixelShader02();
		break;	
	case ToonEdges02VS:
		pShader = new ToonEdgesVertexShader02();
		break;		
	case ToonEdges02PS:
		pShader = new ToonEdgesPixelShader02();
		break;	
	case Toon03VS:
		pShader = new ToonVertexShader03();
		break;		
	case Toon03PS:
		pShader = new ToonPixelShader03();
		break;	
	case ToonColour04VS:
		pShader = new ToonColourVertexShader04();
		break;	
	case ToonColour04PS:
		pShader = new ToonColourPixelShader04();
		break;	
	case ToonEdges04VS:
		pShader = new ToonEdgesVertexShader04();
		break;		
	case ToonEdges04PS:
		pShader = new ToonEdgesPixelShader04();
		break;	
	case ToonColour05VS:
		pShader = new ToonColourVertexShader05();
		break;	
	case ToonColour05PS:
		pShader = new ToonColourPixelShader05();
		break;	
	case ToonEdges05VS:
		pShader = new ToonEdgesVertexShader05();
		break;		
	case ToonEdges05PS:
		pShader = new ToonEdgesPixelShader05();
		break;	
	case CreateMapsToon06VS:
		pShader = new CreateMapsVertexShaderToon06();
		break;	
	case CreateMapsToon06PS:
		pShader = new CreateMapsPixelShaderToon06();
		break;	
	case ComposeImageToon06VS:
		pShader = new ComposeImageVertexShaderToon06();
		break;		
	case ComposeImageToon06PS:
		pShader = new ComposeImagePixelShaderToon06();
		break;	
	}

	return pShader;
}

//--------------------------------------------------------------------------------------
// Set new shaders and update their constant buffers with the current scene and frame data.
//--------------------------------------------------------------------------------------
void RendererImplementation::SwitchShader( ShaderGroups newShaderGroup )
{
	m_shaderGroups[newShaderGroup].m_pShaderPasses[0].m_vertexShader -> Activate( m_pD3d11DeviceContext );
	m_shaderGroups[newShaderGroup].m_pShaderPasses[0].m_pixelShader -> Activate( m_pD3d11DeviceContext );

	m_shaderGroups[newShaderGroup].m_pShaderPasses[0].m_vertexShader -> UpdatePerSceneData( m_pD3d11DeviceContext, m_perSceneData );
	m_shaderGroups[newShaderGroup].m_pShaderPasses[0].m_pixelShader -> UpdatePerSceneData( m_pD3d11DeviceContext, m_perSceneData );

	m_shaderGroups[newShaderGroup].m_pShaderPasses[0].m_vertexShader -> UpdatePerFrameData( m_pD3d11DeviceContext, m_perFrameData );
	m_shaderGroups[newShaderGroup].m_pShaderPasses[0].m_pixelShader -> UpdatePerFrameData( m_pD3d11DeviceContext, m_perFrameData );

	m_activeShaderGroup = newShaderGroup;
	if( newShaderGroup != GroupEditorGeometry )
	{
		m_selectedBrickShaderGroup = newShaderGroup;
	}

	// special case
	if( m_activeShaderGroup == GroupToon03 || m_activeShaderGroup == GroupToon04 || m_activeShaderGroup == GroupToon05 || m_activeShaderGroup == GroupToon06 )
	{
		m_shaderGroups[m_activeShaderGroup].m_pShaderPasses[0].m_pixelShader -> UpdateTextureResource( 0, m_pD3d11DeviceContext, m_pToneTexture, m_pToneTexSamplerState );
	}
}

//--------------------------------------------------------------------------------------
// Switch to a new pass for the current shader group.
//--------------------------------------------------------------------------------------
void RendererImplementation::SwitchPass( int newPass )
{
	if( newPass < m_shaderGroups[m_activeShaderGroup].m_numberOfPasses )
	{
		m_shaderGroups[m_activeShaderGroup].m_pShaderPasses[1].m_vertexShader -> Activate( m_pD3d11DeviceContext );
		m_shaderGroups[m_activeShaderGroup].m_pShaderPasses[1].m_pixelShader -> Activate( m_pD3d11DeviceContext );

		m_shaderGroups[m_activeShaderGroup].m_pShaderPasses[1].m_vertexShader -> UpdatePerSceneData( m_pD3d11DeviceContext, m_perSceneData );
		m_shaderGroups[m_activeShaderGroup].m_pShaderPasses[1].m_pixelShader -> UpdatePerSceneData( m_pD3d11DeviceContext, m_perSceneData );

		m_shaderGroups[m_activeShaderGroup].m_pShaderPasses[1].m_vertexShader -> UpdatePerFrameData( m_pD3d11DeviceContext, m_perFrameData );
		m_shaderGroups[m_activeShaderGroup].m_pShaderPasses[1].m_pixelShader -> UpdatePerFrameData( m_pD3d11DeviceContext, m_perFrameData );
	
		// special case
		if( m_activeShaderGroup == GroupToon02 )
		{
			m_shaderGroups[m_activeShaderGroup].m_pShaderPasses[1].m_pixelShader -> UpdateTextureResource( 0, m_pD3d11DeviceContext, m_pToneTexture, m_pToneTexSamplerState );
		}

		// special case
		if( m_activeShaderGroup == GroupToon06 )
		{

			m_shaderGroups[m_activeShaderGroup].m_pShaderPasses[1].m_pixelShader -> UpdateTextureResource( 0, m_pD3d11DeviceContext, m_pColourMapShaderResourceView, m_pColourMapSamplerState );
			m_shaderGroups[m_activeShaderGroup].m_pShaderPasses[1].m_pixelShader -> UpdateTextureResource( 1, m_pD3d11DeviceContext, m_pNormalDepthMapShaderResourceView, m_pNormalDepthMapSamplerState );
		}
	}
}
