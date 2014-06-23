/* 
*  Kevin Meergans, SquadAI, 2014
*  DrawableFactory.cpp
*  Class with a single static member function that serves as a
*  factory function to create new Drawables.
*/

// Includes
#include "DrawableFactory.h"

//--------------------------------------------------------------------------------------
// Factory function to create new Drawables. Dynamically creates new instances of the 
// requested Drawable.
// Param1: The type of the Drawable to create.
// Returns a pointer to the new Drawable. When the creation of the Drawable failed or the passed
// in type identifier is invalid, a null pointer will be returned.
//--------------------------------------------------------------------------------------
Drawable* DrawableFactory::CreateDrawable(DrawableType type)
{
	switch(type)
	{
	case TriangleType:
		return new TriangleDrawable();
		break;
	case SquareType:
		return new SquareDrawable();
		break;
	case CircleType:
		return new CircleDrawable();
		break;
	case GridType:
		return new GridDrawable();
		break;
	}

	return nullptr;
}

