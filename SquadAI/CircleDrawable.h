/* 
*  Kevin Meergans, SquadAI, 2014
*  CircleDrawable.h
*  Drawable that encapsulates a circle-shaped mesh.
*/

#ifndef CIRCLE_DRAWABLE_H
#define CIRCLE_DRAWABLE_H

// Includes
#include <d3d11.h>
#include "Drawable.h"

class CircleDrawable : public Drawable
{
public:
	CircleDrawable(void);
	~CircleDrawable(void);
	bool Initialise(ID3D11Device* pDevice);
private:

};

#endif // CIRCLE_DRAWABLE_H