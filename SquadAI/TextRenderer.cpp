/* 
*  Kevin Meergans, Improved Lego Editor, 2014
*  TextRendererImplementation.cpp
*  Contains the function definitions for the TextRendererImplementation class.
*/

#include "TextRendererImplementation.h"


TextRendererImplementation::TextRendererImplementation() : 	m_pVertexShader( nullptr ),
															m_pPixelShader( nullptr ),
															m_pVertexInputLayout( nullptr ),
															m_pCbFontColourBuffer( nullptr ),
															m_pCbPerFrameBuffer( nullptr ),
															m_pFontSamplerState( nullptr )
{
}

//--------------------------------------------------------------------------------------
// Initialise the text renderer.
//--------------------------------------------------------------------------------------
HRESULT TextRendererImplementation::Initialise(  ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, HWND hwnd, int screenWidth, int screenHeight, int maxSentenceLength, const XMFLOAT4X4& baseViewMatrix )
{
	HRESULT hr;

	m_screenWidth    = screenWidth;
	m_screenHeight   = screenHeight;
	m_baseViewMatrix = baseViewMatrix;

	// Initialise font object

	hr = m_font.Initialise( pDevice, "Font/FontData3.csv", L"Font/ExportedFont3.bmp" );
	if( FAILED( hr ) )
	{
		return hr;
	}

	pTex = m_font.GetTexture();

	// Create shaders

	hr = pDevice -> CreateVertexShader( g_fontVertexShader, sizeof( g_fontVertexShader ), nullptr, &m_pVertexShader );
	if( FAILED ( hr ) )
		return hr;

	hr = pDevice -> CreatePixelShader( g_fontPixelShader, sizeof( g_fontPixelShader ), nullptr, &m_pPixelShader );
	if( FAILED ( hr ) )
		return hr;

	// Create vertex input layout

	UINT numElements = ARRAYSIZE( FontVertexLayout );
	hr = pDevice -> CreateInputLayout( FontVertexLayout, numElements, g_fontVertexShader, sizeof( g_fontVertexShader ), &m_pVertexInputLayout );
	if( FAILED ( hr ) )
		return hr;

	// Create constant buffers

	D3D11_BUFFER_DESC cbDescFontColour;	
	ZeroMemory( &cbDescFontColour, sizeof( D3D11_BUFFER_DESC ) );

	cbDescFontColour.Usage = D3D11_USAGE_DYNAMIC;
	cbDescFontColour.ByteWidth = sizeof( m_cbFontColour );
	cbDescFontColour.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbDescFontColour.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cbDescFontColour.MiscFlags = 0;

	hr = pDevice -> CreateBuffer( &cbDescFontColour, NULL, &m_pCbFontColourBuffer );
	if( FAILED ( hr ) )
		return hr;

	D3D11_BUFFER_DESC cbDescPerFrame;	
	ZeroMemory( &cbDescPerFrame, sizeof( D3D11_BUFFER_DESC ) );

	cbDescPerFrame.Usage = D3D11_USAGE_DYNAMIC;
	cbDescPerFrame.ByteWidth = sizeof( m_cbPerFrame );
	cbDescPerFrame.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbDescPerFrame.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cbDescPerFrame.MiscFlags = 0;

	hr = pDevice -> CreateBuffer( &cbDescPerFrame, NULL, &m_pCbPerFrameBuffer );
	if( FAILED ( hr ) )
		return hr;

	// Create texture sampler state

	D3D11_SAMPLER_DESC samplerDesc;
	ZeroMemory( &samplerDesc, sizeof( D3D11_SAMPLER_DESC ) );

	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.BorderColor[0] = 0;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 0;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	hr = pDevice -> CreateSamplerState(&samplerDesc, &m_pFontSamplerState);
	if( FAILED( hr ) )
	{
		return false;
	}

	InitialiseSentences( pDevice, pDeviceContext, maxSentenceLength );


	// set shaders and constant shader resources
	/*
	pDeviceContext -> IASetInputLayout(m_pVertexInputLayout);
	pDeviceContext -> VSSetShader(m_pVertexShader, NULL, 0);
	pDeviceContext -> PSSetShader(m_pPixelShader, NULL, 0);

	ID3D11ShaderResourceView* pTex = m_font.GetTexture();
	pDeviceContext -> PSSetShaderResources(0, 1, &pTex );

	pDeviceContext -> PSSetSamplers(0, 1, &m_pFontSamplerState);
	*/

	return S_OK;
}

