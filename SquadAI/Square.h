/* 
*  Kevin Meergans, SquadAI, 2014
*  Square.h
*  Drawable that encapsulates a square-shaped mesh.
*/

#ifndef SQUARE_H
#define SQUARE_H

// Includes
#include <d3d11.h>
#include "Drawable.h"

class Square : public Drawable
{
public:
	Square(void);
	~Square(void);
	bool Initialise(ID3D11Device* pDevice);
private:

};

#endif // SQUARE_H