/* 
*  Kevin Meergans, SquadAI, 2014
*  Order.h
*  Contains enums defining possible commands a team AI can issue to its subordinates, the state of an order 
*  and the priority of it. In addition classes for the different types of order are provided.
*/

#ifndef ORDER_H
#define ORDER_H

// Includes
#include <DirectXMath.h>
#include <vector>

using namespace DirectX;

//--------------------------------------------------------------------------------------
// The available orders that can be issued by a team AI.
//--------------------------------------------------------------------------------------
enum OrderType
{
	AttackEnemyOrder,		// Commands an entity to attack an enemy
	MoveToPositionOrder,	// Commands an entity to move to a certain position
	DefendPositionOrder		// Commands an entity to defend a certain position
};

//--------------------------------------------------------------------------------------
// Determines the state of an order.
//--------------------------------------------------------------------------------------
enum OrderState
{
	PendingOrderState,		// The order was issued but was not yet carried out completely
	SucceededOrderState,	// The order was carried out successfully
	FailedOrderState		// The execution of the order failed
};

//--------------------------------------------------------------------------------------
// Determines the priority of an order.
//--------------------------------------------------------------------------------------
enum OrderPriority
{
	LowPriority,			// The order has low priority
	MediumPriority,			// The order has medium priority
	HighPriority			// The order has high priority
};

//--------------------------------------------------------------------------------------
// Encapsulates all the data contained within an order. Abstract base class.
//--------------------------------------------------------------------------------------
class Order
{
public:
	Order(unsigned long entityId, OrderType orderType, OrderPriority priority); 
	virtual ~Order(void) = 0;

	// Data access functions

	unsigned long GetOrderId(void) const;       
	unsigned long GetEntityId(void) const;   
	OrderType     GetOrderType(void) const;   
	OrderPriority GetOrderPriority(void) const;   
	OrderState    GetOrderState(void) const;   

	void SetEntityId(unsigned long entityId);		
	void SetOrderType(OrderType type);		
	void SetOrderPriority(OrderPriority priority);	
	void SetOrderState(OrderState state);	

private:
	static unsigned long s_OrderId; // This id is incremented and assigned to every created order, 0 is an invalid value

	unsigned long m_orderId;        // The id associated to this order
	unsigned long m_entityId;		// The entity that should execute the order
	OrderType     m_orderType;		// Determines the type of order that the entity should carry out
	OrderPriority m_orderPriority;	// The priority associated to this order
	OrderState    m_orderState;		// The current state of the order
};

//--------------------------------------------------------------------------------------
// Encapsulates all the data required for an attack order.
//--------------------------------------------------------------------------------------
class AttackOrder : public Order
{
public:
	AttackOrder(unsigned long entityId, OrderType orderType, OrderPriority priority, unsigned long enemyId, const XMFLOAT2& enemyPosition);
	~AttackOrder(void);

	// Data access functions

	unsigned long   GetEnemyId(void) const;		
	const XMFLOAT2& GetEnemyPosition(void) const;	

	void SetEnemyId(unsigned long enemyId);		
	void SetEnemyPosition(const XMFLOAT2& enemyPosition);

private:
	unsigned long m_enemyId;		// The id of the enemy that should be attacked
	XMFLOAT2      m_enemyPosition;	// The current or last known position of the enemy
};

//--------------------------------------------------------------------------------------
// Encapsulates all the data required for a move order.
//--------------------------------------------------------------------------------------
class MoveOrder : public Order
{
public:
	MoveOrder(unsigned long entityId, OrderType orderType, OrderPriority priority, const XMFLOAT2& targetPosition);
	~MoveOrder(void);

	// Data access functions

	const XMFLOAT2& GetTargetPosition(void) const;
	std::vector<XMFLOAT2>& GetPath(void);

	void SetTargetPosition(const XMFLOAT2& targetPosition);

private:
	XMFLOAT2              m_targetPosition; // The position, to which the entity should move
	std::vector<XMFLOAT2> m_path;			// The path that the entity should follow (optional, can be empty -> entity will choose path itself)
};

//--------------------------------------------------------------------------------------
// Encapsulates all the data required for a defense order.
//--------------------------------------------------------------------------------------
class DefendOrder : Order
{
public:
	DefendOrder(unsigned long entityId, OrderType orderType, OrderPriority priority, const XMFLOAT2& defendPosition);
	~DefendOrder(void);

	// Data access functions
	const XMFLOAT2& GetDefendPosition(void) const;
	void SetDefendPosition(const XMFLOAT2& defendPosition);

private:
	XMFLOAT2 m_defendPosition; // The position to defend
};


#endif // ORDER_H