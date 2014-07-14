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

	m_pEnvironment = pTestEnvironment;

	UpdateWeights();

	return true;
}

//--------------------------------------------------------------------------------------
// Update the node traversal weights/costs according to the current properties of the
// test environment.
//--------------------------------------------------------------------------------------
void Pathfinder::UpdateWeights()
{
	m_weights.m_horizontalCost = m_pEnvironment->GetGridSpacing();
	m_weights.m_verticalCost   = m_pEnvironment->GetGridSpacing();
	m_weights.m_diagonalCost   = sqrtf(m_weights.m_horizontalCost * m_weights.m_horizontalCost + m_weights.m_verticalCost * m_weights.m_verticalCost);
}

//--------------------------------------------------------------------------------------
// Calculates a path consisting of 2D coordinates.
// Param1: Determines which algorithm to use for calculation of the path.
// Param2: The heuristic to use to estimate costs from nodes to the target.
// Param3: The start position of the path in world space.
// Param4: The destination position of the path in world space.
// Param5: A vector that will hold the path consisting of a series of 2D coordinates.
// Returns true if the path could be calculated successfully, false if no valid path exists or start
// and destination are identical.
//--------------------------------------------------------------------------------------
bool Pathfinder::CalculatePath(PathfindingAlgorithm algorithm, Heuristic heuristic, const XMFLOAT2& startPosition, const XMFLOAT2& targetPosition, std::vector<XMFLOAT2>& path)
{
	XMFLOAT2 startGridPosition;
	XMFLOAT2 targetGridPosition;

	m_pEnvironment->WorldToGridPosition(startPosition, startGridPosition);
	m_pEnvironment->WorldToGridPosition(targetPosition, targetGridPosition);

	if((startGridPosition.x == targetGridPosition.x) && (startGridPosition.y == targetGridPosition.y))
	{
		// Start and target position are identical.
		return false;
	}

	if((startGridPosition.x < 0) || (targetGridPosition.x < 0) && 
	   (startGridPosition.x >= m_pEnvironment->GetNumberOfGridPartitions()) || (targetGridPosition.x >= m_pEnvironment->GetNumberOfGridPartitions()) &&
	   (startGridPosition.y < 0) || (targetGridPosition.y < 0) && 
	   (startGridPosition.y >= m_pEnvironment->GetNumberOfGridPartitions()) || (targetGridPosition.y >= m_pEnvironment->GetNumberOfGridPartitions()))
	{
		// Start or target position lies outside of the test environment.
		return false;
	}

	switch(algorithm)
	{
	case AStar:
		return CalculatePathAStar(heuristic, startGridPosition, targetGridPosition, path);
		break;
	default:
		return false;
	}

	return true;
}

