/* 
*  Kevin Meergans, SquadAI, 2014
*  SquareDrawable.h
*  Drawable that encapsulates a square-shaped mesh.
*/

#ifndef SQUARE_DRAWABLE_H
#define SQUARE_DRAWABLE_H

// Includes
#include <d3d11.h>
#include "Drawable.h"

class SquareDrawable : public Drawable
{
public:
	SquareDrawable(float length);
	~SquareDrawable(void);
	bool Initialise(ID3D11Device* pDevice);

private:
	float m_length; // The length of the sides of the square
};

#endif // SQUARE_DRAWABLE_H