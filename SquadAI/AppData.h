/* 
*  Kevin Meergans, SquadAI, 2014
*  AppData.h
*  Contains data structures used by the application class.
*/

#ifndef APP_DATA_H
#define APP_DATA_H

// Includes
#include "EntityData.h"

//--------------------------------------------------------------------------------------
// Describes the current state and properties of the application.
//--------------------------------------------------------------------------------------
struct AppData
{
	AppData(void) : m_windowWidth(0),
					m_windowHeight(0),
					m_cursorGridPosX(0), 
					m_cursorGridPosY(0), 
					m_selectedEntityType(EntityType(0))
	{}

	int        m_windowWidth;		 // The width in pixels of the application window
	int        m_windowHeight;       // The height in pixels of the application window
	int        m_cursorGridPosX;     // The x-coordinate of the grid field that the cursor is placed at
	int        m_cursorGridPosY;     // The y-coordinate of the grid field that the cursor is placed at
	EntityType m_selectedEntityType; // The entity type currently selected for placement in the test environment
};

#endif // APP_DATA_STRUCTURES_H