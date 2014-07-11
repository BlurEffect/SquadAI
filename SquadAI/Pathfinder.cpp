/* 
*  Kevin Meergans, SquadAI, 2014
*  Pathfinder.cpp
*  The pathfinder class contains functions to calculate paths within a test environment.
*/

// Includes
#include "Pathfinder.h"
#include "TestEnvironment.h"

Pathfinder::Pathfinder(void)
{
	
}

Pathfinder::~Pathfinder(void)
{

}

//--------------------------------------------------------------------------------------
// Initialises the pathfinder and binds it to a test environment.
// Param1: A pointer to the test environment that the pathfinder should work with.
// Returns true if the pathfinder could be initialised successfully, false otherwise.
//--------------------------------------------------------------------------------------
bool Pathfinder::Initialise(TestEnvironment* pTestEnvironment)
{
	if(!pTestEnvironment)
	{
		return false;
	}

	m_weights.m_horizontalCost = pTestEnvironment->GetData().m_gridWidth / pTestEnvironment->GetData().m_gridHorizontalPartitions;
	m_weights.m_verticalCost   = pTestEnvironment->GetData().m_gridHeight / pTestEnvironment->GetData().m_gridVerticalPartitions;
	m_weights.m_diagonalCost   = sqrtf(m_weights.m_horizontalCost * m_weights.m_horizontalCost + m_weights.m_verticalCost * m_weights.m_verticalCost);

	return true;
}

//--------------------------------------------------------------------------------------
// Calculates a path consisting of 2D coordinates.
// Param1: Determines which algorithm to use for calculation of the path.
// Param2: The start position of the path in grid fields.
// Param3: The destination position of the patch in grid fields.
// Param4: A vector that will hold the path consisting of a series of 2D coordinates.
// Returns true if the path could be calculated successfully, false if no valid path exists or start
// and destination are identical.
//--------------------------------------------------------------------------------------
bool Pathfinder::CalculatePath(PathfindingAlgorithm algorithm, const XMFLOAT2& startGridPosition, const XMFLOAT2& targetGridPosition, std::vector<XMFLOAT2>& path)
{
	if((startGridPosition.x == targetGridPosition.x) && (startGridPosition.y == targetGridPosition.y))
	{
		// Start and target position are identical.
		return false;
	}

	if((startGridPosition.x < 0) || (targetGridPosition.x < 0) && 
	   (startGridPosition.x >= m_pEnvironment->GetData().m_gridHorizontalPartitions) || (targetGridPosition.x >= m_pEnvironment->GetData().m_gridHorizontalPartitions) &&
	   (startGridPosition.y < 0) || (targetGridPosition.y < 0) && 
	   (startGridPosition.y >= m_pEnvironment->GetData().m_gridVerticalPartitions) || (targetGridPosition.y >= m_pEnvironment->GetData().m_gridVerticalPartitions))
	{
		// Start or target position lies outside of the test environment.
		return false;
	}

	switch(algorithm)
	{
	case AStar:
		return CalculatePathAStar(startGridPosition, targetGridPosition, path);
		break;
	default:
		return false;
	}

	return true;
}

//--------------------------------------------------------------------------------------
// Calculates a path consisting of 2D coordinates using the A* algorithm.
// Param1: The start position of the path in grid fields.
// Param2: The destination position of the patch in grid fields.
// Param3: A vector that will hold the path consisting of a series of 2D coordinates.
// Returns true if the path could be calculated successfully, false otherwise.
//--------------------------------------------------------------------------------------
bool Pathfinder::CalculatePathAStar(const XMFLOAT2& startGridPosition, const XMFLOAT2& targetGridPosition, std::vector<XMFLOAT2>& path)
{

	return true;
}