/* 
*  Kevin Meergans, SquadAI, 2014
*  Renderer.cpp
*  Sets up and manages DirectX 11 for use by the application and is 
*  responsible for drawing the scene.
*/

//#define DEBUG

// Includes
#include "Renderer.h"

Renderer::Renderer() : m_pSwapChain(nullptr),
					   m_pD3d11Device(nullptr),
					   m_pD3d11DeviceContext(nullptr),
					   m_pRenderTargetView(nullptr),
					   m_pDepthStencilView(nullptr),
					   m_pDepthStencilBuffer(nullptr),
					   m_pDepthEnabledStencilState(nullptr),
					   m_pDepthDisabledStencilState(nullptr),
					   m_pRasterStateCullBackfaces(nullptr),
					   m_pBlendingEnabledBlendingState(nullptr),
					   m_pBlendingDisabledBlendingState(nullptr),
					   m_currentShaderGroup(ShaderType(0)),
					   m_pFontSamplerState(nullptr)

{
	XMStoreFloat4x4(&m_baseViewMatrix, XMMatrixIdentity());

	for(unsigned int i = 0; i < NumberOfSentences; ++i)
	{
		m_pPermanentSentences[i] = nullptr;
	}

	for(unsigned int i = 0; i < NumberOfDrawableTypes; ++i)
	{
		m_drawableObjects[i] = nullptr;
	}
}

Renderer::~Renderer()
{
	Cleanup();
}

//--------------------------------------------------------------------------------------
// Initialises the renderer.
// Param1: A handle to the window that the application will run in.
// Param2: The width of the window in pixels.
// Param3: The height of the window in pixels.
// Param4: The initial view matrix of the application camera. Needed for text rendering.
// Param5: The initial projection matrix of the application camera. Needed for text rendering.
// Returns true if the renderer was initialised successfully, false otherwise.
//--------------------------------------------------------------------------------------
bool Renderer::Initialise(HWND hWnd, UINT windowWidth, UINT windowHeight, const XMFLOAT4X4& viewMatrix, const XMFLOAT4X4& projectionMatrix)
{
	m_windowWidth  = windowWidth;
	m_windowHeight = windowHeight;

	if(InitialiseD3D(hWnd) && 
		InitialiseDrawables() && 
		InitialiseObjectRenderData() &&
		InitialiseShaders() && 
		InitialiseTextRendering(hWnd, viewMatrix, projectionMatrix)
	  )
	{
		// Set default render states and shaders
		PrepareDefaultGeometryRendering();
		return true;
	}

	return false;
}

//--------------------------------------------------------------------------------------
// Initialises DirectX 11 by creating a device and other required components while also initialising 
// all render states (such as blend and cull states) that will be used by the application.
// Param1: A handle to the window that the application will run in.
// Returns true if DirectX 11 was initialised successfully, false otherwise.
//--------------------------------------------------------------------------------------
bool Renderer::InitialiseD3D(HWND hWnd)
{
	// Description of the swap chain buffer
	DXGI_MODE_DESC swapChainBufferDesc;
	ZeroMemory(&swapChainBufferDesc, sizeof(DXGI_MODE_DESC));

	swapChainBufferDesc.Width					= m_windowWidth;
	swapChainBufferDesc.Height					= m_windowHeight;
	swapChainBufferDesc.RefreshRate.Numerator	= 60;
	swapChainBufferDesc.RefreshRate.Denominator	= 1;
	swapChainBufferDesc.Format					= DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainBufferDesc.ScanlineOrdering		= DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainBufferDesc.Scaling					= DXGI_MODE_SCALING_UNSPECIFIED;

	// Description of the swap chain
	DXGI_SWAP_CHAIN_DESC swapChainDesc; 
	ZeroMemory(&swapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));

	swapChainDesc.BufferDesc			= swapChainBufferDesc;
	swapChainDesc.SampleDesc.Count		= 1;
	swapChainDesc.SampleDesc.Quality	= 0;
	swapChainDesc.BufferUsage			= DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferCount			= 1;
	swapChainDesc.OutputWindow			= hWnd; 
	swapChainDesc.Windowed				= TRUE; // use full screen
	swapChainDesc.SwapEffect			= DXGI_SWAP_EFFECT_DISCARD;

	// Create the swap chain and the device
#ifdef DEBUG
	// Create the device in debug mode
	if(FAILED(D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, D3D11_CREATE_DEVICE_DEBUG, NULL, NULL, D3D11_SDK_VERSION, &swapChainDesc, &m_pSwapChain, &m_pD3d11Device, NULL, &m_pD3d11DeviceContext)))
	{
		return false;
	}
#else
	if(FAILED(D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, NULL, NULL, NULL, D3D11_SDK_VERSION, &swapChainDesc, &m_pSwapChain, &m_pD3d11Device, NULL, &m_pD3d11DeviceContext)))
	{
		return false;
	}