//--------------------------------------------------------------------------------------
// Calculates a path consisting of 2D coordinates using the A* algorithm.
// Param1: The heuristic to use to estimate costs from nodes to the target.
// Param2: The start position of the path in grid fields.
// Param3: The destination position of the patch in grid fields.
// Param4: A vector that will hold the path consisting of a series of 2D coordinates.
// Returns true if the path could be calculated successfully, false otherwise.
//--------------------------------------------------------------------------------------
bool Pathfinder::CalculatePathAStar(Heuristic heuristic, const XMFLOAT2& startGridPosition, const XMFLOAT2& targetGridPosition, std::vector<XMFLOAT2>& path)
{
	// Reset all data structures used for this algorithm
	m_openList.clear();
	m_closedList.clear();

	// Get the id of the target node
	unsigned long targetId = m_pEnvironment->GetNodes()[static_cast<int>(targetGridPosition.x)][static_cast<int>(targetGridPosition.y)].GetId();

	// Prepare the start node
	m_pEnvironment->GetNodes()[static_cast<int>(startGridPosition.x)][static_cast<int>(startGridPosition.y)].SetParent(nullptr);
	UpdateCosts(heuristic, &m_pEnvironment->GetNodes()[static_cast<int>(startGridPosition.x)][static_cast<int>(startGridPosition.y)], &m_pEnvironment->GetNodes()[static_cast<int>(targetGridPosition.x)][static_cast<int>(targetGridPosition.y)]);

	// Add the start node to the open list
	m_openList.push_back(&m_pEnvironment->GetNodes()[static_cast<int>(startGridPosition.x)][static_cast<int>(startGridPosition.y)]);

	// Tells whether the path was found or not
	bool found = false;

	while(!found && !m_openList.empty())
	{
		// Sort the open list according to the total estimates of the cost to move through each node
		std::sort(m_openList.begin(), m_openList.end(), CompareNodeCosts());

		// Take the first element from the open list (the one with the smallest total estimate)
		Node* pCurrentNode = m_openList[0];
		
		// Move the current node to the closed list
		m_closedList.push_back(pCurrentNode);
		m_openList.erase(m_openList.begin());

		if(pCurrentNode->GetId() == targetId)
		{
			// The target node was added to the closed list -> path found
			found = true;
			break;
		}

		// Process the nodes adjacent to the current node
		for(std::vector<Node*>::const_iterator it = pCurrentNode->GetAdjacentNodes().begin(); it != pCurrentNode->GetAdjacentNodes().end(); ++it)
		{
			// Only consider the node if it is traversable without cutting corners
			if(!(*it)->IsObstacle() && !IsCuttingCorner(pCurrentNode, *it))
			{
				// Check if the node is already placed in the closed list
				std::vector<Node*>::iterator closedIt = std::find_if(m_closedList.begin(), m_closedList.end(), FindNodeWithId((*it)->GetId()));
				if(closedIt == m_closedList.end())
				{
					// Only consider node if it is not already on the closed list
					std::vector<Node*>::iterator openIt = std::find_if(m_openList.begin(), m_openList.end(), FindNodeWithId((*it)->GetId()));
					if(openIt == m_openList.end())
					{
						// If the node is not in the open list, add it to it
						m_openList.push_back(*it);
						// Makt the current node the parent
						(*it)->SetParent(pCurrentNode);
						// Update the costs of using this node to get to the target
						UpdateCosts(heuristic, (*it), &(m_pEnvironment->GetNodes()[static_cast<int>(targetGridPosition.x)][static_cast<int>(targetGridPosition.y)]));
					}else
					{
						// The node is already placed in the open list, check if the new path to it is shorter
						float oldCost = (*it)->GetMovementCost();
						float newCost = pCurrentNode->GetMovementCost() + GetTraversalCost(pCurrentNode, (*it));
						
						if(newCost < oldCost)
						{
							// The new path is shorter, update the node
							(*it)->SetParent(pCurrentNode);
							UpdateCosts(heuristic, (*it), &(m_pEnvironment->GetNodes()[static_cast<int>(targetGridPosition.x)][static_cast<int>(targetGridPosition.y)]));
						}
					}
				}
			}
		}
	}

	if(found)
	{
		// Construct the path
		ConstructPath(&(m_pEnvironment->GetNodes()[static_cast<int>(targetGridPosition.x)][static_cast<int>(targetGridPosition.y)]), path);
	}

	return found;
}

//--------------------------------------------------------------------------------------
// Builds the path from the given target node back to the start node.
// Param1: The destination node of the path.
// Param2: The vector that will hold the completed path.
//--------------------------------------------------------------------------------------
void Pathfinder::ConstructPath(Node* pTargetNode, std::vector<XMFLOAT2>& path)
{
	// Note: Consider using another container for the path, maybe list or queue.

	std::vector<XMFLOAT2> tempPath;

	const Node* pCurrentNode = pTargetNode;

	while(pCurrentNode != nullptr)
	{
		tempPath.push_back(pCurrentNode->GetWorldPosition());
		pCurrentNode = pCurrentNode->GetParent();
	}

	// Make sure the path is initially empty
	path.clear();

	// Change the order of the path

	for(std::vector<XMFLOAT2>::reverse_iterator it = tempPath.rbegin(); it != tempPath.rend(); ++it)
	{
		path.push_back(*it);
	}
}

//--------------------------------------------------------------------------------------
// Updates the cost to move to the provided node as well as the estimated cost from
// the node to the target.
// Param1: The heuristic to use to estimate costs from nodes to the target.
// Param2: The node that should be updated.
// Param3: The target node.
//--------------------------------------------------------------------------------------
void Pathfinder::UpdateCosts(Heuristic heuristic, Node* pStartNode, Node* pTargetNode)
{
	if(pStartNode->GetParent() == nullptr)
	{
		// Start node
		pStartNode->SetMovementCost(0.0f);
	}else
	{
		// Take parent node's cost into account and add the cost to move to this node from the parent
		pStartNode->SetMovementCost(pStartNode->GetParent()->GetMovementCost() + GetTraversalCost(pStartNode->GetParent(), pStartNode));
	}

	// Note: It would be sufficient to calculate the heuristic estimate once for each node.

	// Calculate the heuristic estimate

	float heuristicValue = 0.0f;
	switch(heuristic)
	{
	case EuclideanDistance:
		heuristicValue = CalculateEuclideanDistance(pStartNode->GetWorldPosition(), pTargetNode->GetWorldPosition());
		break;
	}

	pStartNode->SetHeurisitcValue(heuristicValue);
}

