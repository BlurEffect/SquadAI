/* 
*  Kevin Meergans, SquadAI, 2014
*  TextDataStructures.h
*  Contains data structures used for text rendering.
*/

#ifndef TEXT_DATA_STRUCTURES_H
#define TEXT_DATA_STRUCTURES_H

// Includes
#include <DirectXMath.h>
#include <d3d11.h>

using namespace DirectX;

//--------------------------------------------------------------------------------------
// Describes a character of the font. Assumes that the texture containing the font contains
// all characters in a single row.
//--------------------------------------------------------------------------------------
struct CharacterData
{
	CharacterData(void) : m_leftBound(0.0f), m_rightBound(0.0f), m_size(0){}
	CharacterData(float leftBound, float rightBound, int size) : m_leftBound(leftBound), 
																 m_rightBound(rightBound), 
																 m_size(size)
	{}

	float m_leftBound;	 // Texture U coordinate of the left bound of the part of the texture containing the character
	float m_rightBound;	 // Texture U coordinate of the right bound of the part of the texture containing the character
	int   m_size;		 // Actual width of the character in pixels
};

//--------------------------------------------------------------------------------------
// Describes a vertex used to display text.
//--------------------------------------------------------------------------------------
struct TextVertex
{
	TextVertex(void) : m_position(0.0f, 0.0f, 0.0f), m_texCoord(0.0f, 0.0f){}
	TextVertex(float posX, float posY, float posZ, float texU, float texV) : m_position(posX, posY, posZ),
																			 m_texCoord(texU, texV)
	{}

	XMFLOAT3 m_position; // The position of the vertex
	XMFLOAT2 m_texCoord; // The texture coordinates of the vertex
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

#endif // TEXT_DATA_STRUCTURES_H