#endif // DEBUG

	// Create the back buffer
	ID3D11Texture2D* pBackBuffer;
	if(FAILED(m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&pBackBuffer)))
	{
		return false;
	}

	// Create the render target
	if(FAILED(m_pD3d11Device -> CreateRenderTargetView(pBackBuffer, NULL, &m_pRenderTargetView)))
	{
		return false;
	}

	pBackBuffer->Release();

	//Description of the depth and stencil buffers
	D3D11_TEXTURE2D_DESC depthStencilBufferDesc;
	ZeroMemory(&depthStencilBufferDesc, sizeof(D3D11_TEXTURE2D_DESC));

	depthStencilBufferDesc.Width			  = m_windowWidth;
	depthStencilBufferDesc.Height			  = m_windowHeight;
	depthStencilBufferDesc.MipLevels		  = 1;
	depthStencilBufferDesc.ArraySize		  = 1;
	depthStencilBufferDesc.Format			  = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilBufferDesc.SampleDesc.Count   = 1;
	depthStencilBufferDesc.SampleDesc.Quality = 0;
	depthStencilBufferDesc.Usage			  = D3D11_USAGE_DEFAULT;
	depthStencilBufferDesc.BindFlags		  = D3D11_BIND_DEPTH_STENCIL;
	depthStencilBufferDesc.CPUAccessFlags	  = 0; 
	depthStencilBufferDesc.MiscFlags		  = 0;

	// Create the depth and stencil view
	if(FAILED(m_pD3d11Device->CreateTexture2D(&depthStencilBufferDesc, NULL, &m_pDepthStencilBuffer)))
	{
		return false;
	}
	if(FAILED(m_pD3d11Device->CreateDepthStencilView(m_pDepthStencilBuffer, NULL, &m_pDepthStencilView)))
	{
		return false;
	}

	// Set the render target
	m_pD3d11DeviceContext->OMSetRenderTargets(1, &m_pRenderTargetView, m_pDepthStencilView);

	// Initialise the required render states
	if(!InitialiseRenderStates())
	{
		return false;
	}

	// Create the Viewport
	D3D11_VIEWPORT viewport;
	ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

	viewport.TopLeftX	= 0;
	viewport.TopLeftY	= 0;
	viewport.Width		= static_cast<float>(m_windowWidth);
	viewport.Height		= static_cast<float>(m_windowHeight);
	viewport.MinDepth	= 0.0f;
	viewport.MaxDepth	= 1.0f;

	// Set the Viewport (Bind viewport to rasterizer stage)
	m_pD3d11DeviceContext->RSSetViewports(1, &viewport);

	return true;
}

//--------------------------------------------------------------------------------------
// Creates all render states used by the application and sets the default states on the device.
// Returns true if all states were created successfully, false otherwise.
//--------------------------------------------------------------------------------------
bool Renderer::InitialiseRenderStates()
{
	// Create depth/stencil states

	D3D11_DEPTH_STENCIL_DESC depthEnabledStencilDesc;
	ZeroMemory( &depthEnabledStencilDesc, sizeof( D3D11_DEPTH_STENCIL_DESC ) );

	// Set up the description of the stencil state.
	depthEnabledStencilDesc.DepthEnable    = true;
	depthEnabledStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthEnabledStencilDesc.DepthFunc      = D3D11_COMPARISON_LESS;

	depthEnabledStencilDesc.StencilEnable    = true;
	depthEnabledStencilDesc.StencilReadMask  = 0xFF;
	depthEnabledStencilDesc.StencilWriteMask = 0xFF;

	// Stencil operations if pixel is front-facing.
	depthEnabledStencilDesc.FrontFace.StencilFailOp      = D3D11_STENCIL_OP_KEEP;
	depthEnabledStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthEnabledStencilDesc.FrontFace.StencilPassOp      = D3D11_STENCIL_OP_KEEP;
	depthEnabledStencilDesc.FrontFace.StencilFunc        = D3D11_COMPARISON_ALWAYS;

	// Stencil operations if pixel is back-facing.
	depthEnabledStencilDesc.BackFace.StencilFailOp      = D3D11_STENCIL_OP_KEEP;
	depthEnabledStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthEnabledStencilDesc.BackFace.StencilPassOp      = D3D11_STENCIL_OP_KEEP;
	depthEnabledStencilDesc.BackFace.StencilFunc        = D3D11_COMPARISON_ALWAYS;

	// Create the depth stencil state with depth enabled
	m_pD3d11Device->CreateDepthStencilState(&depthEnabledStencilDesc, &m_pDepthEnabledStencilState);

	D3D11_DEPTH_STENCIL_DESC depthDisabledStencilDesc;
	ZeroMemory( &depthDisabledStencilDesc, sizeof( D3D11_DEPTH_STENCIL_DESC ) );

	// Set up the description of the stencil state.
	depthDisabledStencilDesc.DepthEnable    = false;
	depthDisabledStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthDisabledStencilDesc.DepthFunc      = D3D11_COMPARISON_LESS;

	depthDisabledStencilDesc.StencilEnable    = true;
	depthDisabledStencilDesc.StencilReadMask  = 0xFF;
	depthDisabledStencilDesc.StencilWriteMask = 0xFF;

	// Stencil operations if pixel is front-facing.
	depthDisabledStencilDesc.FrontFace.StencilFailOp      = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthDisabledStencilDesc.FrontFace.StencilPassOp      = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.FrontFace.StencilFunc        = D3D11_COMPARISON_ALWAYS;

	// Stencil operations if pixel is back-facing.
	depthDisabledStencilDesc.BackFace.StencilFailOp      = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthDisabledStencilDesc.BackFace.StencilPassOp      = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.BackFace.StencilFunc        = D3D11_COMPARISON_ALWAYS;

	// Create the depth stencil state with depth disabled
	m_pD3d11Device->CreateDepthStencilState(&depthDisabledStencilDesc, &m_pDepthDisabledStencilState);

	// Create rasterizer state

	// Create the description for the rasterizer
	D3D11_RASTERIZER_DESC rasterDescCullBack;
	ZeroMemory(&rasterDescCullBack, sizeof(D3D11_RASTERIZER_DESC));

	rasterDescCullBack.AntialiasedLineEnable = false;
	rasterDescCullBack.CullMode              = D3D11_CULL_BACK;
	rasterDescCullBack.DepthBias             = 0;
	rasterDescCullBack.DepthBiasClamp        = 0.0f;
	rasterDescCullBack.DepthClipEnable       = true;
	rasterDescCullBack.FillMode              = D3D11_FILL_SOLID;
	rasterDescCullBack.FrontCounterClockwise = false;
	rasterDescCullBack.MultisampleEnable     = false;
	rasterDescCullBack.ScissorEnable         = false;
	rasterDescCullBack.SlopeScaledDepthBias  = 0.0f;

	// Create the rasterizer state from the description
	if(FAILED(m_pD3d11Device->CreateRasterizerState(&rasterDescCullBack, &m_pRasterStateCullBackfaces)))
	{
		return false;
	}

	// Create blend states

	// Create blend state description
	D3D11_BLEND_DESC alphaEnableBlendDesc;
	ZeroMemory(&alphaEnableBlendDesc, sizeof(D3D11_BLEND_DESC));

	alphaEnableBlendDesc.RenderTarget[0].BlendEnable           = TRUE;
	alphaEnableBlendDesc.RenderTarget[0].SrcBlend              = D3D11_BLEND_ONE;
	alphaEnableBlendDesc.RenderTarget[0].DestBlend             = D3D11_BLEND_INV_SRC_ALPHA;
	alphaEnableBlendDesc.RenderTarget[0].BlendOp               = D3D11_BLEND_OP_ADD;
	alphaEnableBlendDesc.RenderTarget[0].SrcBlendAlpha         = D3D11_BLEND_ONE;
	alphaEnableBlendDesc.RenderTarget[0].DestBlendAlpha        = D3D11_BLEND_ZERO;
	alphaEnableBlendDesc.RenderTarget[0].BlendOpAlpha          = D3D11_BLEND_OP_ADD;
	alphaEnableBlendDesc.RenderTarget[0].RenderTargetWriteMask = 0x0f;

	// Create a blend state with blending enabled
	if(FAILED(m_pD3d11Device->CreateBlendState(&alphaEnableBlendDesc, &m_pBlendingEnabledBlendingState)))
	{
		return false;
	}

	D3D11_BLEND_DESC alphaDisableBlendDesc;
	ZeroMemory(&alphaDisableBlendDesc, sizeof(D3D11_BLEND_DESC));

	// Create an alpha enabled blend state description.
	alphaDisableBlendDesc.RenderTarget[0].BlendEnable           = FALSE;
	alphaDisableBlendDesc.RenderTarget[0].SrcBlend              = D3D11_BLEND_ONE;
	alphaDisableBlendDesc.RenderTarget[0].DestBlend             = D3D11_BLEND_INV_SRC_ALPHA;
	alphaDisableBlendDesc.RenderTarget[0].BlendOp               = D3D11_BLEND_OP_ADD;
	alphaDisableBlendDesc.RenderTarget[0].SrcBlendAlpha         = D3D11_BLEND_ONE;
	alphaDisableBlendDesc.RenderTarget[0].DestBlendAlpha        = D3D11_BLEND_ZERO;
	alphaDisableBlendDesc.RenderTarget[0].BlendOpAlpha          = D3D11_BLEND_OP_ADD;
	alphaDisableBlendDesc.RenderTarget[0].RenderTargetWriteMask = 0x0f;

	// Create a blend state with blending disabled
	if(FAILED(m_pD3d11Device -> CreateBlendState(&alphaDisableBlendDesc, &m_pBlendingDisabledBlendingState)))
	{
		return false;
	}

	return true;
}

