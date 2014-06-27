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
	CircleDrawable(float radius, UINT segments);
	~CircleDrawable(void);
	bool Initialise(ID3D11Device* pDevice);
private:
	float m_radius;   // The radius of the circle
	int   m_segments; // Determines the number of segments for the circle and thus the "roundness" of it
};

#endif // CIRCLE_DRAWABLE_H