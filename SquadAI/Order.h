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
	PendingOrderState,      // The order is still being executed
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

	void SetEntityId(unsigned long entityId);		
	void SetOrderType(OrderType type);		
	void SetOrderPriority(OrderPriority priority);	

private:
	static unsigned long s_OrderId; // This id is incremented and assigned to every created order, 0 is an invalid value

	unsigned long m_orderId;        // The id associated to this order
	unsigned long m_entityId;		// The entity that should execute the order
	OrderType     m_orderType;		// Determines the type of order that the entity should carry out
	OrderPriority m_orderPriority;	// The priority associated to this order
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

	const XMFLOAT2&		   GetTargetPosition(void) const;
	std::vector<XMFLOAT2>* GetPath(void);

	void SetTargetPosition(const XMFLOAT2& targetPosition);
	

private:
	XMFLOAT2              m_targetPosition; // The position, to which the entity should move
	std::vector<XMFLOAT2> m_path;			// The path that the entity should follow (optional, can be empty -> entity will choose path itself)
};

//--------------------------------------------------------------------------------------
// Encapsulates all the data required for a defense order.
//--------------------------------------------------------------------------------------
class DefendOrder : public Order
{
public:
	DefendOrder(unsigned long entityId, OrderType orderType, OrderPriority priority, const XMFLOAT2& defendPosition, const XMFLOAT2& viewDirection);
	~DefendOrder(void);

	// Data access functions
	const XMFLOAT2& GetDefendPosition(void) const;
	const XMFLOAT2& GetViewDirection(void) const;
	void SetDefendPosition(const XMFLOAT2& defendPosition);
	void SetViewDirection(const XMFLOAT2& viewDirection);

private:
	XMFLOAT2 m_defendPosition; // The position to defend
	XMFLOAT2 m_viewDirection;  // The direction, into which the entity should look (the direction to observe when on the defense position)
							   // This is optional data, set view direction to (0,0) if not needed
};


#endif // ORDER_H