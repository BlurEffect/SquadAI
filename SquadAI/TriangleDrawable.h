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

class TriangleDrawable : public Drawable<Vertex>
{
public:
	TriangleDrawable(float base, float height);
	~TriangleDrawable(void);
	bool Initialise(ID3D11Device* pDevice);

private:
	float m_base;   // The length of the base of the triangle
	float m_height; // The height of the triangle
};

#endif // TRIANGLE_DRAWABLE_H