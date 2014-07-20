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
	GroupTeamA,
	GroupTeamB,
	GroupAllSoldiers,
	GroupObstacles,
	GroupTeamAAndObstacles,
	GroupTeamBAndObstacles,
	GroupAllSoldiersAndObstacles
};

//--------------------------------------------------------------------------------------
// Holds the information associated to a grid field in edit mode.
//--------------------------------------------------------------------------------------
struct GridField
{
	GridField(void) : m_pEntity(nullptr),
					  m_isEmpty(true),
					  m_id(0),
					  m_type(EntityType(0)),
					  m_rotation(0.0f)
	{}

	Entity*       m_pEntity;  // A pointer to the entity originally placed on this field
	bool          m_isEmpty;  // Tells whether the grid field was originally occupied by an entity.
	unsigned long m_id;       // The ID of the entity placed on this field.
	EntityType    m_type;     // The type of the entity originally placed on this field.
	float         m_rotation; // The original rotation of the entity.
};


#endif // TEST_ENVIRONMENT_DATA_H