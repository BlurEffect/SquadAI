/* 
*  Kevin Meergans, SquadAI, 2014
*  Font.cpp
*  Encapsulates a font that is created by loading in a texture and an accompanying data file containing 
*  information on the characters in the texture.
*  The class is set up to read in font data created with "Codehead's Bitmap Font Generator", which is
*  available from here: http://www.codehead.co.uk/cbfg/ 
*/

// Includes
#include "Font.h"

Font::Font(void) : m_pCharacters(nullptr),
				   m_pTexture(nullptr),
				   m_startCharacter(0),
				   m_numberOfCharacters(0),
				   m_textHeight(0),
				   m_distanceBetweenCharacters(1.0f)
{
}

Font::~Font(void)
{
	Cleanup();
}

//--------------------------------------------------------------------------------------
// Initialises the font.
// Param1: The device that should be used to initialise the font.
// Param2: The filename, or path, to the file containing the font data.
// Param3: The filename, or path, to the font texture to be used with this font object.
// Returns true if the font was initialised successfully, false otherwise.
//--------------------------------------------------------------------------------------
bool Font::Initialise(ID3D11Device* pDevice, const std::string& dataFileName, LPCWSTR textureFilename)
{
	// Load the font data and texture
	return(LoadFontData(dataFileName) && LoadFontTexture(pDevice, textureFilename));
}

//--------------------------------------------------------------------------------------
// Release all allocated resources.
//--------------------------------------------------------------------------------------
void Font::Cleanup(void)
{
	if(m_pCharacters)
	{
		delete[] m_pCharacters;
		m_pCharacters = nullptr;
	}

	if(m_pTexture)
	{
		m_pTexture->Release();
		m_pTexture = nullptr;
	}
}

//--------------------------------------------------------------------------------------
// Fills the passed in vertex array with vertices forming quads to be textured with the
// characters of the font.
// Param1: A pointer to the destination, to which the vertex data will be written.
// Param2: A pointer to the destination, to which the index data will be written.
// Param3: The screen X-coordinate, at which the text should be placed.
// Param4: The screen Y-coordinate, at which the text should be placed.
//--------------------------------------------------------------------------------------
void Font::BuildVertexArray(void* outVertices, void* outIndices, const std::string& text, float screenX, float screenY)
{
	TextVertex*    pVertices = reinterpret_cast<TextVertex*>(outVertices);
	unsigned long* pIndices  = reinterpret_cast<unsigned long*>(outIndices);

	// Get the length of the text
	unsigned int numCharacters = text.length();

	int letter = 0;

	// Build the vertex array by creating a quad for every character in the given text
	for(unsigned int i = 0; i < numCharacters; ++i)
	{
		letter = (static_cast<int>(text[i])) - m_startCharacter;

		// Top left vertex
		pVertices[4 * i].m_position = XMFLOAT3(screenX, screenY, 0.0f);  
		pVertices[4 * i].m_texCoord = XMFLOAT2(m_pCharacters[letter].m_leftBound, 0.0f);
		pIndices[6 * i] = 4 * i;

		// Bottom right vertex
		pVertices[4 * i + 1].m_position = XMFLOAT3((screenX + m_pCharacters[letter].m_size), (screenY - m_textHeight), 0.0f); 
		pVertices[4 * i + 1].m_texCoord = XMFLOAT2(m_pCharacters[letter].m_rightBound, 1.0f);
		pIndices[6 * i + 1] = 4 * i + 1;

		// Bottom left vertex
		pVertices[4 * i + 2].m_position = XMFLOAT3(screenX, (screenY - m_textHeight), 0.0f);  
		pVertices[4 * i + 2].m_texCoord = XMFLOAT2(m_pCharacters[letter].m_leftBound, 1.0f);
		pIndices[6 * i + 2] = 4 * i + 2;

		pIndices[6 * i + 3] = 4 * i; // top left vertex

		// Top right vertex
		pVertices[4 * i + 3].m_position = XMFLOAT3(screenX + m_pCharacters[letter].m_size, screenY, 0.0f);  
		pVertices[4 * i + 3].m_texCoord = XMFLOAT2(m_pCharacters[letter].m_rightBound, 0.0f);
		pIndices[6 * i + 4] = 4 * i + 3;

		pIndices[6 * i + 5] = 4 * i + 1; // bottom right vertex

		// Update the screen coordinate for the next letter, add a small gap in between
		screenX = screenX + m_pCharacters[letter].m_size + m_distanceBetweenCharacters;
	}
}

