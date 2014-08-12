/* 
*  Kevin Meergans, SquadAI, 2014
*  MessageDataStructures.h
*  This file contains an enumeration of available message types and the abstract base
*  class, from which as specific messages have to inherit.
*/

// Note: Messages are defined in their header files and certain types of messages are 
//       contained within a single header file. Although this is against encapsulation,
//		 I decided to do so because there are many different messages and each of them
//       is hardly more than a collection of a few member variables and corresponding getters.

#ifndef MESSAGE_H
#define MESSAGE_H

//--------------------------------------------------------------------------------------
// Identifies the type of a message.
//--------------------------------------------------------------------------------------
enum MessageType
{
	// Messages sent from entities to the test environment
	ProjectileFiredMessageType,			// A projectile was fired by an entity
	
	// Messages sent from the test environment to the entities
	HitMessageType,						// The entity receiving this message was hit by a projectile
	EntityKilledMessageType,			// An entity was killed and other entities are notified of this event (also sent from entities to the test environment)
	ReadyToRespawnMessageType,			// Tells an entity that it is ready to respawn
	
	// Messages sent from the test environment to the game context
	AddObjectiveMessageType,			// Puts an objective object under the control of a specific game context
	EntityReachedObjectiveMessageType,	// An entity collided with an objective object.

	// Messages sent from entities to the team AI
	EnemySpottedMessageType,			// An enemy was spotted by an entity
	LostSightOfEnemyMessageType,		// An entity has lost sight of an enemy
	UpdateEnemyPositionMessageType,		// Updates the position of an enemy 
	EnemyKilledMessageType,				// An enemy has been killed by an entity
	
	// Messages sent from the general game context to the team AI
	ScoreUpdateMessageType,				// A team scored
	TimeUpdateMessageType,				// A certain amount of time has passed in the current round

	// Messages sent from a specific game context (game mode specific)
	FlagPickedUpMessageType,			// A flag has been picked up
	FlagDroppedMessageType,				// A flag has been dropped
	FlagReturnedMessageType,			// A flag has been returned
};

//--------------------------------------------------------------------------------------
// Class representing a message.
//--------------------------------------------------------------------------------------
class Message
{
public:
	Message(MessageType messageType) : m_messageType(messageType){}
	virtual ~Message(void) = 0{}

	MessageType GetType(void) const { return m_messageType; }

private:
	MessageType m_messageType;	// The type of the message
};		

#endif // MESSAGE_H