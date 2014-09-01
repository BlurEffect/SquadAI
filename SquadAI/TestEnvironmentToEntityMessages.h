/* 
*  Kevin Meergans, SquadAI, 2014
*  TestEnvironmentToEntityMessages.h
*  This file contains all messages that are exchanged between the test environment and
*  and entities.
*/

#ifndef TEST_ENVIRONMENT_TO_ENTITY_MESSAGES_H
#define TEST_ENVIRONMENT_TO_ENTITY_MESSAGES_H

// Includes
#include <DirectXMath.h>
#include "Message.h"
#include "ObjectTypes.h"

using namespace DirectX;

//--------------------------------------------------------------------------------------
// Contains data required for a message sent to entities in order to inform them of the
// death of another entity.
//--------------------------------------------------------------------------------------
struct EntityKilledMessageData
{
	EntityKilledMessageData(EntityTeam team, unsigned long id, unsigned long shotterId) : m_team(team),
																						  m_id(id),
																						  m_shooterId(shotterId)
	{}

	EntityTeam    m_team;		// The team that the killed entity belongs to
	unsigned long m_id;			// The id of the entity that was killed
	unsigned long m_shooterId;	// The id of the entity that shot the projectile killing the entity
};

//--------------------------------------------------------------------------------------
// Message sent to entities in order to inform them of the death of another entity.
//--------------------------------------------------------------------------------------
class EntityKilledMessage : public Message
{
public:
	EntityKilledMessage(const EntityKilledMessageData& data) : Message(EntityKilledMessageType),
															   m_data(data)
	{}

	const EntityKilledMessageData& GetData(void) const { return m_data; }

private:
	EntityKilledMessageData m_data; // The contents of the message
};

#endif // TEST_ENVIRONMENT_TO_ENTITY_MESSAGES_H