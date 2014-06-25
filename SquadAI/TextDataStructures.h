/* 
*  Kevin Meergans, Improved Lego Editor, 2014
*  TextDataStructures.h
*  Contains data structures used by the text renderer and font class.
*/

#ifndef TEXT_DATA_STRUCTURES_H
#define TEXT_DATA_STRUCTURES_H

#include <Windows.h>
#include <xnamath.h>
#include <string>
#include <d3d11.h>

//--------------------------------------------------------------------------------------
// Describes a character of the font. Assumes that the texture containing the font contains
// all characters in a single row.
//--------------------------------------------------------------------------------------
struct CharacterData
{
	CharacterData( void ) : m_leftBound( 0.0f ), m_rightBound( 0.0f ), m_size( 0 ){}
	CharacterData( float leftBound, float rightBound, int size ) : m_leftBound( leftBound ), 
																   m_rightBound( rightBound ), 
																   m_size( size )
	{}

	float m_leftBound;		// texture U coordinate of the left bound of the part of the texture containing the character
	float m_rightBound;		// texture U coordinate of the right bound of the part of the texture containing the character
	int   m_size;			// actual size of the character in pixels
};

//--------------------------------------------------------------------------------------
// Describes a vertex used to display text.
//--------------------------------------------------------------------------------------
struct TextVertex
{
	TextVertex( void ) : m_position( 0.0f, 0.0f, 0.0f ), m_texCoord( 0.0f, 0.0f ){}
	TextVertex( float posX, float posY, float posZ, float texU, float texV ) : m_position( posX, posY, posZ ),
																			   m_texCoord( texU, texV )
	{}

	XMFLOAT3 m_position;
	XMFLOAT2 m_texCoord;
};

//--------------------------------------------------------------------------------------
// Determines the colour of rendered text.
//--------------------------------------------------------------------------------------
struct ConstBufferFontColour
{
	XMFLOAT4 m_colour;	
};

//--------------------------------------------------------------------------------------
// Updated once per frame.
//--------------------------------------------------------------------------------------
struct ConstBufferPerFrame
{
	XMFLOAT4X4 m_WVP;
};

//--------------------------------------------------------------------------------------
// Input layout for font vertices.
//--------------------------------------------------------------------------------------
const static D3D11_INPUT_ELEMENT_DESC FontVertexLayout[2] =
{
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0,  0, D3D11_INPUT_PER_VERTEX_DATA,   0 },  
	{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT   ,    0, 12, D3D11_INPUT_PER_VERTEX_DATA,   0 },  
};

//--------------------------------------------------------------------------------------
// Used to distinguish between different sentences(text parts) to be rendered onto the screen.
//--------------------------------------------------------------------------------------
enum SentenceIdentifiers
{
	// Static text
	TxtCursorPosTemplate,
	TxtCursorPosLabel, 
	TxtBrickTypeLabel, 
	TxtColourLabel, 
	TxtRotationLabel, 
	TxtRecModeLabel, 
	TxtDelModeLabel, 
	TxtBrickNumberLabel,
	TxtBricksRenderedLabel,
	TxtStudNumberLabel,
	TxtStudsRenderedLabel,
	TxtShaderLabel,
	TxtFramerateLabel,

	// Dynamic text
	TxtCursorPosX, 
	TxtCursorPosY,
	TxtCursorPosZ,
	TxtBrickType, 
	TxtColour, 
	TxtRotation, 
	TxtRecMode, 
	TxtDelMode, 
	TxtBrickNumber,
	TxtBricksRendered,
	TxtStudNumber,
	TxtStudsRendered,
	TxtShader,
	TxtFramerate,
	NumberOfSentences
};

//--------------------------------------------------------------------------------------
// Describes the content and look of a sentence.
//--------------------------------------------------------------------------------------
struct SentenceData
{
	SentenceData() : m_text( "" ), m_drawX( 0 ), m_drawY( 0 ), m_colourRGB( 0, 0, 0 ){}
	SentenceData( char* text, int drawX, int drawY, const XMFLOAT3& colourRGB ) : m_text( text ), 
																							   m_drawX( drawX ), 
																							   m_drawY( drawY ),
																							   m_colourRGB( colourRGB )
	{}
		
	const char*	 m_text;		// content of the sentence, text to be drawn
	int			 m_drawX;		// pixel x coordinate where to draw the text
	int			 m_drawY;		// pixel y coordinate where to draw the text
	XMFLOAT3	 m_colourRGB;	// the colour of the text
};





#endif