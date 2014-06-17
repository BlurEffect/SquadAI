/* 
*  Kevin Meergans, SquadAI, 2014
*  TriangleDrawable.h
*  Drawable that encapsulates a triangle-shaped mesh.
*/

#ifndef TRIANGLE_DRAWABLE_H
#define TRIANGLE_DRAWABLE_H

// Includes
#include <d3d11.h>
#include "Drawable.h"

class TriangleDrawable : public Drawable
{
public:
	TriangleDrawable(void);
	~TriangleDrawable(void);
	bool Initialise(ID3D11Device* pDevice);
private:

};

#endif // TRIANGLE_DRAWABLE_H