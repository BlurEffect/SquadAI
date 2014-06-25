/* 
*  Kevin Meergans, Improved Lego Editor, 2014
*  SentenceImplementation.h
*  Implementation part of the Sentence class.The public interface of this class
*  and the one of the Sentence class have to be kept consistent.
*/

#ifndef SENTENCE_IMPLEMENTATION_H
#define SENTENCE_IMPLEMENTATION_H

#include <windows.h>
#include "TextDataStructures.h"
#include <D3D11.h>
#include <xnamath.h>
#include "Font.h"


class SentenceImplementation
{
public:
	SentenceImplementation( void );
	HRESULT Initialise( ID3D11Device* pDevice, int maxLength );
	HRESULT Cleanup( void );
	HRESULT Update( ID3D11DeviceContext* pDeviceContext, Font* pFont, const SentenceData& sentenceData );
	void Render( ID3D11DeviceContext* pDeviceContext );
private:

	ID3D11Buffer* m_pVertexBuffer;
	ID3D11Buffer* m_pIndexBuffer; 
	int			  m_vertexCount;
	int			  m_indexCount;
	int			  m_maxLength;
	XMFLOAT3      m_colourRGB;
};

#endif
