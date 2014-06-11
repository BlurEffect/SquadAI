/* 
*  Kevin Meergans, SquadAI, 2014
*  Triangle.h
*  Drawable that encapsulates a triangle-shaped mesh.
*/

#ifndef TRIANGLE_H
#define TRIANGLE_H

// Includes
#include <d3d11.h>
#include "Drawable.h"

class Triangle : public Drawable
{
public:
	Triangle(void);
	~Triangle(void);
	bool Initialise(ID3D11Device* pDevice);
private:

};

#endif // TRIANGLE_H