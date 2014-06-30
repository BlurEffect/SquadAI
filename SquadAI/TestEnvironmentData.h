/* 
*  Kevin Meergans, SquadAI, 2014
*  TestEnvironmentData.h
*  Contains data structures used by the test environment class
*/

#ifndef TEST_ENVIRONMENT_DATA_H
#define TEST_ENVIRONMENT_DATA_H

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

	float m_gridWidth;
	float m_gridHeight; 
	int   m_gridHorizontalPartitions; 
	int   m_gridVerticalPartitions;
};

#endif // TEST_ENVIRONMENT_DATA_H