/* 
*  Kevin Meergans, SquadAI, 2014
*  TeamAIToEntityMessages.h
*  This file contains all messages that are sent from a team AI to individual entities.
*/

#ifndef TEAM_AI_TO_ENTITY_MESSAGES_H
#define TEAM_AI_TO_ENTITY_MESSAGES_H

// Includes
#include "Message.h"
#include "Order.h"

//--------------------------------------------------------------------------------------
// Contains data required for a message sent when a team Ai issues an order to a
// subordinate entity.
//--------------------------------------------------------------------------------------
struct FollowOrderMessageData
{
	FollowOrderMessageData(Order* pOrder) : m_pOrder(pOrder)
	{}

	Order* m_pOrder; // A pointer to the order sent by the team AI
};


//--------------------------------------------------------------------------------------
// Message sent when an entity has spotted an enemy and wants
// to inform the associated team AI of that event.
//--------------------------------------------------------------------------------------
class FollowOrderMessage : public Message
{
public:
	FollowOrderMessage(const FollowOrderMessageData& data) 
		: Message(FollowOrderMessageType),
		  m_data(data)
	{}

	const FollowOrderMessageData& GetData(void) const { return m_data; }

private:
	FollowOrderMessageData m_data; // The contents of the message
};

#endif // TEAM_AI_TO_ENTITY_MESSAGES_H