//--------------------------------------------------------------------------------------
// Initialise the sentences needed for the scene.
//--------------------------------------------------------------------------------------
HRESULT TextRendererImplementation::InitialiseSentences( ID3D11Device* pDevice, ID3D11DeviceContext* pContext, int maxLength )
{
	HRESULT hr;

	// Create initial sentence data
	
	// Static text

	m_sentenceData[TxtCursorPosTemplate].m_colourRGB = XMFLOAT3( 1.0f, 1.0f, 1.0f );
	m_sentenceData[TxtCursorPosTemplate].m_drawX = ( m_screenWidth / 2 ) - 150;
	m_sentenceData[TxtCursorPosTemplate].m_drawY = ( m_screenHeight / 2) - 10;
	m_sentenceData[TxtCursorPosTemplate].m_text = "       |        |     ";

	m_sentenceData[TxtCursorPosLabel].m_colourRGB = XMFLOAT3( 1.0f, 1.0f, 1.0f );
	m_sentenceData[TxtCursorPosLabel].m_drawX = ( m_screenWidth / 2 ) - 300;
	m_sentenceData[TxtCursorPosLabel].m_drawY = ( m_screenHeight / 2) - 10;
	m_sentenceData[TxtCursorPosLabel].m_text = "Cursor: ";

	m_sentenceData[TxtBrickTypeLabel].m_colourRGB = XMFLOAT3( 1.0f, 1.0f, 1.0f );
	m_sentenceData[TxtBrickTypeLabel].m_drawX = ( m_screenWidth / 2 ) - 300;
	m_sentenceData[TxtBrickTypeLabel].m_drawY = ( m_screenHeight / 2) - 30;
	m_sentenceData[TxtBrickTypeLabel].m_text = "Brick Type: ";

	m_sentenceData[TxtColourLabel].m_colourRGB = XMFLOAT3( 1.0f, 1.0f, 1.0f );
	m_sentenceData[TxtColourLabel].m_drawX = ( m_screenWidth / 2 ) - 300;
	m_sentenceData[TxtColourLabel].m_drawY = ( m_screenHeight / 2) - 50;
	m_sentenceData[TxtColourLabel].m_text = "Colour: ";

	m_sentenceData[TxtRotationLabel].m_colourRGB = XMFLOAT3( 1.0f, 1.0f, 1.0f );
	m_sentenceData[TxtRotationLabel].m_drawX = ( m_screenWidth / 2 ) - 300;
	m_sentenceData[TxtRotationLabel].m_drawY = ( m_screenHeight / 2) - 70;
	m_sentenceData[TxtRotationLabel].m_text = "Rotation: ";

	m_sentenceData[TxtRecModeLabel].m_colourRGB = XMFLOAT3( 1.0f, 1.0f, 1.0f );
	m_sentenceData[TxtRecModeLabel].m_drawX = ( m_screenWidth / 2 ) - 300;
	m_sentenceData[TxtRecModeLabel].m_drawY = ( m_screenHeight / 2) - 90;
	m_sentenceData[TxtRecModeLabel].m_text = "Recording Mode: ";

	m_sentenceData[TxtDelModeLabel].m_colourRGB = XMFLOAT3( 1.0f, 1.0f, 1.0f );
	m_sentenceData[TxtDelModeLabel].m_drawX = ( m_screenWidth / 2 ) - 300;
	m_sentenceData[TxtDelModeLabel].m_drawY = ( m_screenHeight / 2) - 110;
	m_sentenceData[TxtDelModeLabel].m_text = "Deletion Mode: ";

	m_sentenceData[TxtBrickNumberLabel].m_colourRGB = XMFLOAT3( 1.0f, 1.0f, 1.0f );
	m_sentenceData[TxtBrickNumberLabel].m_drawX = ( m_screenWidth / 2 ) - 300;
	m_sentenceData[TxtBrickNumberLabel].m_drawY = ( m_screenHeight / 2) - 130;
	m_sentenceData[TxtBrickNumberLabel].m_text = "Bricks in scene: ";

	m_sentenceData[TxtBricksRenderedLabel].m_colourRGB = XMFLOAT3( 1.0f, 1.0f, 1.0f );
	m_sentenceData[TxtBricksRenderedLabel].m_drawX = ( m_screenWidth / 2 ) - 300;
	m_sentenceData[TxtBricksRenderedLabel].m_drawY = ( m_screenHeight / 2) - 150;
	m_sentenceData[TxtBricksRenderedLabel].m_text = "Bricks rendered: ";

	m_sentenceData[TxtStudNumberLabel].m_colourRGB = XMFLOAT3( 1.0f, 1.0f, 1.0f );
	m_sentenceData[TxtStudNumberLabel].m_drawX = ( m_screenWidth / 2 ) - 300;
	m_sentenceData[TxtStudNumberLabel].m_drawY = ( m_screenHeight / 2) - 170;
	m_sentenceData[TxtStudNumberLabel].m_text = "Studs in scene: ";

	m_sentenceData[TxtStudsRenderedLabel].m_colourRGB = XMFLOAT3( 1.0f, 1.0f, 1.0f );
	m_sentenceData[TxtStudsRenderedLabel].m_drawX = ( m_screenWidth / 2 ) - 300;
	m_sentenceData[TxtStudsRenderedLabel].m_drawY = ( m_screenHeight / 2) - 190;
	m_sentenceData[TxtStudsRenderedLabel].m_text = "Studs rendered: ";

	m_sentenceData[TxtShaderLabel].m_colourRGB = XMFLOAT3( 1.0f, 1.0f, 1.0f );
	m_sentenceData[TxtShaderLabel].m_drawX = ( m_screenWidth / 2 ) - 300;
	m_sentenceData[TxtShaderLabel].m_drawY = ( m_screenHeight / 2) - 210;
	m_sentenceData[TxtShaderLabel].m_text = "Shader: ";

	m_sentenceData[TxtFramerateLabel].m_colourRGB = XMFLOAT3( 1.0f, 1.0f, 1.0f );
	m_sentenceData[TxtFramerateLabel].m_drawX = ( m_screenWidth / 2 ) - 300;
	m_sentenceData[TxtFramerateLabel].m_drawY = ( m_screenHeight / 2) - 230;
	m_sentenceData[TxtFramerateLabel].m_text = "Framerate: ";

	// Dynamic text

	m_sentenceData[TxtCursorPosX].m_colourRGB = XMFLOAT3( 1.0f, 1.0f, 1.0f );
	m_sentenceData[TxtCursorPosX].m_drawX = ( m_screenWidth / 2 ) - 150;
	m_sentenceData[TxtCursorPosX].m_drawY = ( m_screenHeight / 2) - 10;
	m_sentenceData[TxtCursorPosX].m_text = "-614";

	m_sentenceData[TxtCursorPosY].m_colourRGB = XMFLOAT3( 1.0f, 1.0f, 1.0f );
	m_sentenceData[TxtCursorPosY].m_drawX = ( m_screenWidth / 2 ) - 100;
	m_sentenceData[TxtCursorPosY].m_drawY = ( m_screenHeight / 2) - 10;
	m_sentenceData[TxtCursorPosY].m_text = "-322";

	m_sentenceData[TxtCursorPosZ].m_colourRGB = XMFLOAT3( 1.0f, 1.0f, 1.0f );
	m_sentenceData[TxtCursorPosZ].m_drawX = ( m_screenWidth / 2 ) - 50;
	m_sentenceData[TxtCursorPosZ].m_drawY = ( m_screenHeight / 2) - 10;
	m_sentenceData[TxtCursorPosZ].m_text = "-545";

	m_sentenceData[TxtBrickType].m_colourRGB = XMFLOAT3( 1.0f, 1.0f, 1.0f );
	m_sentenceData[TxtBrickType].m_drawX = ( m_screenWidth / 2 ) - 150;
	m_sentenceData[TxtBrickType].m_drawY = ( m_screenHeight / 2) - 30;
	m_sentenceData[TxtBrickType].m_text = "Std2x3x4";

	m_sentenceData[TxtColour].m_colourRGB = XMFLOAT3( 1.0f, 1.0f, 1.0f );
	m_sentenceData[TxtColour].m_drawX = ( m_screenWidth / 2 ) - 150;
	m_sentenceData[TxtColour].m_drawY = ( m_screenHeight / 2) - 50;
	m_sentenceData[TxtColour].m_text = "Dark Blue";

	m_sentenceData[TxtRotation].m_colourRGB = XMFLOAT3( 1.0f, 1.0f, 1.0f );
	m_sentenceData[TxtRotation].m_drawX = ( m_screenWidth / 2 ) - 150;
	m_sentenceData[TxtRotation].m_drawY = ( m_screenHeight / 2) - 70;
	m_sentenceData[TxtRotation].m_text = "90";

	m_sentenceData[TxtRecMode].m_colourRGB = XMFLOAT3( 1.0f, 1.0f, 1.0f );
	m_sentenceData[TxtRecMode].m_drawX = ( m_screenWidth / 2 ) - 150;
	m_sentenceData[TxtRecMode].m_drawY = ( m_screenHeight / 2) - 90;
	m_sentenceData[TxtRecMode].m_text = "OFF";

	m_sentenceData[TxtDelMode].m_colourRGB = XMFLOAT3( 1.0f, 1.0f, 1.0f );
	m_sentenceData[TxtDelMode].m_drawX = ( m_screenWidth / 2 ) - 150;
	m_sentenceData[TxtDelMode].m_drawY = ( m_screenHeight / 2) - 110;
	m_sentenceData[TxtDelMode].m_text = "ON";

	m_sentenceData[TxtBrickNumber].m_colourRGB = XMFLOAT3( 1.0f, 1.0f, 1.0f );
	m_sentenceData[TxtBrickNumber].m_drawX = ( m_screenWidth / 2 ) - 150;
	m_sentenceData[TxtBrickNumber].m_drawY = ( m_screenHeight / 2) - 130;
	m_sentenceData[TxtBrickNumber].m_text = "4563";

	m_sentenceData[TxtBricksRendered].m_colourRGB = XMFLOAT3( 1.0f, 1.0f, 1.0f );
	m_sentenceData[TxtBricksRendered].m_drawX = ( m_screenWidth / 2 ) - 150;
	m_sentenceData[TxtBricksRendered].m_drawY = ( m_screenHeight / 2) - 150;
	m_sentenceData[TxtBricksRendered].m_text = "1278";

	m_sentenceData[TxtStudNumber].m_colourRGB = XMFLOAT3( 1.0f, 1.0f, 1.0f );
	m_sentenceData[TxtStudNumber].m_drawX = ( m_screenWidth / 2 ) - 150;
	m_sentenceData[TxtStudNumber].m_drawY = ( m_screenHeight / 2) - 170;
	m_sentenceData[TxtStudNumber].m_text = "4563";

	m_sentenceData[TxtStudsRendered].m_colourRGB = XMFLOAT3( 1.0f, 1.0f, 1.0f );
	m_sentenceData[TxtStudsRendered].m_drawX = ( m_screenWidth / 2 ) - 150;
	m_sentenceData[TxtStudsRendered].m_drawY = ( m_screenHeight / 2) - 190;
	m_sentenceData[TxtStudsRendered].m_text = "1278";

	m_sentenceData[TxtShader].m_colourRGB = XMFLOAT3( 1.0f, 1.0f, 1.0f );
	m_sentenceData[TxtShader].m_drawX = ( m_screenWidth / 2 ) - 150;
	m_sentenceData[TxtShader].m_drawY = ( m_screenHeight / 2) - 210;
	m_sentenceData[TxtShader].m_text = "Simple Unlit";

	m_sentenceData[TxtFramerate].m_colourRGB = XMFLOAT3( 1.0f, 1.0f, 1.0f );
	m_sentenceData[TxtFramerate].m_drawX = ( m_screenWidth / 2 ) - 150;
	m_sentenceData[TxtFramerate].m_drawY = ( m_screenHeight / 2) - 230;
	m_sentenceData[TxtFramerate].m_text = "134.87";


	for( int i = 0; i < NumberOfSentences; ++i )
	{

		hr = m_sentences[i].Initialise( pDevice, maxLength );
		if( FAILED( hr ) )
		{
			return hr;
		}

		hr = m_sentences[i].Update( pContext, &m_font, m_sentenceData[i] );
		if( FAILED( hr ) )
		{
			return hr;
		}
	}

	return S_OK;
}

