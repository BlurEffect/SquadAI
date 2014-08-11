/* 
*  Kevin Meergans, SquadAI, 2014
*  TestEnvironmentData.h
*  Contains data structures used by the test environment class
*/

#ifndef TEST_ENVIRONMENT_DATA_H
#define TEST_ENVIRONMENT_DATA_H

// Includes
#include "Entity.h"

//--------------------------------------------------------------------------------------
// Holds a value for every direction in the test environment.
//--------------------------------------------------------------------------------------
enum Direction
{
	North,
	NorthEast,
	East,
	SouthEast,
	South,
	SouthWest,
	West,
	NorthWest,
	NumberOfDirections
};

//--------------------------------------------------------------------------------------
// Used to distinguish between different groups of entities.
//--------------------------------------------------------------------------------------
enum EntityGroup
{
	GroupTeamRed,					// The group of all living soldiers of team red
	GroupTeamBlue,					// The group of all living soldiers of team blue
	GroupAllSoldiers,				// The group of all living soldiers
	GroupObstacles,					// The group of all obstacles/cover positions
	GroupTeamRedAndObstacles,		// The group of all living soldiers of team A and all obstacles
	GroupTeamBlueAndObstacles,		// The group of all living soldiers of team B and all obstacles
	GroupAllSoldiersAndObstacles,	// The group of all living soldiers and all obstacles
	GroupObjectivesTeamRed,			// The group of all objectives associated to team red
	GroupObjectivesTeamBlue,        // The group of all objectives associated to team blue
	GroupAllObjectives              // The group of all objectives
};

//--------------------------------------------------------------------------------------
// Holds the information associated to a grid field in edit mode.
//--------------------------------------------------------------------------------------
struct GridField
{
	GridField(void) : m_pEntity(nullptr),
					  m_isEmpty(true),
					  m_id(0),
					  m_type(ObjectType(0)),
					  m_rotation(0.0f)
	{}

	Entity*       m_pEntity;  // A pointer to the entity originally placed on this field
	bool          m_isEmpty;  // Tells whether the grid field was originally occupied by an entity.
	unsigned long m_id;       // The ID of the entity placed on this field.
	ObjectType    m_type;     // The type of the entity originally placed on this field.
	float         m_rotation; // The original rotation of the entity.
};

#endif // TEST_ENVIRONMENT_DATA_H