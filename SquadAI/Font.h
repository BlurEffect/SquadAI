/* 
*  Kevin Meergans, Improved Lego Editor, 2014
*  FontImplementation.h
*  Implementation part of the Font class.The public interface of this class
*  and the one of the Font class have to be kept consistent.
*/

#ifndef FONT_IMPLEMENTATION_H
#define FONT_IMPLEMENTATION_H

#include <windows.h>
#include "TextDataStructures.h"
#include <D3D11.h>
#include <D3DX11.h>
#include <string>
#include <fstream>
#include <xnamath.h>


class FontImplementation
{
public:
	FontImplementation();
	HRESULT Initialise( ID3D11Device* pDevice, const std::string& dataFileName, LPCWSTR textureFilename );
	HRESULT Cleanup( void );
	void    BuildVertexArray( void* outVertices, const char* text, float screenX, float screenY);
	ID3D11ShaderResourceView* GetTexture() const;
private:
	bool    LoadFontData( const std::string& filename );
	bool    LoadFontTexture( ID3D11Device* pDevice, LPCWSTR filename );

	CharacterData*			  m_pCharacters;		// The character data for the font used by this class to build text
	ID3D11ShaderResourceView* m_pTexture;			// The texture containing the font
	byte				      m_startCharacter;		// The first character in the font, usually empty space char
	int                       m_numberOfCharacters; // The number of characters in the font
	int						  m_textHeight;			// The height of the text
};

#endif

