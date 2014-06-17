/* 
*  Kevin Meergans, SquadAI, 2014
*  DrawableFactory.h
*  Class with a single static member function that serves as a
*  factory function to create new Drawables.
*/

#ifndef DRAWABLE_FACTORY_H
#define DRAWABLE_FACTORY_H

// Includes
#include "Drawable.h"
#include "TriangleDrawable.h"
#include "SquareDrawable.h"
#include "CircleDrawable.h"
#include "ObjectRenderData.h"

class DrawableFactory
{
public:
	static Drawable* CreateDrawable(DrawableType type);
};

#endif // DRAWABLE_FACTORY_H