//--------------------------------------------------------------------------------------
// Update the contents of the sentences based on the current editor state.
//--------------------------------------------------------------------------------------
void TextRendererImplementation::UpdateSentences( ID3D11DeviceContext* pDeviceContext, const EditorData& editorData, int numberOfBricks, int bricksRendered, int numberOfStuds, int studsRendered, const PerformanceData& performanceData )
{
	// Change sentence data according to editor state/user input

	//char *  itoa ( int value, char * str, int base ); // for ints
	//int snprintf ( char * s, size_t n, const char * format, ... ); for floats

	char bufCursorX[5];
	if( abs( editorData.m_cursorPosition.m_x ) < 1000 )
	{
		_itoa_s( editorData.m_cursorPosition.m_x, bufCursorX, 5, 10);
		m_sentenceData[TxtCursorPosX].m_text = bufCursorX;
	}else
	{
		m_sentenceData[TxtCursorPosX].m_text = "----";
	}
	
	char bufCursorY[5];
	if( abs( editorData.m_cursorPosition.m_y ) < 1000 )
	{
		_itoa_s( editorData.m_cursorPosition.m_y, bufCursorY, 5, 10);
		m_sentenceData[TxtCursorPosY].m_text = bufCursorY;
	}else
	{
		m_sentenceData[TxtCursorPosY].m_text = "----";
	}

	char bufCursorZ[5];
	if( abs( editorData.m_cursorPosition.m_z ) < 1000 )
	{
		_itoa_s( editorData.m_cursorPosition.m_z, bufCursorZ, 5, 10);
	m_sentenceData[TxtCursorPosZ].m_text = bufCursorZ;
	}else
	{
		m_sentenceData[TxtCursorPosZ].m_text = "----";
	}

	if( editorData.m_selectedBuildingBlock < NumberOfBrickTypes )
	{
		m_sentenceData[TxtBrickType].m_text = BrickTypeNames[editorData.m_selectedBuildingBlock].c_str();
	}else
	{
		m_sentenceData[TxtBrickType].m_text = editorData.m_customStructureName;
	}

	m_sentenceData[TxtColour].m_text = BrickColourNames[editorData.m_selectedBrickColour].c_str();

	char bufRotation[5];
	_itoa_s( editorData.m_brickRotation, bufRotation, 5, 10);

	m_sentenceData[TxtRotation].m_text = bufRotation;

	if( editorData.m_isRecordingModeActive )
	{
		m_sentenceData[TxtRecMode].m_text = "ON";
	}else
	{
		m_sentenceData[TxtRecMode].m_text = "OFF";
	}

	if( editorData.m_isDeletionModeActive )
	{
		m_sentenceData[TxtDelMode].m_text = "ON";
	}else
	{
		m_sentenceData[TxtDelMode].m_text = "OFF";
	}

	char bufNumberOfBricks[8];
	_itoa_s( numberOfBricks, bufNumberOfBricks, 8, 10);
	m_sentenceData[TxtBrickNumber].m_text = bufNumberOfBricks;

	char bufBricksRendered[8];
	_itoa_s( bricksRendered, bufBricksRendered, 8, 10);
	m_sentenceData[TxtBricksRendered].m_text = bufBricksRendered;

	char bufNumberOfStuds[8];
	_itoa_s( numberOfStuds, bufNumberOfStuds, 8, 10);
	m_sentenceData[TxtStudNumber].m_text = bufNumberOfStuds;

	char bufStudsRendered[8];
	_itoa_s( studsRendered, bufStudsRendered, 8, 10);
	m_sentenceData[TxtStudsRendered].m_text = bufStudsRendered;

	m_sentenceData[TxtShader].m_text = ShaderGroupNames[editorData.m_selectedShaderGroup].c_str();

	char bufFPS[7];
	_itoa_s( performanceData.m_framesPerSecond, bufFPS, 7, 10);
	m_sentenceData[TxtFramerate].m_text = bufFPS;

	//m_sentences[TxtColour].Update( pDeviceContext, &m_font, m_sentenceData[TxtColour]);
	// Only update the parts that can actually change
	for( int i = TxtCursorPosX; i < NumberOfSentences; ++i )
	{
		m_sentences[i].Update( pDeviceContext, &m_font, m_sentenceData[i] );
	}
}


