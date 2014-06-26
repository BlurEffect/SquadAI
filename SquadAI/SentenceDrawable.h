/* 
*  Kevin Meergans, SquadAI, 2014
*  Sentence.h
*  Encapsulates a piece of text.
*/

#ifndef SENTENCE_H
#define SENTENCE_H

// Includes
#include <DirectXMath.h>
#include <D3D11.h>
#include "Drawable.h"
#include "TextDataStructures.h"
#include "Font.h"
#include "Buffer.h"

using namespace DirectX;

class SentenceDrawable : Drawable
{
public:
	SentenceDrawable(int maxLength, Font* pFont, const std::string& text, int anchorX, int anchorY, const XMFLOAT3& colourRGB);
	~SentenceDrawable(void);
	bool Initialise(ID3D11Device* pDevice);
	void Draw(ID3D11DeviceContext* pDeviceContext);
	void Cleanup(void);

	// Data access

	const Font*        GetFont(void) const;
	const std::string& GetText(void) const;
	int                GetAnchorX(void) const;
	int                GetAnchorY(void) const;
	const XMFLOAT3&	   GetColourRGB(void) const;

	void SetFont(Font* pFont);
	void SetText(const std::string& text);
	void SetAnchorX(int anchorX);
	void SetAnchorY(int anchorY);
	void SetColourRGB(const XMFLOAT3& colourRGB);

private:
	bool Update(ID3D11DeviceContext* pDeviceContext);

	Buffer<TextVertex>    m_vertexBuffer;  // The buffer holding the vertex data for the sentence
	Buffer<unsigned long> m_indexBuffer;   // The buffer holding the index data for the sentence
	int					  m_vertexCount;   // The number of vertices needed to render the sentence
	int					  m_indexCount;	   // The number of indices needed to render the sentence
	
	int				      m_maxLength;	   // The maximum length of the sentence in characters
	int                   m_currentLength; // The length of the current content of the sentence in characters

	Font*                 m_pFont;         // The font used with this sentence
	std::string		      m_text;		   // Content of the sentence, text to be drawn
	int					  m_anchorX;	   // Pixel x coordinate, at which to start drawing the text
	int					  m_anchorY;	   // Pixel y coordinate, at which to start drawing the text
	XMFLOAT3		      m_colourRGB;	   // The colour, in which to render the content of the sentence

	bool                  m_doUpdate;      // Tells whether the vertex data has to be updated before rendering
};

#endif // SENTENCE_H