//--------------------------------------------------------------------------------------
// Creates and initialise the Drawables available to the renderer.
// Returns true if the drawables were created and intialised successfully, false otherwise.
//--------------------------------------------------------------------------------------
bool Renderer::InitialiseDrawables()
{
	// Create and initialise the Drawables

	m_drawableObjects[TriangleType] = new TriangleDrawable(1.0f, 1.0f);
	if(!m_drawableObjects[TriangleType])
	{
		return false;
	}else if(!m_drawableObjects[TriangleType]->Initialise(m_pD3d11Device))
	{
		return false;
	}

	m_drawableObjects[SquareType] = new SquareDrawable(1.0f);
	if(!m_drawableObjects[SquareType])
	{
		return false;
	}else if(!m_drawableObjects[SquareType]->Initialise(m_pD3d11Device))
	{
		return false;
	}

	m_drawableObjects[CircleType] = new CircleDrawable(0.5f, 24);
	if(!m_drawableObjects[CircleType])
	{
		return false;
	}else if(!m_drawableObjects[CircleType]->Initialise(m_pD3d11Device))
	{
		return false;
	}

	// Initialise a default grid
	if(!SetupGrid(10.0f, 10))
	{
		return false;
	}

	return true;
}

//--------------------------------------------------------------------------------------
// Initialises the entity render data for each possible entity.
// Returns true if the entity render data was initialised successfully, false otherwise.
//--------------------------------------------------------------------------------------
bool Renderer::InitialiseObjectRenderData()
{

	m_objectRenderData[RedSoldierType].m_drawableType = TriangleType;
	m_objectRenderData[RedSoldierType].m_colour       = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
	m_objectRenderData[RedSoldierType].m_name         = "Red Soldier";
	m_objectRenderData[RedSoldierType].m_baseZ        = -1.0f;

	m_objectRenderData[BlueSoldierType].m_drawableType = TriangleType;
	m_objectRenderData[BlueSoldierType].m_colour       = XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);
	m_objectRenderData[BlueSoldierType].m_name         = "Blue Soldier";
	m_objectRenderData[BlueSoldierType].m_baseZ        = -1.0f;

	m_objectRenderData[ObstacleType].m_drawableType = SquareType;
	m_objectRenderData[ObstacleType].m_colour       = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	m_objectRenderData[ObstacleType].m_name         = "Obstacle";
	m_objectRenderData[ObstacleType].m_baseZ        = -2.0f;
			
	m_objectRenderData[RedFlagType].m_drawableType = CircleType;
	m_objectRenderData[RedFlagType].m_colour       = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
	m_objectRenderData[RedFlagType].m_name         = "Red Flag";
	m_objectRenderData[RedFlagType].m_baseZ        = -2.0f;

	m_objectRenderData[BlueFlagType].m_drawableType = CircleType;
	m_objectRenderData[BlueFlagType].m_colour       = XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);
	m_objectRenderData[BlueFlagType].m_name         = "Blue Flag";
	m_objectRenderData[BlueFlagType].m_baseZ        = -2.0f;

	m_objectRenderData[RedBaseAreaType].m_drawableType = SquareType;
	m_objectRenderData[RedBaseAreaType].m_colour       = XMFLOAT4(0.35f, 0.0f, 0.0f, 1.0f);
	m_objectRenderData[RedBaseAreaType].m_name         = "Red Base Area";
	m_objectRenderData[RedBaseAreaType].m_baseZ        = 3.0f;

	m_objectRenderData[BlueBaseAreaType].m_drawableType = SquareType;
	m_objectRenderData[BlueBaseAreaType].m_colour       = XMFLOAT4(0.0f, 0.0f, 0.35f, 1.0f);
	m_objectRenderData[BlueBaseAreaType].m_name         = "Blue Base Area";
	m_objectRenderData[BlueBaseAreaType].m_baseZ        = 3.0f;

	m_objectRenderData[RedSpawnPointType].m_drawableType = SquareType;
	m_objectRenderData[RedSpawnPointType].m_colour       = XMFLOAT4(0.6f, 0.0f, 0.0f, 1.0f);
	m_objectRenderData[RedSpawnPointType].m_name         = "Red Spawn Point";
	m_objectRenderData[RedSpawnPointType].m_baseZ        = 0.0f;

	m_objectRenderData[BlueSpawnPointType].m_drawableType = SquareType;
	m_objectRenderData[BlueSpawnPointType].m_colour       = XMFLOAT4(0.0f, 0.0f, 0.6f, 1.0f);
	m_objectRenderData[BlueSpawnPointType].m_name         = "Blue Spawn Point";
	m_objectRenderData[BlueSpawnPointType].m_baseZ        = 0.0f;
	
	m_objectRenderData[RedAttackPositionType].m_drawableType = TriangleType;
	m_objectRenderData[RedAttackPositionType].m_colour       = XMFLOAT4(1.0f, 0.5f, 0.0f, 1.0f);
	m_objectRenderData[RedAttackPositionType].m_name         = "Red Attack Point";
	m_objectRenderData[RedAttackPositionType].m_baseZ        = -2.0f;

	m_objectRenderData[BlueAttackPositionType].m_drawableType = TriangleType;
	m_objectRenderData[BlueAttackPositionType].m_colour       = XMFLOAT4(0.0f, 0.5f, 1.0f, 1.0f);
	m_objectRenderData[BlueAttackPositionType].m_name         = "Blue Attack Point";
	m_objectRenderData[BlueAttackPositionType].m_baseZ        = -2.0f;

	m_objectRenderData[DeadRedSoldierType].m_drawableType = TriangleType;
	m_objectRenderData[DeadRedSoldierType].m_colour       = XMFLOAT4(0.5f, 0.0f, 0.0f, 1.0f);
	m_objectRenderData[DeadRedSoldierType].m_name         = "Dead Red Soldier";
	m_objectRenderData[DeadRedSoldierType].m_baseZ        = 1.0f;
	
	m_objectRenderData[DeadBlueSoldierType].m_drawableType = TriangleType;
	m_objectRenderData[DeadBlueSoldierType].m_colour       = XMFLOAT4(0.0f, 0.0f, 0.5f, 1.0f);
	m_objectRenderData[DeadBlueSoldierType].m_name         = "Dead Blue Soldier";
	m_objectRenderData[DeadBlueSoldierType].m_baseZ        = 1.0f;

	m_objectRenderData[ProjectileType].m_drawableType = CircleType;
	m_objectRenderData[ProjectileType].m_colour       = XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f);
	m_objectRenderData[ProjectileType].m_name         = "Projectile";
	m_objectRenderData[ProjectileType].m_baseZ        = 0.0f;

	return true;
}

