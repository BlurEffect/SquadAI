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
#include "CollidableObject.h"

using namespace DirectX;


class Node
{
public:
	Node(void);
	~Node(void);

	void Initialise(unsigned long id, const XMFLOAT2& gridPos, const XMFLOAT2& worldPos, bool isObstacle);
	bool AddAdjacentNode(Node* pNode);

	void Reset(void);

	// Data access functions
	unsigned long			  GetId(void) const;
	const Node*				  GetParent(void) const;
	const XMFLOAT2&			  GetGridPosition(void) const;
	const XMFLOAT2&			  GetWorldPosition(void) const;
	bool					  IsObstacle(void) const;
	CollidableObject*		  GetObstacle(void);
	bool					  IsCovered(Direction direction) const;
	EntityTeam				  GetTerritoryOwner(void) const;
	EntityTeam                GetAttackPosition(void) const;
	bool  		              IsEntranceToBase(void) const;
	const std::vector<Node*>& GetAdjacentNodes(void) const;
	float					  GetMovementCost(void) const;
	float					  GetHeurisitcValue(void) const;
	float					  GetTotalEstimate(void) const;

	void SetId(unsigned long id);
	void SetParent(Node* node);
	void SetGridPosition(const XMFLOAT2& gridPos);
	void SetWorldPosition(const XMFLOAT2& worldPos);
	void SetObstacle(CollidableObject* pObstacle);
	void SetCovered(Direction direction, bool isCovered);
	void SetTerritoryOwner(EntityTeam team);
	void SetEntranceToBase(bool isEntrance);
	void SetAttackPosition(EntityTeam team);
	void SetMovementCost(float cost);
	void SetHeurisitcValue(float heuristicValue);

private:
	unsigned long	  m_nodeId;							   // Each node has a unique identifier associated to it for quick comparsion
	Node*			  m_pParentNode;                       // The current parent node to this one
	XMFLOAT2		  m_gridPos;					       // The position of the node in grid space and grid units
	XMFLOAT2		  m_worldPos;						   // The position of the node in world space
	bool			  m_isObstacle;					       // Tells whether this node is an obstacle or can be traversed
	CollidableObject* m_pObstacle;					       // The obstacle placed on this node, null if there is no obstacle
	bool		      m_coverProvided[NumberOfDirections]; // Tells whether this node is covered from some directions or is all in the open
	EntityTeam        m_territoryOwner;					   // Tells whether this node is part of the base of a team
	bool              m_isEntranceToBase;				   // Tells whether the node is an entrance node into a team base
	EntityTeam        m_isAttackPosition;			       // Tells for which team this node is an attack position.

	// Needed for pathfinding
	std::vector<Node*> m_adjacentNodes;				   // The nodes in the graph that can be directly reached from this one
	float			   m_movementCost;				   // The cost of a path from the start position to this node
	float			   m_heuristicValue;			   // A heuristic value used as an estimate of the cost from this node to the target
};

#endif // NODE_H