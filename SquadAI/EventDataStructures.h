/* 
*  Kevin Meergans, SquadAI, 2014
*  EventDataStructures.h
*  This file contains data structures bundling data for each of the available events.
*/

#ifndef EVENT_DATA_STRUCTURES_H
#define EVENT_DATA_STRUCTURES_H

// Includes
#include <DirectXMath.h>
#include "EventTypes.h"
#include "ObjectTypes.h"

// Forward declarations
class Objective;
class Entity;

using namespace DirectX;

//------------------------------------------------------------------------------------------------------------------------------------
// Events sent from entities to the test environment

//--------------------------------------------------------------------------------------
// Contains data required for an event sent when an entity fires a projectile.
//--------------------------------------------------------------------------------------
struct ProjectileFiredEventData
{
public:
	ProjectileFiredEventData(unsigned long shooterId, EntityTeam shooterTeam, const XMFLOAT2& origin, const XMFLOAT2& target) 
		  : m_shooterId(shooterId),
		    m_shooterTeam(shooterTeam),
		    m_origin(origin),
		    m_target(target)
	{}

	unsigned long m_shooterId;   // The id of the entity that shot the projectile
	EntityTeam    m_shooterTeam; // The team of the entity that shot the target
	XMFLOAT2      m_origin;		 // The position, from which the projectile is being shot
	XMFLOAT2      m_target;		 // The target position, at which the projectile is being fired
};


//--------------------------------------------------------------------------------------
// Contains data required for an event sent when an entity was killed.
//--------------------------------------------------------------------------------------
struct EntityDiedEventData
{
public:
	EntityDiedEventData(EntityTeam team, unsigned long id) : m_team(team),
															 m_id(id)
	{}

	EntityTeam    m_team; // The team that the killed entity belongs to
	unsigned long m_id;   // The id of the entity that was killed
};



//------------------------------------------------------------------------------------------------------------------------------------
// Events sent from the test environment to entities

//--------------------------------------------------------------------------------------
// Contains data required for an event sent when an entity is hit by a projectile.
//--------------------------------------------------------------------------------------
struct EntityHitEventData
{
public:
	EntityHitEventData(float damage, unsigned long id, bool shooterAlive, const XMFLOAT2& position) : m_damage(damage),
																									  m_shooterAlive(shooterAlive),
																									  m_id(id),
																									  m_position(position)
	{}

	float         m_damage;			// The damage that the projectile will deal
	unsigned long m_id;				// The id of the shooting entity
	bool          m_shooterAlive;	// Tells whether the entity that shot the projectile is still alive or dead
	XMFLOAT2      m_position;		// The position, from which the projectile was originally shot
};


//--------------------------------------------------------------------------------------
// Contains data required for an event sent when an entity is ready to respawn.
//--------------------------------------------------------------------------------------
struct RespawnEventData
{
public:
	RespawnEventData(const XMFLOAT2& respawnPosition) : m_respawnPosition(respawnPosition)
	{}

	XMFLOAT2 m_respawnPosition; // The position, where the entity will reenter the game
};



//------------------------------------------------------------------------------------------------------------------------------------
// Events sent from the test environment to the game context

//--------------------------------------------------------------------------------------
// Contains data required for an event sent when an objective should be put under the 
// control of a specific game context.
//--------------------------------------------------------------------------------------
struct ObjectiveAddedEventData
{
public:
	ObjectiveAddedEventData(Objective* pObjective) : m_pObjective(pObjective)
	{}

	Objective* m_pObjective; // The objective that should be added to the game context
};

//--------------------------------------------------------------------------------------
// Contains data required for a message sent when an entity has reached an objective,
// that is collided with it.
//--------------------------------------------------------------------------------------
struct EntityReachedObjectiveEventData 
{
public:
	EntityReachedObjectiveEventData(Entity* pEntity, Objective* pObjective) : m_pEntity(pEntity),
																			  m_pObjective(pObjective)
	{}

	Entity*    m_pEntity;    // The entity that reached the objective
	Objective* m_pObjective; // The objective that was reached by the entity
};


#endif // EVENT_DATA_STRUCTURES_H