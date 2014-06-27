/* 
*  Kevin Meergans, SquadAI, 2014
*  Font.h
*  Encapsulates a font that is created by loading in a texture and an accompanying data file containing 
*  information on the characters in the texture.
*  The class is set up to read in font data created with "Codehead's Bitmap Font Generator", which is
*  available from here: http://www.codehead.co.uk/cbfg/ 
*/

#ifndef FONT_H
#define FONT_H

// Includes
#include <D3D11.h>
#include <D3DX11.h>
#include <fstream>
#include <string>
#include <DirectXMath.h>
#include "TextDataStructures.h"

using namespace DirectX;

class Font
{
public:
	Font(void);
	~Font(void);
	bool Initialise(ID3D11Device* pDevice, const std::string& dataFileName, LPCWSTR textureFilename);
	void Cleanup(void);
	void BuildVertexArray(void* outVertices, void* outIndices, const std::string& text, float screenX, float screenY);

	// Data access
	ID3D11ShaderResourceView* GetTexture();

private:
	bool LoadFontData(const std::string& filename);
	bool LoadFontTexture(ID3D11Device* pDevice, LPCWSTR filename);

	CharacterData*			  m_pCharacters;			   // The character data for the font used by this class to build text
	ID3D11ShaderResourceView* m_pTexture;				   // The texture containing the font
	byte				      m_startCharacter;			   // The first character in the font, usually empty space char
	UINT                      m_numberOfCharacters;		   // The number of characters in the font
	UINT					  m_textHeight;				   // The height of the text
	float                     m_distanceBetweenCharacters; // The distance between single characters within a text
};

#endif // FONT_H

