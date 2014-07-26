/* 
*  Kevin Meergans, SquadAI, 2014
*  Flag.cpp
*  Encapsulates a flag object.
*/

// Includes
#include "Flag.h"

Flag::Flag(void) : CollidableObject(),
				   m_team(EntityTeam(None)),
				   m_resetInterval(0.0f),
				   m_currentState(InBase),
				   m_resetPosition(0.0f, 0.0f),
				   m_resetTimePassed(0.0f),
				   m_pCarrier(nullptr)
{
}

Flag::~Flag(void)
{
}

//--------------------------------------------------------------------------------------
// Initialise the flag object.
// Param1: The initial position of the flag in world space.
// Param2: The rotation of the flag.
// Param3: The scale of the flag.
// Param4: The type of the collider that should be created.
// Param5: A pointer to the initialisation data for the collider.
// Param6: The team that the flag belongs to.
// Param7: The time it takes for the flag to be reset after being dropped
// Returns true if the flag was initialised successfully, false otherwise.
//--------------------------------------------------------------------------------------
bool Flag::Initialise(const XMFLOAT2& position, float rotation, float uniformScale, ColliderType colliderType, void* pColliderData, EntityTeam team, float resetInterval)
{
	if(!CollidableObject::Initialise(position, rotation, uniformScale, colliderType, pColliderData))
	{
		return false;
	}

	m_team            = team;
	m_resetInterval	  = resetInterval;
	m_currentState    = InBase;
	m_resetPosition   = GetPosition();
	m_resetTimePassed = 0.0f;
	m_pCarrier        = nullptr;

	return true;
}

//--------------------------------------------------------------------------------------
// Call this when the flag is picked up by an entity.
// Param1: A pointer to entity that is trying to pick up the flag.
// Returns true if the flag was successfully picked up, false otherwise.
//--------------------------------------------------------------------------------------
bool Flag::OnPickUp(Entity* pEntity)
{
	if(!pEntity)
	{
		return false;
	}

	m_pCarrier     = pEntity;
	m_currentState = Stolen;
	
	// set carrier bool var on entity

	return true;
}
	
//--------------------------------------------------------------------------------------
// Call this to update the flag including position and reset timer.
// Param1: The time in seconds passed since the last update.
//--------------------------------------------------------------------------------------
void Flag::Update(float deltaTime)
{
	switch(m_currentState)
	{
	case Stolen:
		// Update the position of the flag according to the position of the carrier
		SetPosition(m_pCarrier->GetPosition());
		break;
	case Dropped:
		m_resetTimePassed += deltaTime;

		if(m_resetTimePassed >= m_resetInterval)
		{
			// Reset the flag if it was dropped for a specific time
			OnReset();
		}
		break;
	}

}

//--------------------------------------------------------------------------------------
// Call this when the flag was dropped 
//--------------------------------------------------------------------------------------
void Flag::OnDrop(void)
{
	m_currentState	  = Dropped;
	m_pCarrier        = nullptr;
	m_resetTimePassed = 0.0f;
}

//--------------------------------------------------------------------------------------
// Call this to reset the flag and move it back to its original position in the base
// of the team it belongs to.
//--------------------------------------------------------------------------------------
void Flag::OnReset(void)
{
	SetPosition(m_resetPosition);
	m_currentState = InBase;
}

// Data access functions

EntityTeam Flag::GetTeam(void) const
{
	return m_team;
}

float Flag::GetResetInterval(void) const
{
	return m_resetInterval;
}

FlagState Flag::GetState(void) const
{
	return m_currentState;
}

const XMFLOAT2& Flag::GetResetPosition(void) const
{
	return m_resetPosition;
}

float Flag::GetResetTimePassed(void) const
{
	return m_resetTimePassed;
}

const Entity* Flag::GetCarrier(void) const
{
	return m_pCarrier;
}

void Flag::SetTeam(EntityTeam team)
{
	m_team = team;
}

void Flag::SetResetInterval(float resetInterval)
{
	m_resetInterval = resetInterval;
}

void Flag::SetResetPosition(const XMFLOAT2& position)
{
	m_resetPosition = position;
}

void Flag::SetCarrier(Entity* pCarrier)
{
	m_pCarrier = pCarrier;
}
