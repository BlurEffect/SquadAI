/* 
*  Kevin Meergans, SquadAI, 2014
*  MessageDataStructures.h
*  This file contains data structures required for the message passing between entities and the
*  test environment.
*/

#ifndef MESSAGE_DATA_STRUCTURES_H
#define MESSAGE_DATA_STRUCTURES_H

// Includes
#include <DirectXMath.h>
#include "ObjectTypes.h"

using namespace DirectX;

//--------------------------------------------------------------------------------------
// Identifies the type of a message.
//--------------------------------------------------------------------------------------
enum MessageType
{
	HitMessageType,			   // The entity receiving this message was hit by a projectile
	EntityKilledMessageType    // An entity was killed and other entities are notified of this event
};

//--------------------------------------------------------------------------------------
// Class representing a message.
//--------------------------------------------------------------------------------------
class Message
{
public:
	Message(MessageType messageType) : m_messageType(messageType){}

	MessageType GetType(void) const { return m_messageType; }

private:
	MessageType m_messageType;	// The type of the message
};		

// The following classes define the specific data contained in the messages
// of the different types

//--------------------------------------------------------------------------------------
// Contains data required for a message sent when an entity is hit by a projectile.
//--------------------------------------------------------------------------------------
class HitMessage : public Message
{
public:
	HitMessage(float damage, unsigned long id, const XMFLOAT2& position) : Message(HitMessageType),
																		   m_damage(damage),
																		   m_id(id),
														                   m_position(position)
	{}

	float		    GetDamage(void) const    { return m_damage; }
	unsigned long   GetShooterId(void) const { return m_id; }
	const XMFLOAT2& GetPosition(void) const  { return m_position; }

private:
	float         m_damage;	  // The damage that the projectile will deal
	unsigned long m_id;       // The id of the shooting entity
	XMFLOAT2      m_position; // The position, from which the projectile was shot
};

//--------------------------------------------------------------------------------------
// Contains data required for a message sent when an entity was killed.
//--------------------------------------------------------------------------------------
struct EntityKilledMessage : Message
{
public:
	EntityKilledMessage(EntityTeam team, unsigned long id) : Message(EntityKilledMessageType),
															 m_team(team),
														     m_id(id)
	{}

	EntityTeam    GetTeam(void) const { return m_team; }
	unsigned long GetId(void) const { return m_id; }

private:
	EntityTeam    m_team; // The team that the killed entity belongs to
	unsigned long m_id;   // The id of the entity that was killed
};


#endif // MESSAGE_DATA_STRUCTURES_H