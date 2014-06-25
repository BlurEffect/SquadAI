/* 
*  Kevin Meergans, Improved Lego Editor, 2014
*  FontImplementation.cpp
*  Contains the function definitions for the FontImplementation class.
*/

#include "FontImplementation.h"

FontImplementation::FontImplementation() : m_pCharacters( nullptr ),
										   m_pTexture( nullptr ),
										   m_startCharacter( 0 ),
										   m_numberOfCharacters( 0 )
{
}

//--------------------------------------------------------------------------------------
// Initialise the font.
//--------------------------------------------------------------------------------------
HRESULT FontImplementation::Initialise( ID3D11Device* pDevice, const std::string& dataFileName, LPCWSTR textureFilename )
{
	// Load the font data and texture

	if( LoadFontData( dataFileName ) &&  LoadFontTexture( pDevice, textureFilename ) )
	{
		return S_OK;
	}else
	{
		return E_FAIL;
	}

}

//--------------------------------------------------------------------------------------
// Release all allocated resources.
//--------------------------------------------------------------------------------------
HRESULT FontImplementation::Cleanup( void )
{
	if( m_pCharacters )
	{
		delete[] m_pCharacters;
		m_pCharacters = nullptr;
	}

	if( m_pTexture )
	{
		m_pTexture -> Release();
		m_pTexture = nullptr;
	}

	return S_OK;
}

//--------------------------------------------------------------------------------------
// Fill the passed in vertex array with vertices "containing" the text to display the text
// described by the second parameter and positioned at the screen coordinates denoted by the 
// last two parameters.
//--------------------------------------------------------------------------------------
void FontImplementation::BuildVertexArray( void* outVertices, const char* text, float screenX, float screenY)
{
	TextVertex* pVertices;

	// Point to the passed in array 
	pVertices = ( TextVertex* )outVertices;

	// Get the length of the text
	int numCharacters = ( int )strlen( text );

	int index  = 0;
	int letter = 0;

	// Build the vertex array by creating a quad for every character in the given text
	for(int i = 0; i < numCharacters; i++)
	{
		letter = ( ( int )text[i] ) - m_startCharacter;

		// Create the first triangle
		pVertices[index].m_position = XMFLOAT3(screenX, screenY, 0.0f);  // Top left.
		pVertices[index].m_texCoord = XMFLOAT2(m_pCharacters[letter].m_leftBound, 0.0f);
		++index;

		pVertices[index].m_position = XMFLOAT3((screenX + m_pCharacters[letter].m_size), (screenY - m_textHeight), 0.0f);  // Bottom right.
		pVertices[index].m_texCoord = XMFLOAT2(m_pCharacters[letter].m_rightBound, 1.0f);
		++index;

		pVertices[index].m_position = XMFLOAT3(screenX, (screenY - m_textHeight), 0.0f);  // Bottom left.
		pVertices[index].m_texCoord = XMFLOAT2(m_pCharacters[letter].m_leftBound, 1.0f);
		++index;

		// Create the second triangle
		pVertices[index].m_position = XMFLOAT3(screenX, screenY, 0.0f);  // Top left.
		pVertices[index].m_texCoord = XMFLOAT2(m_pCharacters[letter].m_leftBound, 0.0f);
		++index;

		pVertices[index].m_position = XMFLOAT3(screenX + m_pCharacters[letter].m_size, screenY, 0.0f);  // Top right.
		pVertices[index].m_texCoord = XMFLOAT2(m_pCharacters[letter].m_rightBound, 0.0f);
		++index;

		pVertices[index].m_position = XMFLOAT3((screenX + m_pCharacters[letter].m_size), (screenY - m_textHeight), 0.0f);  // Bottom right.
		pVertices[index].m_texCoord = XMFLOAT2(m_pCharacters[letter].m_rightBound, 1.0f);
		++index;

		// Update the screen coordinate for the next letter, add a small gap in between
		screenX = screenX + m_pCharacters[letter].m_size + 1.0f;
	}
}

//--------------------------------------------------------------------------------------
// Returns a pointer to the texture containing the font.
//--------------------------------------------------------------------------------------
ID3D11ShaderResourceView* FontImplementation::GetTexture() const
{
	return m_pTexture;
}

//--------------------------------------------------------------------------------------
// Load the font data describing properties of the font characters from the given file.
//--------------------------------------------------------------------------------------
bool FontImplementation::LoadFontData( const std::string& filename )
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

	// invalid file, prevent division by zero later on
	if( imageWidth == 0 )
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
	// invalid file, prevent division by zero later on
	if( cellWidth == 0 )
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
	if( !m_pCharacters )
	{
		return false;
	}

	in.get(temp);
	// read until the beginning of the start character, skip the ones before
	int garbage = 0;
	for( int i = 0; i < m_startCharacter + m_numberOfCharacters; ++i )
	{
		while(temp != ',')
		{
			in.get(temp);
		}

		if( i < m_startCharacter )
		{
			in >> garbage;
		}else
		{
			in >> m_pCharacters[i - m_startCharacter].m_size;

			// calculate the texture coordinates
			//m_pCharacters[i - m_startCharacter].m_leftBound  = static_cast<float>( (i - m_startCharacter) * cellWidth ) / imageWidth;
			//m_pCharacters[i - m_startCharacter].m_rightBound = static_cast<float>( ( (i - m_startCharacter)+1 ) * cellWidth ) / imageWidth;
			
			m_pCharacters[i - m_startCharacter].m_leftBound  = static_cast<float>( (i - m_startCharacter) * cellWidth ) / imageWidth;
			m_pCharacters[i - m_startCharacter].m_rightBound = static_cast<float>( (i - m_startCharacter ) * cellWidth + m_pCharacters[i - m_startCharacter].m_size) / imageWidth;
		
			
		
		}
		in.get(temp);
	}

	/*
	int a = 0;

	in.get(temp);
	// Read in the number of characters contained in the file
	for( int i = 0; i < m_numberOfCharacters; i++ )
	{
		while(temp != ',')
		{
			in.get(temp);
		}
		//in >> m_pCharacters[i].m_size;
		in >> a;

		// calculate the texture coordinates
		m_pCharacters[i].m_leftBound  = static_cast<float>( i * cellWidth ) / imageWidth;
		m_pCharacters[i].m_rightBound = static_cast<float>( ( i+1 ) * cellWidth ) / imageWidth;
	}
	*/
	// Close the file
	in.close();

	return true;
}

//--------------------------------------------------------------------------------------
// Load the texture containing the font.
//--------------------------------------------------------------------------------------
bool FontImplementation::LoadFontTexture( ID3D11Device* pDevice, LPCWSTR filename )
{
	// Create the texture
	HRESULT result = D3DX11CreateShaderResourceViewFromFile( pDevice, filename, NULL, NULL, &m_pTexture, NULL );

	return SUCCEEDED( result );
}