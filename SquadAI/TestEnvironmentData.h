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
// Describes the current state and properties of the test environment.
//--------------------------------------------------------------------------------------
struct TestEnvironmentData
{
	TestEnvironmentData(void) : m_gridWidth(0.0f),
								m_gridHeight(0.0f),
								m_gridHorizontalPartitions(0),
								m_gridVerticalPartitions(0)
	{}

	float        m_gridWidth;
	float        m_gridHeight; 
	unsigned int m_gridHorizontalPartitions; 
	unsigned int m_gridVerticalPartitions;
};

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
// Represents a node of the graph underlying the grid and stores information
// required for pathfinding and other systems.
//--------------------------------------------------------------------------------------
struct NodeData
{
	// integrate this into grid field?
	NodeData(void) : m_worldPos(0.0f, 0.0f),
					 m_gridPos(0.0f, 0.0f),
					 m_isObstacle(false)
	{
		for(unsigned int i = 0; i < NumberOfDirections; ++i)
		{
			m_coverProvided[i] = false;
		}
	}
	// needs node ID to check whether already contained in lists (just calc array pos)
	XMFLOAT2 m_worldPos;						  // The position of the node in world space
	XMFLOAT2 m_gridPos;						      // The position of the node in grid space and grid units
	bool     m_isObstacle;					      // Tells whether this node is an obstacle or can be passed
	bool     m_coverProvided[NumberOfDirections]; // Tells whether this node is covered from some directions or is all in the open
};

//--------------------------------------------------------------------------------------
// Holds the data required for a grid field while in edit mode. This is also needed
// to reset the grid to its initial state after running a simulation.
//--------------------------------------------------------------------------------------
struct GridEditData
{
	GridEditData(void) : m_isEmpty(true),
						 m_id(0),
					     m_type(EntityType(0)),
						 m_rotation(0.0f)
	{}

	bool          m_isEmpty;  // Tells whether the grid field was originally occupied by an entity.
	unsigned long m_id;       // The ID of the entity placed on this field.
	EntityType    m_type;     // The type of the entity originally placed on this field.
	float         m_rotation; // The original rotation of the entity.
};

//--------------------------------------------------------------------------------------
// Holds the information associated to a grid field.
//--------------------------------------------------------------------------------------
struct GridField
{
	GridField(void){}

	GridEditData m_editData; // Holds data for the grid field required in edit mode.
	NodeData     m_nodeData; // The data of the node associated to this grid field.
};


#endif // TEST_ENVIRONMENT_DATA_H