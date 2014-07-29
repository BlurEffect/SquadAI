/* 
*  Kevin Meergans, SquadAI, 2014
*  Sentence.h
*  Encapsulates a piece of text.
*/

// Includes
#include "SentenceDrawable.h"

SentenceDrawable::SentenceDrawable(UINT maxLength, Font* pFont, const std::string& text, int anchorX, int anchorY, const XMFLOAT3& colourRGB)
	: m_maxLength(maxLength),
	  m_pFont(pFont),
	  m_text(text),
	  m_anchorX(anchorX),
	  m_anchorY(anchorY),
	  m_colour(colourRGB.x, colourRGB.y, colourRGB.z, 1.0f),
	  m_doUpdate(false)
{
	m_currentLength = text.length();

	// Make sure the content of the sentence is not too big
	if(m_currentLength > m_maxLength)
	{
		m_currentLength = m_maxLength;
	}
}

SentenceDrawable::~SentenceDrawable(void)
{
}

//--------------------------------------------------------------------------------------
// Initialises the sentence.
// Param1: A pointer to the device that should be used for the creation of this Drawable.
// Returns true if the initialisation succeeded, false otherwise.
//--------------------------------------------------------------------------------------
bool SentenceDrawable::Initialise(ID3D11Device* pDevice)
{
	// Calculate vertex and index counts
	m_vertexCount = 4 * m_maxLength;
	m_indexCount  = 6 * m_maxLength;

	// Create vertex and index buffers for the sentence

	m_vertexBuffer.Initialise(VertexBuffer, pDevice, D3D11_USAGE_DYNAMIC, nullptr, m_vertexCount);
	m_indexBuffer.Initialise(IndexBuffer, pDevice, D3D11_USAGE_DYNAMIC, nullptr, m_indexCount);

	// This ensures that the sentence is updated with the correct data before being rendered the first time
	m_doUpdate = true;

	return true;
}

//--------------------------------------------------------------------------------------
// Update the content and look of the sentence. Includes updating of vertex and index buffers.
// Param1: The device context to use for updating of the sentence.
// Returns true if the sentence was successfully updated, false otherwise.
//--------------------------------------------------------------------------------------
bool SentenceDrawable::Update(ID3D11DeviceContext* pDeviceContext)
{
	// Note: It might be better to store the pointers to this dynamic memory as members of the class
	//       and to reuse them instead of allocating new memory for every update of the sentence.

	// Create the vertex array
	TextVertex* pVertices = new TextVertex[m_vertexCount];
	if(!pVertices)
	{
		return false;
	}

	// Create the index array
	unsigned long* pIndices = new unsigned long[m_indexCount];
	if(!pIndices)
	{
		return false;
	}

	// Build the vertex and index array for the current text
	m_pFont->BuildVertexArray(reinterpret_cast<void*>(pVertices), reinterpret_cast<void*>(pIndices), m_text, static_cast<float>(m_anchorX), static_cast<float>(m_anchorY));

	// Update the vertex and index buffers

	return m_vertexBuffer.Update(pDeviceContext, pVertices, m_currentLength * 4) &&
		   m_indexBuffer.Update(pDeviceContext, pIndices, m_currentLength * 6);
}

//--------------------------------------------------------------------------------------
// Renders the Sentence.
// Param1: The device context to use for rendering.
//--------------------------------------------------------------------------------------
void SentenceDrawable::Draw(ID3D11DeviceContext* pDeviceContext)
{
	if(m_doUpdate)
	{
		Update(pDeviceContext);
		m_doUpdate = false;
	}

	Drawable::DrawPart(pDeviceContext, m_currentLength * 6);
}

//--------------------------------------------------------------------------------------
// Releases resources.
//--------------------------------------------------------------------------------------
void SentenceDrawable::Cleanup(void)
{
	m_pFont = nullptr;
	Drawable::Cleanup();
}


// Data access functions

UINT SentenceDrawable::GetMaxLength(void) const
{
	return m_maxLength;
}

UINT SentenceDrawable::GetCurrentLength(void) const
{
	return m_currentLength;
}

const Font* SentenceDrawable::GetFont(void) const
{
	return m_pFont;
}

const std::string& SentenceDrawable::GetText(void) const
{
	return m_text;
}

int SentenceDrawable::GetAnchorX(void) const
{
	return m_anchorX;
}

int SentenceDrawable::GetAnchorY(void) const
{
	return m_anchorY;
}

const XMFLOAT4&	SentenceDrawable::GetColour(void) const
{
	return m_colour;
}

// Setting the sentence properties to new values will set the update flag,
// which will lead to an update to the vertex and index data of the sentence.

void SentenceDrawable::SetFont(Font* pFont)
{
	if(m_pFont != pFont)
	{
		m_pFont = pFont;
		m_doUpdate = true;
	}
}

void SentenceDrawable::SetText(const std::string& text)
{
	if((m_text != text) && (text.length() <= m_maxLength))
	{
		m_text = text;
		m_currentLength = text.length();
		m_doUpdate = true;
	}
}

void SentenceDrawable::SetAnchorX(int anchorX)
{
	if(m_anchorX != anchorX)
	{
		m_anchorX = anchorX;
		m_doUpdate = true;
	}
}

void SentenceDrawable::SetAnchorY(int anchorX)
{
	if(m_anchorX != anchorX)
	{
		m_anchorX = anchorX;
		m_doUpdate = true;
	}
}

void SentenceDrawable::SetColourRGB(const XMFLOAT3& colourRGB)
{
	if((m_colour.x != colourRGB.x) || (m_colour.x != colourRGB.y) || (m_colour.x != colourRGB.z))
	{
		m_colour.x = colourRGB.x;
		m_colour.y = colourRGB.y;
		m_colour.z = colourRGB.z;
		m_doUpdate = true;
	}
}
