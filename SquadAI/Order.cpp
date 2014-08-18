/* 
*  Kevin Meergans, SquadAI, 2014
*  Order.cpp
*  Contains enums defining possible commands a team AI can issue to its subordinates, the state of an order 
*  and the priority of it. In addition classes for the different types of order are provided.
*/

// Includes
#include "Order.h"

//--------------------------------------------------------------------------------------
// class Order
//--------------------------------------------------------------------------------------

unsigned long Order::s_OrderId = 0;

Order::Order(unsigned long entityId, OrderType orderType, OrderPriority priority) : m_orderId(++s_OrderId),
																					m_entityId(entityId),
																					m_orderType(orderType),
																					m_orderPriority(priority)
{
}

Order::~Order(void)
{
}

// Data access functions

unsigned long Order::GetOrderId(void) const
{
	return m_orderId;
}

unsigned long Order::GetEntityId(void) const
{
	return m_entityId;
}

OrderType Order::GetOrderType(void) const
{
	return m_orderType;
}
   
OrderPriority Order::GetOrderPriority(void) const
{
	return m_orderPriority;
}

void Order::SetEntityId(unsigned long entityId)
{
	m_entityId = entityId;
}
		
void Order::SetOrderType(OrderType type)
{
	m_orderType = type;
}
		
void Order::SetOrderPriority(OrderPriority priority)
{
	m_orderPriority = priority;
}
	
//--------------------------------------------------------------------------------------
// class AttackOrder
//--------------------------------------------------------------------------------------

AttackOrder::AttackOrder(unsigned long entityId, OrderType orderType, OrderPriority priority, unsigned long enemyId, const XMFLOAT2& enemyPosition)
	: Order(entityId, orderType, priority),
	  m_enemyId(enemyId),
	  m_enemyPosition(enemyPosition)
{
}

AttackOrder::~AttackOrder(void)
{
}

// Data access functions

unsigned long AttackOrder::GetEnemyId(void) const
{
	return m_enemyId;
}

const XMFLOAT2& AttackOrder::GetEnemyPosition(void) const
{
	return m_enemyPosition;
}

void AttackOrder::SetEnemyId(unsigned long enemyId)
{
	m_enemyId = enemyId;
}		

void AttackOrder::SetEnemyPosition(const XMFLOAT2& enemyPosition)
{
	m_enemyPosition = enemyPosition;
}

//--------------------------------------------------------------------------------------
// class MoveOrder
//--------------------------------------------------------------------------------------

MoveOrder::MoveOrder(unsigned long entityId, OrderType orderType, OrderPriority priority, const XMFLOAT2& targetPosition)
	: Order(entityId, orderType, priority),
	  m_targetPosition(targetPosition)
{
}

MoveOrder::~MoveOrder(void)
{
}

// Data access functions

const XMFLOAT2& MoveOrder::GetTargetPosition(void) const
{
	return m_targetPosition;
}

std::vector<XMFLOAT2>* MoveOrder::GetPath(void)
{
	return &m_path;
}

void MoveOrder::SetTargetPosition(const XMFLOAT2& targetPosition)
{
	m_targetPosition = targetPosition;
}


//--------------------------------------------------------------------------------------
// class DefendOrder
//--------------------------------------------------------------------------------------

DefendOrder::DefendOrder(unsigned long entityId, OrderType orderType, OrderPriority priority, const XMFLOAT2& defendPosition, const XMFLOAT2& viewDirection)
	: Order(entityId, orderType, priority),
	m_defendPosition(defendPosition),
	m_viewDirection(viewDirection)
{
}

DefendOrder::~DefendOrder(void)
{
}

// Data access functions

const XMFLOAT2& DefendOrder::GetDefendPosition(void) const
{
	return m_defendPosition;
}

const XMFLOAT2& DefendOrder::GetViewDirection(void) const
{
	return m_viewDirection;
}

void DefendOrder::SetDefendPosition(const XMFLOAT2& defendPosition)
{
	m_defendPosition = defendPosition;
}

void DefendOrder::SetViewDirection(const XMFLOAT2& viewDirection)
{
	m_viewDirection = viewDirection;
}
