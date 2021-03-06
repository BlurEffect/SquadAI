/* 
*  Kevin Meergans, SquadAI, 2014
*  Node.cpp
*  Represents a node of the graph associated to the test environment in simulation mode,
*  required for pathfinding and other systems.
*/ 

// Includes
#include "Node.h"

Node::Node() : m_nodeId(0),
			   m_pParentNode(nullptr),
			   m_gridPos(0.0f, 0.0f),
			   m_worldPos(0.0f, 0.0f),
			   m_isObstacle(false),
			   m_pObstacle(nullptr),
			   m_territoryOwner(EntityTeam(None)),
			   m_isEntranceToBase(false),
			   m_isAttackPosition(None),
			   m_movementCost(0.0f),
			   m_heuristicValue(0.0f)
{
	for(unsigned int i = 0; i < NumberOfDirections; ++i)
	{
		m_coverProvided[i] = false;
	}
}

Node::~Node(void)
{
}

//--------------------------------------------------------------------------------------
// Update the text on the screen.
// Param1: The current application data.
//--------------------------------------------------------------------------------------
void Node::Initialise(unsigned long id, const XMFLOAT2& gridPos, const XMFLOAT2& worldPos, bool isObstacle)
{
	m_nodeId = id;
	m_gridPos = gridPos;
	m_worldPos = worldPos;
	m_isObstacle = isObstacle;
}

//--------------------------------------------------------------------------------------
// Adds a node to this node's adjacent ones.
// Param1: A pointer to the node to add to this node's adjacent ones.
// Returns true if the node was added to the list of adjacent nodes, false if the passed in
// node was invalid or already contained in the list.
//--------------------------------------------------------------------------------------
bool Node::AddAdjacentNode(Node* pNode)
{
	if(pNode == nullptr)
	{
		// Invalid pointer
		return false;
	}

	for(std::vector<Node*>::iterator it = m_adjacentNodes.begin(); it != m_adjacentNodes.end(); ++it)
	{
		if((*it)->m_nodeId == pNode->m_nodeId)
		{
			// Node is already contained as an adjacent one
			return false;
		}
	}

	m_adjacentNodes.push_back(pNode);

	return true;
}

//--------------------------------------------------------------------------------------
// Resets the node.
//--------------------------------------------------------------------------------------
void Node::Reset(void)
{
	m_isObstacle       = false;
	m_pObstacle        = nullptr;
	m_isEntranceToBase = false;
	m_isAttackPosition = None;
	m_territoryOwner   = None;

	for(unsigned int i = 0; i < NumberOfDirections; ++i)
	{
		m_coverProvided[i] = false;
	}
}

// Data access functions

unsigned long Node::GetId(void) const
{
	return m_nodeId;
}

const Node* Node::GetParent(void) const
{
	return m_pParentNode;
}

const XMFLOAT2& Node::GetGridPosition(void) const
{
	return m_gridPos;
}

const XMFLOAT2& Node::GetWorldPosition(void) const
{
	return m_worldPos;
}

bool Node::IsObstacle(void) const
{
	return m_isObstacle;
}

CollidableObject* Node::GetObstacle(void) 
{
	return m_pObstacle;
}

bool Node::IsCovered(Direction direction) const
{
	return m_coverProvided[direction];
}

EntityTeam Node::GetTerritoryOwner(void) const
{
	return m_territoryOwner;
}

EntityTeam Node::GetAttackPosition(void) const
{
	return m_isAttackPosition;
}

bool Node::IsEntranceToBase(void) const
{
	return m_isEntranceToBase;
}

const std::vector<Node*>& Node::GetAdjacentNodes(void) const
{
	return m_adjacentNodes;
}

float Node::GetMovementCost(void) const
{
	return m_movementCost;
}
	
float Node::GetHeurisitcValue(void) const
{
	return m_heuristicValue;
}

float Node::GetTotalEstimate(void) const
{
	return m_movementCost + m_heuristicValue;
}

void Node::SetId(unsigned long id)
{
	m_nodeId = id;
}

void Node::SetParent(Node* node)
{
	m_pParentNode = node;
}

void Node::SetGridPosition(const XMFLOAT2& gridPos)
{
	m_gridPos = gridPos;
}

void Node::SetWorldPosition(const XMFLOAT2& worldPos)
{
	m_worldPos = worldPos;
}

void Node::SetObstacle(CollidableObject* pObstacle)
{
	m_pObstacle = pObstacle;

	m_isObstacle = (m_pObstacle != nullptr);
}

void Node::SetCovered(Direction direction, bool isCovered)
{
	m_coverProvided[direction] = isCovered;
}

void Node::SetTerritoryOwner(EntityTeam team)
{
	m_territoryOwner = team;
}

void Node::SetEntranceToBase(bool isEntrance)
{
	m_isEntranceToBase = isEntrance;
}

void Node::SetAttackPosition(EntityTeam team)
{
	m_isAttackPosition = team;
}

void Node::SetMovementCost(float cost) 
{
	m_movementCost = cost;
}
	
void Node::SetHeurisitcValue(float heuristicValue) 
{
	m_heuristicValue = heuristicValue;
}