//--------------------------------------------------------------------------------------
// Load the font data from the given file describing properties of the font characters.
// Param1: The filename, or path, of the file containing the font data.
// Returns true if the data was loaded successfully, false otherwise.
//--------------------------------------------------------------------------------------
bool Font::LoadFontData( const std::string& filename )
{
	std::ifstream in;

	// Open the file
	in.open(filename);
	if(in.fail())
	{
		return false;
	}

	char temp = ' ';

	// Information listed at the top of the font file. At the moment only a few are being used.

	std::string fontName("");
	int imageWidth  = 0;
	int imageHeight = 0;
	int cellWidth   = 0;
	int cellHeight  = 0;
	int startChar   = 0;
	int fontHeight  = 0;
	int fontWidth   = 0;

	while(temp != ',')
	{
		in.get(temp);
	}
	in >> imageWidth;

	// Invalid file, prevent division by zero later on
	if(imageWidth == 0)
	{
		in.close();
		return false;
	}
	in.get(temp);
	while(temp != ',')
	{
		in.get(temp);
	}
	in >> imageHeight;
	in.get(temp);
	while(temp != ',')
	{
		in.get(temp);
	}
	in >> cellWidth;

	// Invalid file, prevent division by zero later on
	if(cellWidth == 0)
	{
		in.close();
		return false;
	}
	in.get(temp);
	while(temp != ',')
	{
		in.get(temp);
	}
	in >> cellHeight;
	in.get(temp);
	while(temp != ',')
	{
		in.get(temp);
	}
	in >> startChar;
	in.get(temp);
	while(temp != ',')
	{
		in.get(temp);
	}
	in >> fontName;
	in.get(temp);
	while(temp != ',')
	{
		in.get(temp);
	}
	in >> fontHeight;
	in.get(temp);
	while(temp != ',')
	{
		in.get(temp);
	}
	in >> fontWidth;

	m_startCharacter     = startChar;
	m_numberOfCharacters = imageWidth / cellWidth;
	m_textHeight		 = cellHeight;

	// Create character array
	m_pCharacters = new CharacterData[m_numberOfCharacters];
	if(!m_pCharacters)
	{
		return false;
	}

	in.get(temp);

	// Read until the beginning of the start character, skip the ones before
	int garbage = 0;
	for(unsigned int i = 0; i < m_startCharacter + m_numberOfCharacters; ++i)
	{
		while(temp != ',')
		{
			in.get(temp);
		}

		if(i < m_startCharacter)
		{
			in >> garbage;
		}else
		{
			in >> m_pCharacters[i - m_startCharacter].m_size;

			m_pCharacters[i - m_startCharacter].m_leftBound  = static_cast<float>((i - m_startCharacter) * cellWidth) / imageWidth;
			m_pCharacters[i - m_startCharacter].m_rightBound = static_cast<float>((i - m_startCharacter) * cellWidth + m_pCharacters[i - m_startCharacter].m_size) / imageWidth;
		}
		in.get(temp);
	}

	// Close the file
	in.close();

	return true;
}

//--------------------------------------------------------------------------------------
// Loads the texture containing the font.
// Param1: The filename, or path, of the file containing the font texture.
// Returns true if the texture was loaded successfully, false otherwise.
//--------------------------------------------------------------------------------------
bool Font::LoadFontTexture(ID3D11Device* pDevice, LPCWSTR filename)
{
	// Create the texture
	return SUCCEEDED(D3DX11CreateShaderResourceViewFromFile(pDevice, filename, NULL, NULL, &m_pTexture, NULL));
}

// Data access functions

ID3D11ShaderResourceView* Font::GetTexture()
{
	return m_pTexture;
}