/* 
*  Kevin Meergans, SquadAI, 2014
*  EventTypes.h
*  This file contains an enumeration of available event types. Events are fairly similar to messages but are
*  processed immediately when being received by an entity while messages are added to a queue that is
*  processed when the entity is updated. Events usually involve the test environment as one of the
*  communication partners.
*/

#ifndef EVENT_TYPES_H
#define EVENT_TYPES_H

//--------------------------------------------------------------------------------------
// Identifies the type of a event.
//--------------------------------------------------------------------------------------
enum EventType
{
	// Events sent from entities to the test environment
	ProjectileFiredEventType,			// A projectile was fired by an entity, which has to be added by the test environment
	EntityDiedEventType,				// An entity was killed through a hit and has to notify the test environment

	// Events sent from the test environment to the entities
	EntityHitEventType,					// The test environment detected a collision of a projectile with an entity and notifies latter one of having been hit
	RespawnEventType,					// The test environment notifies an entity of being able to respawn

	// Events sent from the test environment to the game context
	ObjectiveAddedEventType,			// An objective was added by the user and should be registered with the game context to put it under its control
	EntityReachedObjectiveEventType,	// An entity collided with an objective object, notify the game context
};

#endif // EVENT_TYPES_H