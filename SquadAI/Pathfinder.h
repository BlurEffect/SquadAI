/* 
*  Kevin Meergans, SquadAI, 2014
*  Pathfinder.h
*  The pathfinder class contains functions to calculate paths within a test environment.
*/

#ifndef PATHFINDER_H
#define PATHFINDER_H

// Includes
#include <DirectXMath.h>
#include <vector>
#include <algorithm>
#include "Node.h"

// Forward Declaration
class TestEnvironment;

using namespace DirectX;

//--------------------------------------------------------------------------------------
// Defines available algorithms to use for path calculation by the pathfinder class.
//--------------------------------------------------------------------------------------
enum PathfindingAlgorithm
{
	AStar // A* pathfinding algorithm
};

//--------------------------------------------------------------------------------------
// Defines available heuristic functions to estimate costs from a node to the target.
//--------------------------------------------------------------------------------------
enum Heuristic
{
	EuclideanDistance // Euclidean distance heuristic, length of the straight line between the node and the target used as estimate
};

//--------------------------------------------------------------------------------------
// Bundles the weights associated to different type of graph node traversal.
//--------------------------------------------------------------------------------------
struct TraversalWeights
{
	TraversalWeights(void) : m_horizontalCost(1.0f),
							 m_verticalCost(1.0f),
							 m_diagonalCost(1.0f)
	{}

	float m_horizontalCost; // The cost associated to the traversal of a graph node in horizontal direction
	float m_verticalCost;   // The cost associated to the traversal of a graph node in vertical direction
	float m_diagonalCost;   // The cost associated to the traversal of a graph node in diagonal direction
};

class Pathfinder
{
public:
	Pathfinder(void);
	~Pathfinder(void);

	bool Initialise(TestEnvironment* pTestEnvironment);
	bool CalculatePath(PathfindingAlgorithm algorithm, Heuristic heuristic, const XMFLOAT2& startPosition, const XMFLOAT2& targetPosition, std::vector<XMFLOAT2>& path);

private:

	bool CalculatePathAStar(Heuristic heuristic, const XMFLOAT2& startGridPosition, const XMFLOAT2& targetGridPosition, std::vector<XMFLOAT2>& path);
	void UpdateCosts(Heuristic heuristic, Node* pStartNode, Node* pTargetNode);
	float GetTraversalCost(const Node* pStartNode, const Node* pTargetNode);
	float CalculateEstimate(Node* pStartNode, Node* pTargetNode);
	float CalculateEuclideanDistance(const XMFLOAT2& startPosition, const XMFLOAT2& targetPosition);
	void ConstructPath(Node* pTargetNode, std::vector<XMFLOAT2>& path);

	TestEnvironment* m_pEnvironment; // A pointer to the test environment this pathfinder belongs to
	TraversalWeights m_weights;      // The weights used to calculate distances in the graph

	std::vector<Node*> m_openList;   // List of open nodes required for A* pathfinding
	std::vector<Node*> m_closedList; // List of closed nodes required for A* pathfinding

	//--------------------------------------------------------------------------------------
	// Private comparator class used to determine which node to choose from the open list.
	// Can be used to sort a container of pointers to nodes according to their respective
	// combined cost (cost so far + heuristic estimate).
	//--------------------------------------------------------------------------------------
	class CompareNodeCosts
	{
	public:
		bool operator()(const Node* pNode1, const Node* pNode2) 
		{
			return pNode1->GetTotalEstimate() < pNode2->GetTotalEstimate();
		}
    };

	//--------------------------------------------------------------------------------------
	// Private comparator class used to find a node with a certain id within a container.
	//--------------------------------------------------------------------------------------
	class FindNodeWithId
	{
	public:
		FindNodeWithId(unsigned long id) : m_id(id){}
		bool operator()(const Node* pNode) 
		{
			return pNode->GetId() == m_id;
		}

	private:
		unsigned long m_id;
    };
};

#endif // PATHFINDER_H