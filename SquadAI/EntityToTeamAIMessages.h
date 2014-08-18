/* 
*  Kevin Meergans, SquadAI, 2014
*  EntityToTeamAIMessages.h
*  This file contains all messages that are sent from individual entities to the
*  team AI associated to these entities.
*/

#ifndef ENTITY_TO_TEAM_AI_MESSAGES_H
#define ENTITY_TO_TEAM_AI_MESSAGES_H

// Includes
#include <DirectXMath.h>
#include "Message.h"
#include "Order.h"

using namespace DirectX;

//--------------------------------------------------------------------------------------
// Contains data required for a message sent when an entity has spotted an enemy and wants
// to inform the associated team AI of that event.
//--------------------------------------------------------------------------------------
struct EnemySpottedMessageData
{
	EnemySpottedMessageData(unsigned long spotterId, unsigned long enemyId, const XMFLOAT2& enemyPosition) 
		: m_spotterId(spotterId),
		  m_enemyId(enemyId),
		  m_enemyPosition(enemyPosition)
	{}

	unsigned long m_spotterId;      // The id of the entity that spotted an enemy
	unsigned long m_enemyId;		// The id of the enemy that was spotted by the entity
	XMFLOAT2      m_enemyPosition;	// The position, at which the entity was spotted
};


//--------------------------------------------------------------------------------------
// Message sent when an entity has spotted an enemy and wants
// to inform the associated team AI of that event.
//--------------------------------------------------------------------------------------
class EnemySpottedMessage : public Message
{
public:
	EnemySpottedMessage(const EnemySpottedMessageData& data) 
		: Message(EnemySpottedMessageType),
		  m_data(data)
	{}

	const EnemySpottedMessageData& GetData(void) const { return m_data; }

private:
	EnemySpottedMessageData m_data; // The contents of the message
};

//--------------------------------------------------------------------------------------
// Contains data required for a message sent when an entity lost sight of an enemy
// and wants to inform the associated team AI of that event.
//--------------------------------------------------------------------------------------
struct LostSightOfEnemyMessageData
{
	LostSightOfEnemyMessageData(unsigned long entityId, unsigned long enemyId) : m_entityId(entityId),
																				 m_enemyId(enemyId)
	{}

	unsigned long m_entityId; // The id of the entity that lost sight of an enemy
	unsigned long m_enemyId;  // The id of the enemy that was lost sight of
};

//--------------------------------------------------------------------------------------
// Message sent when an entity lost sight of an enemy
// and wants to inform the associated team AI of that event.
//--------------------------------------------------------------------------------------
class LostSightOfEnemyMessage : public Message
{
public:
	LostSightOfEnemyMessage(const LostSightOfEnemyMessageData& data) : Message(LostSightOfEnemyMessageType),
																	   m_data(data)
	{}

	const LostSightOfEnemyMessageData& GetData(void) const { return m_data; }

private:
	LostSightOfEnemyMessageData m_data; // The contents of the message
};

//--------------------------------------------------------------------------------------
// Contains data required for a message sent by an entity to update the position of an
// enemy entity in the knowledge of the team AI.
//--------------------------------------------------------------------------------------
struct UpdateEnemyPositionMessageData
{
	UpdateEnemyPositionMessageData(unsigned long enemyId, const XMFLOAT2& enemyPosition) : m_enemyId(enemyId),
																						   m_enemyPosition(enemyPosition)
	{}

	unsigned long m_enemyId;		  // The id of the enemy, for which the position should be updated
	XMFLOAT2      m_enemyPosition; // The position, at which the enemy was seen
};
	
//--------------------------------------------------------------------------------------
// Message sent by an entity to update the position of an
// enemy entity in the knowledge of the team AI.
//--------------------------------------------------------------------------------------
class UpdateEnemyPositionMessage : public Message
{
public:
	UpdateEnemyPositionMessage(const UpdateEnemyPositionMessageData& data) : Message(UpdateEnemyPositionMessageType),
																		     m_data(data)
	{}

	const UpdateEnemyPositionMessageData& GetData(void) const { return m_data; }

private:
	UpdateEnemyPositionMessageData m_data; // The contents of the message
};

//--------------------------------------------------------------------------------------
// Contains data required for a message sent when an entity was attacked by an enemy and wants
// to inform the team AI of that event.
//--------------------------------------------------------------------------------------
struct AttackedByEnemyMessageData
{
	AttackedByEnemyMessageData(unsigned long entityId, unsigned long enemyId, const XMFLOAT2& attackPos) : m_entityId(entityId),
																										   m_enemyId(enemyId),
																										   m_attackPosition(attackPos)
	{}

	unsigned long m_entityId;		// The id of the entity that attacked by an enemy
	unsigned long m_enemyId;		// The id of the enemy, that attacked the entity
	XMFLOAT2      m_attackPosition; // The position, from which the entity was attacked
};

//--------------------------------------------------------------------------------------
// Message sent when an entity was attacked by an enemy and wants
// to inform the team AI of that event.
//--------------------------------------------------------------------------------------
class AttackedByEnemyMessage : public Message
{
public:
	AttackedByEnemyMessage(const AttackedByEnemyMessageData& data) : Message(AttackedByEnemyMessageType),
																	 m_data(data)
	{}

	const AttackedByEnemyMessageData& GetData(void) const { return m_data; }

private:
	AttackedByEnemyMessageData m_data; // The contents of the message
};

//--------------------------------------------------------------------------------------
// Contains data required for a message sent when an entity wants to inform the team AI 
// of an updated status of its current order.
//--------------------------------------------------------------------------------------
struct UpdateOrderStateMessageData
{
	UpdateOrderStateMessageData(unsigned long entityId, OrderState orderState) : m_entityId(entityId),
																				 m_orderState(orderState)
	{}

	unsigned long m_entityId;	 //The id of the entity that wants to report an update
	OrderState    m_orderState;  // The current state of the entity's active order
};

//--------------------------------------------------------------------------------------
// Message sent when an entity wants to inform the team AI 
// of an updated status of its current order.
//--------------------------------------------------------------------------------------
class UpdateOrderStateMessage : public Message
{
public:
	UpdateOrderStateMessage(const UpdateOrderStateMessageData& data) : Message(UpdateOrderStateMessageType),
																	   m_data(data)
	{}

	const UpdateOrderStateMessageData& GetData(void) const { return m_data; }

private:
	UpdateOrderStateMessageData m_data; // The contents of the message
};

#endif // ENTITY_TO_TEAM_AI_MESSAGES_H