//--------------------------------------------------------------------------------------
// Initialise shaders that will be used by the application.
// Returns true if the shaders were initialised successfully, false otherwise.
//--------------------------------------------------------------------------------------
bool Renderer::InitialiseShaders()
{
	for(int i = 0; i < NumberOfShaderTypes; ++i)
	{
		if(!m_shaderGroups[i].Initialise(m_pD3d11Device, ShaderType(i)))
		{
			return false;
		}
	}

	return true;
}

//--------------------------------------------------------------------------------------
// Cleanup renderer, free ressources.
//--------------------------------------------------------------------------------------
bool Renderer::InitialiseTextRendering(HWND hwnd, const XMFLOAT4X4& baseViewMatrix, const XMFLOAT4X4& baseProjectionMatrix)
{
	m_baseViewMatrix       = baseViewMatrix;
	m_baseProjectionMatrix = baseProjectionMatrix;

	// Initialise font object

	if(!m_font.Initialise(m_pD3d11Device, "Font/FontData.csv", L"Font/ExportedFont.bmp"))
	{
		return false;
	}

	// Create sampler state for font texture

	D3D11_SAMPLER_DESC samplerDesc;
	ZeroMemory(&samplerDesc, sizeof(D3D11_SAMPLER_DESC));

	samplerDesc.Filter		   = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU	   = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressV	   = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressW	   = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.MipLODBias	   = 0.0f;
	samplerDesc.MaxAnisotropy  = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.BorderColor[0] = 0;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 0;
	samplerDesc.MinLOD		   = 0;
	samplerDesc.MaxLOD		   = D3D11_FLOAT32_MAX;

	if(FAILED(m_pD3d11Device->CreateSamplerState(&samplerDesc, &m_pFontSamplerState)))
	{
		return false;		
	};

	return InitialiseSentences();
}

