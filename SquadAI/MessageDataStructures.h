/* 
*  Kevin Meergans, SquadAI, 2014
*  MessageDataStructures.h
*  This file contains data structures required for the message passing between entities and the
*  test environment as well as other components of the application.
*/

#ifndef MESSAGE_DATA_STRUCTURES_H
#define MESSAGE_DATA_STRUCTURES_H

// Includes
#include <DirectXMath.h>
#include "ObjectTypes.h"
#include "Entity.h"
#include "Objective.h"

using namespace DirectX;

//--------------------------------------------------------------------------------------
// Identifies the type of a message.
//--------------------------------------------------------------------------------------
enum MessageType
{
	ProjectileFiredMessageType,			// A projectile was fired by an entity
	HitMessageType,						// The entity receiving this message was hit by a projectile
	EntityKilledMessageType,			// An entity was killed and other entities are notified of this event
	ReadyToRespawnMessageType,			// Tells an entity that it is ready to respawn
	AddObjectiveMessageType,			// Puts an objective object under the control of a specific game context
	EntityReachedObjectiveMessageType	// An entity collided with an objective object.
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

//--------------------------------------------------------------------------------------
// Contains data required for a message sent when an entity is ready to respawn.
//--------------------------------------------------------------------------------------
class ReadyToRespawnMessage : public Message
{
public:
	ReadyToRespawnMessage(const XMFLOAT2& respawnPosition) : Message(ReadyToRespawnMessageType),
															 m_respawnPosition(respawnPosition)
	{}

	const XMFLOAT2& GetRespawnPosition(void) const { return m_respawnPosition; }

private:
	XMFLOAT2 m_respawnPosition; // The position, where the entity will reenter the game
};

//--------------------------------------------------------------------------------------
// Contains data required for a message sent when an objective should be put under the 
// control of a specific game context.
//--------------------------------------------------------------------------------------
class AddObjectiveMessage : public Message
{
public:
	AddObjectiveMessage(Objective* pObjective) : Message(AddObjectiveMessageType),
														m_pObjective(pObjective)
	{}

	Objective* GetObjective(void) { return m_pObjective; }

private:
	Objective* m_pObjective; // The objective that should be added to the game context
};

//--------------------------------------------------------------------------------------
// Contains data required for a message sent when an entity has reached an objective,
// that is collided with it.
//--------------------------------------------------------------------------------------
class EntityReachedObjectiveMessage : public Message
{
public:
	EntityReachedObjectiveMessage(Entity* pEntity, Objective* pObjective) : Message(EntityReachedObjectiveMessageType),
																			m_pEntity(pEntity),
																			m_pObjective(pObjective)
	{}

	Entity*			  GetEntity(void)    { return m_pEntity; }
	Objective* GetObjective(void) { return m_pObjective; }

private:
	Entity*			  m_pEntity;  // The entity that reached the objective
	Objective* m_pObjective; // The objective that was reached by the entity
};


#endif // MESSAGE_DATA_STRUCTURES_H