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
	ProjectileFiredMessageType, // A projectile was fired by an entity
	HitMessageType,			    // The entity receiving this message was hit by a projectile
	EntityKilledMessageType     // An entity was killed and other entities are notified of this event
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
// Contains data required for a message sent when an entity fires a projectile.
//--------------------------------------------------------------------------------------
class ProjectileFiredMessage : public Message
{
public:
	ProjectileFiredMessage(unsigned long shooterId, EntityTeam shooterTeam, const XMFLOAT2& origin, const XMFLOAT2& target) 
		: Message(ProjectileFiredMessageType),
		  m_shooterId(shooterId),
		  m_shooterTeam(shooterTeam),
		  m_origin(origin),
		  m_target(target)
	{}
	
	unsigned long	GetShooterId(void) const { return m_shooterId; }
	EntityTeam      GetShooterTeam(void) const { return m_shooterTeam; }
	const XMFLOAT2& GetOrigin(void) const { return m_origin; }
	const XMFLOAT2& GetTarget(void) const { return m_target; }

private:
	unsigned long m_shooterId;   // The id of the entity that shot the projectile
	EntityTeam    m_shooterTeam; // The team of the entity that shot the target
	XMFLOAT2      m_origin;		 // The position, from which the projectile is being shot
	XMFLOAT2      m_target;		 // The target position, at which the projectile is being fired
};

//--------------------------------------------------------------------------------------
// Contains data required for a message sent when an entity is hit by a projectile.
//--------------------------------------------------------------------------------------
class HitMessage : public Message
{
public:
	HitMessage(float damage, unsigned long id, bool shooterAlive, const XMFLOAT2& position) : Message(HitMessageType),
																							  m_damage(damage),
																							  m_shooterAlive(shooterAlive),
																							  m_id(id),
																							  m_position(position)
	{}

	float		    GetDamage(void) const      { return m_damage; }
	unsigned long   GetShooterId(void) const   { return m_id; }
	bool            IsShooterAlive(void) const { return m_shooterAlive; }
	const XMFLOAT2& GetPosition(void) const    { return m_position; }

private:
	float         m_damage;			// The damage that the projectile will deal
	unsigned long m_id;				// The id of the shooting entity
	bool          m_shooterAlive;	// Tells whether the entity that shot the projectile is still alive or dead
	XMFLOAT2      m_position;		// The position, from which the projectile was originally shot
};

//--------------------------------------------------------------------------------------
// Contains data required for a message sent when an entity was killed.
//--------------------------------------------------------------------------------------
class EntityKilledMessage : public Message
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