//--------------------------------------------------------------------------------------
// Creates and initialises the pieces of text that are permanently present on the screen.
// Returns true if the sentences were created and initialised successfully, false otherwise.
//--------------------------------------------------------------------------------------
bool Renderer::InitialiseSentences(void)
{
	// Create the sentences

	// Calculate two reference points to make placement of sentences a bit more convenient
	int right = static_cast<int>(m_windowWidth) / 2;
	int top   = static_cast<int>(m_windowHeight) / 2;

	
	m_pPermanentSentences[LabelState] = new SentenceDrawable(7, &m_font, "State: ", right - 250, top -20, XMFLOAT3(1.0f, 1.0f, 1.0f));
	if(!m_pPermanentSentences[LabelState])
	{
		return false;
	}

	m_pPermanentSentences[TxtState] = new SentenceDrawable(10, &m_font, "Edit", right - 150, top -20, XMFLOAT3(1.0f, 1.0f, 1.0f));
	if(!m_pPermanentSentences[TxtState])
	{
		return false;
	}

	m_pPermanentSentences[LabelCursorPos] = new SentenceDrawable(8, &m_font, "Cursor: ", right - 250, top -40, XMFLOAT3(1.0f, 1.0f, 1.0f));
	if(!m_pPermanentSentences[LabelCursorPos])
	{
		return false;
	}
	m_pPermanentSentences[LabelCursorPosSeparators] = new SentenceDrawable(26, &m_font, "       |       ", right - 150, top -40, XMFLOAT3(1.0f, 1.0f, 1.0f));
	if(!m_pPermanentSentences[LabelCursorPosSeparators])
	{
		return false;
	}
	m_pPermanentSentences[TxtCursorPosX] = new SentenceDrawable(3, &m_font, "100", right - 150, top -40, XMFLOAT3(1.0f, 1.0f, 1.0f));
	if(!m_pPermanentSentences[TxtCursorPosX])
	{
		return false;
	}
	m_pPermanentSentences[TxtCursorPosY] = new SentenceDrawable(3, &m_font, "100", right - 100, top -40, XMFLOAT3(1.0f, 1.0f, 1.0f));
	if(!m_pPermanentSentences[TxtCursorPosY])
	{
		return false;
	}

	m_pPermanentSentences[LabelRotation] = new SentenceDrawable(10, &m_font, "Rotation: ", right - 250, top -60, XMFLOAT3(1.0f, 1.0f, 1.0f));
	if(!m_pPermanentSentences[LabelRotation])
	{
		return false;
	}
	m_pPermanentSentences[TxtRotation] = new SentenceDrawable(3, &m_font, "180", right - 150, top -60, XMFLOAT3(1.0f, 1.0f, 1.0f));
	if(!m_pPermanentSentences[TxtRotation])
	{
		return false;
	}

	m_pPermanentSentences[LabelObjectType] = new SentenceDrawable(8, &m_font, "Entity: ", right - 250, top -80, XMFLOAT3(1.0f, 1.0f, 1.0f));
	if(!m_pPermanentSentences[LabelObjectType])
	{
		return false;
	}
	m_pPermanentSentences[TxtObjectType] = new SentenceDrawable(20, &m_font, "Cover Position", right - 150, top -80, XMFLOAT3(1.0f, 1.0f, 1.0f));
	if(!m_pPermanentSentences[TxtObjectType])
	{
		return false;
	}

	m_pPermanentSentences[LabelFramerate] = new SentenceDrawable(12, &m_font, "Framerate: ", right - 250, top -100, XMFLOAT3(1.0f, 1.0f, 1.0f));
	if(!m_pPermanentSentences[LabelFramerate])
	{
		return false;
	}
	m_pPermanentSentences[TxtFramerate] = new SentenceDrawable(4, &m_font, "1000", right - 150, top -100, XMFLOAT3(1.0f, 1.0f, 1.0f));
	if(!m_pPermanentSentences[TxtFramerate])
	{
		return false;
	}

	m_pPermanentSentences[LabelTimeLeft] = new SentenceDrawable(6, &m_font, "Time: ", right - 250, top -140, XMFLOAT3(1.0f, 1.0f, 1.0f));
	if(!m_pPermanentSentences[LabelTimeLeft])
	{
		return false;
	}
	m_pPermanentSentences[TxtTimeLeft] = new SentenceDrawable(5, &m_font, "15:00", right - 150, top -140, XMFLOAT3(1.0f, 1.0f, 1.0f));
	if(!m_pPermanentSentences[TxtTimeLeft])
	{
		return false;
	}

	m_pPermanentSentences[LabelScore] = new SentenceDrawable(7, &m_font, "Score: ", right - 250, top -160, XMFLOAT3(1.0f, 1.0f, 1.0f));
	if(!m_pPermanentSentences[LabelScore])
	{
		return false;
	}
	m_pPermanentSentences[LabelScoreSeparators] = new SentenceDrawable(27, &m_font, "        |       ", right - 150, top -160, XMFLOAT3(1.0f, 1.0f, 1.0f));
	if(!m_pPermanentSentences[LabelScoreSeparators])
	{
		return false;
	}
	m_pPermanentSentences[TxtScoreRed] = new SentenceDrawable(4, &m_font, "1000", right - 150, top -160, XMFLOAT3(1.0f, 0.0f, 0.0f));
	if(!m_pPermanentSentences[TxtScoreRed])
	{
		return false;
	}
	m_pPermanentSentences[TxtScoreBlue] = new SentenceDrawable(4, &m_font, "1000", right - 90, top -160, XMFLOAT3(0.0f, 0.0f, 1.0f));
	if(!m_pPermanentSentences[TxtScoreBlue])
	{
		return false;
	}
	
	m_pPermanentSentences[LabelKills] = new SentenceDrawable(7, &m_font, "Kills: ", right - 250, top -180, XMFLOAT3(1.0f, 1.0f, 1.0f));
	if(!m_pPermanentSentences[LabelKills])
	{
		return false;
	}
	m_pPermanentSentences[LabelKillsSeparators] = new SentenceDrawable(27, &m_font, "        |       ", right - 150, top -180, XMFLOAT3(1.0f, 1.0f, 1.0f));
	if(!m_pPermanentSentences[LabelKillsSeparators])
	{
		return false;
	}
	m_pPermanentSentences[TxtKillsRed] = new SentenceDrawable(4, &m_font, "1000", right - 150, top -180, XMFLOAT3(1.0f, 0.0f, 0.0f));
	if(!m_pPermanentSentences[TxtKillsRed])
	{
		return false;
	}
	m_pPermanentSentences[TxtKillsBlue] = new SentenceDrawable(4, &m_font, "1000", right - 90, top -180, XMFLOAT3(0.0f, 0.0f, 1.0f));
	if(!m_pPermanentSentences[TxtKillsBlue])
	{
		return false;
	}

	m_pPermanentSentences[LabelShotsFired] = new SentenceDrawable(13, &m_font, "Shots fired: ", right - 250, top -200, XMFLOAT3(1.0f, 1.0f, 1.0f));
	if(!m_pPermanentSentences[LabelShotsFired])
	{
		return false;
	}
	m_pPermanentSentences[LabelShotsFiredSeparators] = new SentenceDrawable(27, &m_font, "        |       ", right - 150, top -200, XMFLOAT3(1.0f, 1.0f, 1.0f));
	if(!m_pPermanentSentences[LabelShotsFiredSeparators])
	{
		return false;
	}
	m_pPermanentSentences[TxtShotsFiredRed] = new SentenceDrawable(4, &m_font, "1000", right - 150, top -200, XMFLOAT3(1.0f, 0.0f, 0.0f));
	if(!m_pPermanentSentences[TxtShotsFiredRed])
	{
		return false;
	}
	m_pPermanentSentences[TxtShotsFiredBlue] = new SentenceDrawable(4, &m_font, "1000", right - 90, top -200, XMFLOAT3(0.0f, 0.0f, 1.0f));
	if(!m_pPermanentSentences[TxtShotsFiredBlue])
	{
		return false;
	}


	// Initialise the sentences

	for(int i = 0; i < NumberOfSentences; ++i)
	{
		if(!m_pPermanentSentences[i]->Initialise(m_pD3d11Device))
		{
			return false;
		}
	}

	return true;
}