//--------------------------------------------------------------------------------------
// Calculates the cost asociated to moving from a node to the specified adjacent node.
// Does not calculate the cost between arbitrary nodes, only between adjacent ones.
// Param1: The start node.
// Param2: The target node.
//--------------------------------------------------------------------------------------
float Pathfinder::GetTraversalCost(const Node* pStartNode, const Node* pTargetNode)
{
	float movementCost = 0.0f;

	// Check where the node is placed in relation to its parent
	if((pTargetNode->GetGridPosition().x != pStartNode->GetGridPosition().x) &&
		(pTargetNode->GetGridPosition().y != pStartNode->GetGridPosition().y))
	{
		// Diagonal
		movementCost = m_weights.m_diagonalCost;
	}else if(pTargetNode->GetGridPosition().x != pStartNode->GetGridPosition().x)
	{
		// Vertical
		movementCost = m_weights.m_verticalCost;
	}else
	{
		// Horizontal
		movementCost = m_weights.m_horizontalCost;
	}

	return movementCost;
}

//--------------------------------------------------------------------------------------
// Calculates the euclidean distance between a start and target position.
// Param1: The start position in world space.
// Param2: The target position in world space.
// Returns the Euclidean distance in world space.
//--------------------------------------------------------------------------------------
float Pathfinder::CalculateEuclideanDistance(const XMFLOAT2& startPosition, const XMFLOAT2& targetPosition)
{
	// Get the vector from the start to the target position
	XMVECTOR targetVector = XMLoadFloat2(&targetPosition) - XMLoadFloat2(&startPosition);

	// Calculate the magnitude of the vector
	float magnitude = 0.0f;
	XMStoreFloat(&magnitude, XMVector2Length(targetVector));

	return magnitude;
}

//--------------------------------------------------------------------------------------
// Checks for a pair of adjacent nodes whether an entity would cut the corner of an 
// obstacle when moving from one node to the other.
// Param1: The start node.
// Param2: The adjacent target node.
// Returns true if the path from the start to the target node would cut a corner.
//--------------------------------------------------------------------------------------
bool Pathfinder::IsCuttingCorner(Node* pStartNode, Node* pTargetNode)
{
	// The corner of an obstacle will only be cut if the nodes are placed diagonally, check that first
	if((pStartNode->GetGridPosition().x != pTargetNode->GetGridPosition().x) && (pStartNode->GetGridPosition().y != pTargetNode->GetGridPosition().y))
	{
		// Determine the difference in grid fields between the two nodes
		int x = static_cast<int>(pTargetNode->GetGridPosition().x - pStartNode->GetGridPosition().x);
		int y = static_cast<int>(pTargetNode->GetGridPosition().y - pStartNode->GetGridPosition().y);

		if((m_pEnvironment->GetNodes()[static_cast<int>(pStartNode->GetGridPosition().x) + x][static_cast<int>(pStartNode->GetGridPosition().y)].IsObstacle()) ||
		   (m_pEnvironment->GetNodes()[static_cast<int>(pStartNode->GetGridPosition().x)][static_cast<int>(pStartNode->GetGridPosition().y) + y].IsObstacle()))
		{
			return true;
		}
	}

	return false;
}

// Data access functions

float Pathfinder::GetWeightHorizontal(void) const
{
	return m_weights.m_horizontalCost;
}

float Pathfinder::GetWeightVertical(void) const
{
	return m_weights.m_verticalCost;
}

float Pathfinder::GetWeightDiagonal(void) const
{
	return m_weights.m_diagonalCost;
}

void Pathfinder::SetWeightHorizontal(float weight)
{
	m_weights.m_horizontalCost = weight;
}

void Pathfinder::SetWeightVertical(float weight)
{
	m_weights.m_verticalCost = weight;
}

void Pathfinder::SetWeightDiagonal(float weight)
{
	m_weights.m_diagonalCost = weight;
}
