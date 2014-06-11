/* 
*  Kevin Meergans, SquadAI, 2014
*  Circle.h
*  Drawable that encapsulates a circle-shaped mesh.
*/

#ifndef CIRCLE_H
#define CIRCLE_H

// Includes
#include <d3d11.h>
#include "Drawable.h"

class Circle : public Drawable
{
public:
	Circle(void);
	~Circle(void);
	bool Initialise(ID3D11Device* pDevice);
private:

};

#endif // CIRCLE_H