//--------------------------------------------------------------------------------------
// Cleanup renderer, free ressources.
//--------------------------------------------------------------------------------------
void Renderer::Cleanup( void )
{
	// Cleanup all drawables
	for(int i = 0; i < NumberOfDrawableTypes; ++i)
	{
		if(m_drawableObjects[i])
		{
			m_drawableObjects[i]->Cleanup();
			delete m_drawableObjects[i];
			m_drawableObjects[i] = nullptr;
		}
	}

	for(int i = 0; i < NumberOfShaderTypes; ++i)
	{
		m_shaderGroups[i].Cleanup();
	}

	for(int i = 0; i < NumberOfSentences; ++i)
	{
		if(m_pPermanentSentences[i])
		{
			m_pPermanentSentences[i]->Cleanup();
			delete m_pPermanentSentences[i];
			m_pPermanentSentences[i] = nullptr;
		}
	}

	if(m_pFontSamplerState)
	{
		m_pFontSamplerState->Release();
		m_pFontSamplerState = nullptr;
	}

	// Cleanup all the DirectX stuff
	if(m_pSwapChain)		  { m_pSwapChain->Release();          m_pSwapChain		    = nullptr; }
	if(m_pD3d11Device)		  { m_pD3d11Device->Release();        m_pD3d11Device	    = nullptr; }
	if(m_pD3d11DeviceContext) { m_pD3d11DeviceContext->Release(); m_pD3d11DeviceContext = nullptr; }
	if(m_pRenderTargetView)	  { m_pRenderTargetView->Release();   m_pRenderTargetView   = nullptr; }
	if(m_pDepthStencilView)   { m_pDepthStencilView->Release();   m_pDepthStencilView   = nullptr; }
	if(m_pDepthStencilBuffer) { m_pDepthStencilBuffer->Release(); m_pDepthStencilBuffer = nullptr; }

	if(m_pDepthEnabledStencilState)	     { m_pDepthEnabledStencilState->Release();      m_pDepthEnabledStencilState	     = nullptr; }
	if(m_pDepthDisabledStencilState)	 { m_pDepthDisabledStencilState->Release();     m_pDepthDisabledStencilState     = nullptr; }
	if(m_pRasterStateCullBackfaces)	     { m_pRasterStateCullBackfaces->Release();      m_pRasterStateCullBackfaces      = nullptr; }
	if(m_pBlendingEnabledBlendingState)  { m_pBlendingEnabledBlendingState->Release();  m_pBlendingEnabledBlendingState  = nullptr; }
	if(m_pBlendingDisabledBlendingState) { m_pBlendingDisabledBlendingState->Release(); m_pBlendingDisabledBlendingState = nullptr; }
}

//--------------------------------------------------------------------------------------
// Render the current scene.
// Param1: The view matrix of the camera for which to render the frame.
// Param2: The projection matrix of the camera for which to render the frame.
// Param3: The current application data, required for text rendering to provide information to the user.
// Param4: The current game context, required for text rendering.
//--------------------------------------------------------------------------------------
void Renderer::RenderScene(const XMFLOAT4X4& viewMatrix, const XMFLOAT4X4& projectionMatrix, const AppData& appData, const GameContext* pGameContext)
{
	// Clear the backbuffer
	m_pD3d11DeviceContext->ClearRenderTargetView(m_pRenderTargetView, g_kBackgroundColour);

	// Refresh the Depth/Stencil view
	m_pD3d11DeviceContext->ClearDepthStencilView(m_pDepthStencilView, D3D11_CLEAR_DEPTH|D3D11_CLEAR_STENCIL, 1.0f, 0);
	
	// Render the test environment
	RenderTestEnvironment(viewMatrix, projectionMatrix);

	// Render the text (GUI)
	RenderText(appData, pGameContext);

	// Present the backbuffer to the screen
	m_pSwapChain->Present(0, 0);

	// Reset the render context for the next frame
	m_renderContext.Reset();
}

