/* 
*  Kevin Meergans, SquadAI, 2014
*  Node.h
*  Represents a node of the graph associated to the test environment in simulation mode,
*  required for pathfinding and other systems.
*/

#ifndef NODE_H
#define NODE_H

// Includes
#include <DirectXMath.h>
#include <vector>
#include "TestEnvironmentData.h"

using namespace DirectX;

class Node
{
public:
	Node(void);
	~Node(void);

	void Initialise(unsigned long id, const XMFLOAT2& gridPos, const XMFLOAT2& worldPos, bool isObstacle);

	bool AddAdjacentNode(Node* pNode);

	// Data access functions
	unsigned long   GetId(void) const;
	const Node*     GetParent(void) const;
	const XMFLOAT2& GetGridPosition(void) const;
	const XMFLOAT2& GetWorldPosition(void) const;
	bool			IsObstacle(void) const;
	bool            IsCovered(Direction direction) const;
	const std::vector<Node*>& GetAdjacentNodes(void) const;

	void SetId(unsigned long id);
	void SetParent(Node* node);
	void SetGridPosition(const XMFLOAT2& gridPos);
	void SetWorldPosition(const XMFLOAT2& worldPos);
	void SetObstacle(bool isObstacle);
	void SetCovered(Direction direction, bool isCovered);

private:
	unsigned long m_nodeId;							   // Each node has a unique identifier associated to it for quick comparsion
	Node*		  m_pParentNode;                       // The current parent node to this one
	XMFLOAT2	  m_gridPos;					       // The position of the node in grid space and grid units
	XMFLOAT2	  m_worldPos;						   // The position of the node in world space
	bool		  m_isObstacle;					       // Tells whether this node is an obstacle or can be traversed
	bool		  m_coverProvided[NumberOfDirections]; // Tells whether this node is covered from some directions or is all in the open

	std::vector<Node*> m_adjacentNodes;				   // The nodes in the graph that can be directly reached from this one
};

#endif // NODE_H