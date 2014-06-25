/* 
*  Kevin Meergans, SquadAI, 2014
*  TextRenderer.h
*  The class is responsible for rendering text to the screen.
*/

#ifndef TEXT_RENDERER_H
#define TEXT_RENDERER_H

// Includes
#include <d3d11.h>
#include <DirectXMath.h>

#include "TextDataStructures.h"
#include "Font.h"
#include <vector>
#include "Sentence.h"
#include "EditorDataStructures.h"
#include "PerformanceTimerDataStructures.h"

// shaders
#include "CompiledFontVertexShader.h"
#include "CompiledFontPixelShader.h"

using namespace DirectX;

class TextRendererImplementation
{
public:
	TextRendererImplementation();
	HRESULT Initialise( ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, HWND hwnd, int screenWidth, int screenHeight, int maxSentenceLength, const XMFLOAT4X4& baseViewMatrix );
	HRESULT Cleanup();
	void	Render( ID3D11DeviceContext* pContext, const XMFLOAT4X4& worldMatrix , const XMFLOAT4X4& orthoMatrix, const EditorData& editorData, int numberOfBricks, int bricksRendered, int numberOfStuds, int studsRendered, const PerformanceData& performanceData );
private:
	HRESULT InitialiseSentences( ID3D11Device* pDevice, ID3D11DeviceContext* pContext, int maxLength );
	void    UpdateSentences( ID3D11DeviceContext* pDeviceContext, const EditorData& editorData, int numberOfBricks, int bricksRendered, int numberOfStuds, int studsRendered, const PerformanceData& performanceData );

	ID3D11VertexShader*		m_pVertexShader;
	ID3D11PixelShader*		m_pPixelShader;
	ID3D11InputLayout*		m_pVertexInputLayout;
	ID3D11Buffer*			m_pCbFontColourBuffer;
	ID3D11Buffer*			m_pCbPerFrameBuffer;
	ID3D11SamplerState*     m_pFontSamplerState;

	ConstBufferFontColour	m_cbFontColour;
	ConstBufferPerFrame		m_cbPerFrame;

	int						m_screenWidth;
	int						m_screenHeight;
	XMFLOAT4X4				m_baseViewMatrix;
	Font					m_font;
	Sentence				m_sentences[NumberOfSentences];
	SentenceData			m_sentenceData[NumberOfSentences];

	ID3D11ShaderResourceView* pTex;
};

#endif // TEXT_RENDERER_H