//--------------------------------------------------------------------------------------
// Renders the test environment.
// Param1: The view matrix of the camera for which to render the frame.
// Param2: The projection matrix of the camera for which to render the frame.
//--------------------------------------------------------------------------------------
void Renderer::RenderTestEnvironment(const XMFLOAT4X4& viewMatrix, const XMFLOAT4X4& projectionMatrix)
{
	PrepareDefaultGeometryRendering();

	// Calculate the view projection matrix
	XMFLOAT4X4 viewProjection;
	XMStoreFloat4x4(&viewProjection, XMLoadFloat4x4(&viewMatrix) * XMLoadFloat4x4(&projectionMatrix));

	// Render the grid

	// Update the per object data according to the current instance
	m_perObjectData.m_colour = g_kGridColour;
	XMStoreFloat4x4(&m_perObjectData.m_worldViewProjection, XMMatrixTranslation(0.0f, 0.0f, -5.0f) * XMLoadFloat4x4(&viewProjection));
	// Update the shader's constant buffer
	m_shaderGroups[m_currentShaderGroup].SetObjectData(m_pD3d11DeviceContext, m_perObjectData);
	// Render the grid drawable
	m_drawableObjects[GridType]->Draw(m_pD3d11DeviceContext);


	// Render the objects collected by the render context
	for(int i = 0; i < NumberOfObjectTypes; ++i)
	{
		for(int k = 0; k < m_renderContext.GetObjectCount(ObjectType(i)); ++k)
		{
			// Update the per object data according to the current instance
			m_perObjectData.m_colour = m_objectRenderData[i].m_colour;

			XMMATRIX zTranslationMatrix = XMMatrixTranslation(0.0f, 0.0f, m_objectRenderData[i].m_baseZ);

			XMStoreFloat4x4(&m_perObjectData.m_worldViewProjection, zTranslationMatrix * XMLoadFloat4x4(&m_renderContext.GetInstances(ObjectType(i))[k].m_world) * XMLoadFloat4x4(&viewProjection));
			// Update the shader's constant buffer
			m_shaderGroups[m_currentShaderGroup].SetObjectData(m_pD3d11DeviceContext, m_perObjectData);
			// Draw the object
			m_drawableObjects[m_objectRenderData[i].m_drawableType]->Draw(m_pD3d11DeviceContext);
		}
	}
}

//--------------------------------------------------------------------------------------
// Renders all the text.
// Param1: The current application data.
// Param2: The context of the current game.
//--------------------------------------------------------------------------------------
void Renderer::RenderText(const AppData& appData, const GameContext* pGameContext)
{
	// Update the sentences
	UpdateSentences(appData, pGameContext);

	PrepareTextRendering();

	// Set the font texture to the font pixel shader
	m_shaderGroups[m_currentShaderGroup].SetTexture(0, m_pD3d11DeviceContext, m_font.GetTexture(), m_pFontSamplerState, true);
	
	// Set the per frame data for the shaders
	XMStoreFloat4x4(&m_perFrameData.m_viewProjectionText, XMLoadFloat4x4(&m_baseViewMatrix) * XMLoadFloat4x4(&m_baseProjectionMatrix));
	m_shaderGroups[m_currentShaderGroup].SetFrameData(m_pD3d11DeviceContext, m_perFrameData);

	// Render the sentences
	for(int i = 0; i < NumberOfSentences; ++i)
	{
		// Set per object data for shaders
		m_perObjectData.m_colour = m_pPermanentSentences[i]->GetColour();
		m_shaderGroups[m_currentShaderGroup].SetObjectData(m_pD3d11DeviceContext, m_perObjectData);

		// Render the sentence
		m_pPermanentSentences[i]->Draw(m_pD3d11DeviceContext);
	}
}

