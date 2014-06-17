/* 
*  Kevin Meergans, SquadAI, 2014
*  GridDrawable.h
*  Drawable that encapsulates a grid made up of single lines.
*/

// Note: This grid Drawable is pretty limited at the moment with the constants in
//       the included "GridSetting.h" determining the properties.

#ifndef GRID_DRAWABLE_H
#define GRID_DRAWABLE_H

// Includes
#include <d3d11.h>
#include "Drawable.h"
#include "GridSettings.h"

class GridDrawable : public Drawable
{
public:
	GridDrawable(void);
	~GridDrawable(void);
	bool Initialise(ID3D11Device* pDevice);
private:

};

#endif // GRID_DRAWABLE_H