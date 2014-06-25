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

class GridDrawable : public Drawable
{
public:
	GridDrawable(float width, float height, int horizontalPartitions, int verticalPartitions);
	~GridDrawable(void);
	bool Initialise(ID3D11Device* pDevice);
private:
	float m_width;				  // The width of the grid
	float m_height;				  // The height of the grid
	int   m_horizontalPartitions; // The number of partitions along the x-axis of the grid
	int   m_verticalPartitions;   // The number of partitions along the y-axis of the grid
};

#endif // GRID_DRAWABLE_H