//--------------------------------------------------------------------------------------
// Release allocated resources.
//--------------------------------------------------------------------------------------
HRESULT TextRendererImplementation::Cleanup()
{
	for( int i = 0; i < NumberOfSentences; ++i )
	{
		m_sentences[i].Cleanup();
	}

	if( m_pVertexShader )		{ m_pVertexShader		-> Release(); m_pVertexShader		= nullptr; }
	if( m_pPixelShader )		{ m_pPixelShader		-> Release(); m_pPixelShader		= nullptr; }
	if( m_pVertexInputLayout )	{ m_pVertexInputLayout  -> Release(); m_pVertexInputLayout	= nullptr; }
	if( m_pCbFontColourBuffer )	{ m_pCbFontColourBuffer -> Release(); m_pCbFontColourBuffer = nullptr; }
	if( m_pCbPerFrameBuffer )	{ m_pCbPerFrameBuffer	-> Release(); m_pCbPerFrameBuffer	= nullptr; }
	if( m_pFontSamplerState )	{ m_pFontSamplerState	-> Release(); m_pFontSamplerState	= nullptr; }

	return S_OK;
}


//--------------------------------------------------------------------------------------
// Render text.
//--------------------------------------------------------------------------------------
void TextRendererImplementation::Render( ID3D11DeviceContext* pContext, const XMFLOAT4X4& worldMatrix , const XMFLOAT4X4& orthoMatrix, const EditorData& editorData, int numberOfBricks, int bricksRendered, int numberOfStuds, int studsRendered, const PerformanceData& performanceData )
{
	// Update sentence data
	UpdateSentences( pContext, editorData, numberOfBricks, bricksRendered, numberOfStuds, studsRendered, performanceData );

	// set shaders and associated resources

	pContext -> VSSetShader(m_pVertexShader, NULL, 0);
	pContext -> PSSetShader(m_pPixelShader, NULL, 0);

	pContext -> IASetInputLayout(m_pVertexInputLayout);
	
	//ID3D11ShaderResourceView* pTex = m_font.GetTexture();
	pContext -> PSSetShaderResources(0, 1, &pTex );

	pContext -> PSSetSamplers(0, 1, &m_pFontSamplerState);

	// Set the constant buffer for the vertex shader

	// Calculate World-View-Projection Matrix
	XMStoreFloat4x4( &m_cbPerFrame.m_WVP, XMLoadFloat4x4(&worldMatrix) * XMLoadFloat4x4(&m_baseViewMatrix) * XMLoadFloat4x4(&orthoMatrix) );
	//XMStoreFloat4x4( &m_cbPerFrame.m_WVP, XMLoadFloat4x4(&orthoMatrix) );

	D3D11_MAPPED_SUBRESOURCE mappedResource;
	pContext->Map(m_pCbPerFrameBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	memcpy(mappedResource.pData, &m_cbPerFrame, sizeof(ConstBufferPerFrame));
	pContext->Unmap(m_pCbPerFrameBuffer, 0);

	pContext -> VSSetConstantBuffers( 0, 1, &m_pCbPerFrameBuffer );

	// Render the sentences
	for( int i = 0; i < NumberOfSentences; ++i )
	{
		// Set constant buffer for pixel shader

		XMStoreFloat4( &m_cbFontColour.m_colour, XMLoadFloat3( &m_sentenceData[i].m_colourRGB ) );
		m_cbFontColour.m_colour.w = 1.0f;

		D3D11_MAPPED_SUBRESOURCE mappedResource;
		pContext->Map(m_pCbFontColourBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
		memcpy(mappedResource.pData, &m_cbFontColour, sizeof(ConstBufferFontColour));
		pContext->Unmap(m_pCbFontColourBuffer, 0);

		pContext -> PSSetConstantBuffers( 0, 1, &m_pCbFontColourBuffer );

		// Render the sentence

		m_sentences[i].Render( pContext );

	}
}