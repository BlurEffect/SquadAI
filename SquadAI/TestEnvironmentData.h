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
// Holds the information associated to a grid field.
//--------------------------------------------------------------------------------------
struct GridField
{
	GridField(void) : m_id(0),
					  m_type(EntityType(0)) 
	{}

	unsigned long m_id;   // The id of the object associated associated to this grid field, 0 if the field is empty
	EntityType    m_type; // The type of the object associated to this grid field
};

#endif // TEST_ENVIRONMENT_DATA_H