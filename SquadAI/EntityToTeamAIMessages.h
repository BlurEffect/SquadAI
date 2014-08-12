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

using namespace DirectX;

//--------------------------------------------------------------------------------------
// Contains data required for a message sent when an entity has spotted an enemy and wants
// to inform the associated team AI of that event.
//--------------------------------------------------------------------------------------
class EnemySpottedMessage : public Message
{
public:
	EnemySpottedMessage(unsigned long spotterId, unsigned long enemyId, const XMFLOAT2& enemyPosition) 
		: Message(EnemySpottedMessageType),
		  m_spotterId(spotterId),
		  m_enemyId(enemyId),
		  m_enemyPosition(enemyPosition)
	{}

	unsigned long   GetSpotterId(void) const { return m_spotterId; }
	unsigned long   GetEnemyId(void) const { return m_enemyId; }
	const XMFLOAT2& GetEnemyPosition(void) const { return m_enemyPosition; }

private:
	unsigned long m_spotterId;      // The id of the entity that spotted an enemy
	unsigned long m_enemyId;		// The id of the enemy that was spotted by the entity
	XMFLOAT2      m_enemyPosition;	// The position, at which the entity was spotted
};

//--------------------------------------------------------------------------------------
// Contains data required for a message sent when an entity lost sight of an enemy
// and wants to inform the associated team AI of that event.
//--------------------------------------------------------------------------------------
class LostSightOfEnemyMessage : public Message
{
public:
	LostSightOfEnemyMessage(unsigned long entityId, unsigned long enemyId) : Message(LostSightOfEnemyMessageType),
																			 m_entityId(entityId),
																			 m_enemyId(enemyId)
	{}

	unsigned long GetEntityId(void) const { return m_entityId; }
	unsigned long GetEnemyId(void) const { return m_enemyId; }

private:
	unsigned long m_entityId; // The id of the entity that lost sight of an enemy
	unsigned long m_enemyId;  // The id of the enemy that was lost sight of
};

//--------------------------------------------------------------------------------------
// Contains data required for a message sent by an entity to update the position of an
// enemy entity in the knowledge of the team AI.
//--------------------------------------------------------------------------------------
class UpdateEnemyPositionMessage : public Message
{
public:
	UpdateEnemyPositionMessage(unsigned long enemyId, const XMFLOAT2& enemyPosition) : Message(UpdateEnemyPositionMessageType),
																				   	   m_enemyId(enemyId),
																					   m_enemyPosition(enemyPosition)
	{}

	unsigned long   GetEnemyId(void) const { return m_enemyId; }
	const XMFLOAT2& GetEnemyPosition(void) const { return m_enemyPosition; }

private:
	unsigned long m_enemyId;		  // The id of the enemy, for which the position should be updated
	XMFLOAT2      m_enemyPosition; // The position, at which the enemy was seen
};
	
//--------------------------------------------------------------------------------------
// Contains data required for a message sent when an entity has killed an enemy and wants
// to inform the team AI of that event.
//--------------------------------------------------------------------------------------
class EnemyKilledMessage : public Message
{
public:
	EnemyKilledMessage(unsigned long entityId, unsigned long enemyId) : Message(EnemyKilledMessageType),
																		m_entityId(entityId),
																		m_enemyId(enemyId)
	{}

	unsigned long GetEntityId(void) { return m_entityId; }
	unsigned long GetEnemyId(void) { return m_enemyId; }

private:
	unsigned long m_entityId; // The id of the entity that killed the enemy
	unsigned long m_enemyId;  // The id of the enemy, that was killed
};

#endif // ENTITY_TO_TEAM_AI_MESSAGES_H