//--------------------------------------------------------------------------------------
// Update the text on the screen.
// Param1: The current application data.
// Param2: The current game context.
//--------------------------------------------------------------------------------------
void Renderer::UpdateSentences(const AppData& appData, const GameContext* pGameContext)
{
	// Note: Only update sentences when their content changed.

	switch(appData.m_applicationState)
	{
	case EditMode:
		m_pPermanentSentences[TxtState]->SetText("Edit");
		break;
	case SimulationRunning:
		m_pPermanentSentences[TxtState]->SetText("Simulation");
		break;
	case SimulationPaused:
		m_pPermanentSentences[TxtState]->SetText("Paused");
		break;
	}

	char bufferCursorX[4];
	if(appData.m_cursorGridPosX >= 0)
	{
		_itoa_s(appData.m_cursorGridPosX, bufferCursorX, 4, 10);
		m_pPermanentSentences[TxtCursorPosX]->SetText(bufferCursorX);
	}else
	{
		m_pPermanentSentences[TxtCursorPosX]->SetText("---");
	}

	char bufferCursorY[4];
	if(appData.m_cursorGridPosY >= 0)
	{
		_itoa_s(appData.m_cursorGridPosY, bufferCursorY, 4, 10);
		m_pPermanentSentences[TxtCursorPosY]->SetText(bufferCursorY);
	}else
	{
		m_pPermanentSentences[TxtCursorPosY]->SetText("---");
	}

	char bufferRotation[4];
	_itoa_s(static_cast<int>(appData.m_currentRotation), bufferRotation, 4, 10);
	m_pPermanentSentences[TxtRotation]->SetText(bufferRotation);

	m_pPermanentSentences[TxtObjectType]->SetText(m_objectRenderData[appData.m_selectedObjectType].m_name);

	char bufferFramerate[5];
	if(appData.m_framesPerSecond <= 9999)
	{
		_itoa_s(appData.m_framesPerSecond, bufferFramerate, 5, 10);
		m_pPermanentSentences[TxtFramerate]->SetText(bufferFramerate);
	}else
	{
		m_pPermanentSentences[TxtFramerate]->SetText("9999");
	}

	char bufferTime[6];
	if(pGameContext->GetTimeLeft() <= 3600)
	{
		if(pGameContext->GetTimeLeft() > 0.0f)
		{
			unsigned int minutes = static_cast<unsigned int>(pGameContext->GetTimeLeft()) / 60;
			unsigned int seconds = static_cast<unsigned int>(pGameContext->GetTimeLeft()) % 60;

			_snprintf_s(bufferTime, 6, 6, "%02d:%02d", minutes, seconds);
			m_pPermanentSentences[TxtTimeLeft]->SetText(bufferTime);
		}else
		{
			m_pPermanentSentences[TxtTimeLeft]->SetText("00:00");
		}
	}else
	{
		m_pPermanentSentences[TxtTimeLeft]->SetText("60:00");
	}

	char bufferScoreRed[5];
	if(pGameContext->GetScore(TeamRed) <= 9999)
	{
		_itoa_s(pGameContext->GetScore(TeamRed), bufferScoreRed, 5, 10);
		m_pPermanentSentences[TxtScoreRed]->SetText(bufferScoreRed);
	}else
	{
		m_pPermanentSentences[TxtScoreRed]->SetText("9999");
	}

	char bufferScoreBlue[5];
	if(pGameContext->GetScore(TeamBlue) <= 9999)
	{
		_itoa_s(pGameContext->GetScore(TeamBlue), bufferScoreBlue, 5, 10);
		m_pPermanentSentences[TxtScoreBlue]->SetText(bufferScoreBlue);
	}else
	{
		m_pPermanentSentences[TxtScoreBlue]->SetText("9999");
	}

	char bufferKillsRed[5];
	if(pGameContext->GetKills(TeamRed) <= 9999)
	{
		_itoa_s(pGameContext->GetKills(TeamRed), bufferKillsRed, 5, 10);
		m_pPermanentSentences[TxtKillsRed]->SetText(bufferKillsRed);
	}else
	{
		m_pPermanentSentences[TxtKillsRed]->SetText("9999");
	}

	char bufferKillsBlue[5];
	if(pGameContext->GetKills(TeamBlue) <= 9999)
	{
		_itoa_s(pGameContext->GetKills(TeamBlue), bufferKillsBlue, 5, 10);
		m_pPermanentSentences[TxtKillsBlue]->SetText(bufferKillsBlue);
	}else
	{
		m_pPermanentSentences[TxtKillsBlue]->SetText("9999");
	}

	char bufferShotsFiredRed[5];
	if(pGameContext->GetShotsFired(TeamRed) <= 9999)
	{
		_itoa_s(pGameContext->GetShotsFired(TeamRed), bufferShotsFiredRed, 5, 10);
		m_pPermanentSentences[TxtShotsFiredRed]->SetText(bufferShotsFiredRed);
	}else
	{
		m_pPermanentSentences[TxtShotsFiredRed]->SetText("9999");
	}

	char bufferShotsFiredBlue[5];
	if(pGameContext->GetShotsFired(TeamBlue) <= 9999)
	{
		_itoa_s(pGameContext->GetShotsFired(TeamBlue), bufferShotsFiredBlue, 5, 10);
		m_pPermanentSentences[TxtShotsFiredBlue]->SetText(bufferShotsFiredBlue);
	}else
	{
		m_pPermanentSentences[TxtShotsFiredBlue]->SetText("9999");
	}

}

//--------------------------------------------------------------------------------------
// Prepares the renderer for a new test environment by changing the grid's extents accordingly.
// Param1: The size of a side of the square grid.
// Param2: The number of grid fields along one axis of the square grid
// Returns true if the new grid was set up successfully, false otherwise.
//--------------------------------------------------------------------------------------
bool Renderer::SetupGrid(float gridSize, unsigned int numberOfGridPartitions)
{
	// Delete the old grid
	if(m_drawableObjects[GridType])
	{
		m_drawableObjects[GridType]->Cleanup();
		delete m_drawableObjects[GridType];
		m_drawableObjects[GridType] = nullptr;
	}

	m_drawableObjects[GridType] = new GridDrawable(gridSize, gridSize, numberOfGridPartitions, numberOfGridPartitions);
	if(!m_drawableObjects[GridType])
	{
		return false;
	}else if(!m_drawableObjects[GridType]->Initialise(m_pD3d11Device))
	{
		return false;
	}

	return true;
}

//--------------------------------------------------------------------------------------
// Sets the default render states and shaders for the default rendering of geometry.
//--------------------------------------------------------------------------------------
void Renderer::PrepareDefaultGeometryRendering(void)
{
	// Enable backface culling
	m_pD3d11DeviceContext->RSSetState(m_pRasterStateCullBackfaces);

	// Enable depth buffering
	m_pD3d11DeviceContext->OMSetDepthStencilState(m_pDepthEnabledStencilState, 1);
	
	// Disable alpha blending
	float blendFactor[4] = {0.0f, 0.0f, 0.0f, 0.0f};
	m_pD3d11DeviceContext -> OMSetBlendState(m_pBlendingDisabledBlendingState, blendFactor, 0xffffffff);

	SetShaderGroup(SimpleUnlit);

	m_shaderGroups[m_currentShaderGroup].Activate(m_pD3d11DeviceContext);
}

//--------------------------------------------------------------------------------------
// Sets the default render states and shaders for text rendering.
//--------------------------------------------------------------------------------------
void Renderer::PrepareTextRendering(void)
{
	// Enable backface culling
	m_pD3d11DeviceContext->RSSetState(m_pRasterStateCullBackfaces);

	// Disable depth buffering
	m_pD3d11DeviceContext->OMSetDepthStencilState(m_pDepthDisabledStencilState, 1);
	
	// Turn on alpha blending
	float blendFactor[4] = {0.0f, 0.0f, 0.0f, 0.0f};
	m_pD3d11DeviceContext -> OMSetBlendState(m_pBlendingEnabledBlendingState, blendFactor, 0xffffffff);

	SetShaderGroup(SimpleFont);

	m_shaderGroups[m_currentShaderGroup].Activate(m_pD3d11DeviceContext);
}

// Data access functions

void Renderer::SetShaderGroup(ShaderType type)
{
	m_currentShaderGroup = type;
}

RenderContext& Renderer::GetRenderContext(void)
{
	return m_renderContext;
}