/* 
*  Kevin Meergans, SquadAI, 2014
*  Flag.cpp
*  Encapsulates a flag object.
*/

// Includes
#include "Flag.h"

Flag::Flag(void) : m_team(EntityTeam(0)),
				   m_position(0.0f, 0.0f),
				   m_rotation(0.0f),
				   m_uniformScale(1.0f),
				   m_resetInterval(0.0f),
				   m_pCollider(nullptr),
				   m_currentState(InBase),
				   m_resetPosition(0.0f, 0.0f),
				   m_resetTimePassed(0.0f),
				   m_pCarrier(nullptr)
{
}

Flag::~Flag(void)
{
	if(m_pCollider)
	{
		delete m_pCollider;
	}
}

//--------------------------------------------------------------------------------------
// Initialise the flag object.
// Param1: The team that the flag belongs to.
// Param2: The initial position of the flag in world space.
// Param3: The rotation of the flag.
// Param4: The scale of the flag.
// Param5: The time it takes for the flag to be reset after being dropped
// Returns true if the flag was initialised successfully, false otherwise.
//--------------------------------------------------------------------------------------
bool Flag::Initialise(EntityTeam team, const XMFLOAT2& position, float rotation, float uniformScale, float resetInterval, Collider* pCollider)
{
	m_team = team;
	m_position = position;
	m_rotation = rotation;
	m_uniformScale = uniformScale;
	m_resetInterval = resetInterval;

	m_pCollider = ColliderFactory::CreateCollider(pCollider);

	if(!m_pCollider)
	{
		return false;
	}

	m_currentState = InBase;
	m_resetPosition = m_position;
	m_resetTimePassed = 0.0f;
	m_pCarrier = nullptr;

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
		m_position = m_pCarrier->GetPosition();
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
	m_position = m_resetPosition;
	m_currentState = InBase;
}

// Data access functions

EntityTeam Flag::GetTeam(void) const
{
	return m_team;
}

const XMFLOAT2& Flag::GetPosition(void) const
{
	return m_position;
}

float Flag::GetRotation(void) const
{
	return m_rotation;
}

float Flag::GetUniformScale(void) const
{
	return m_uniformScale;
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

void Flag::SetPosition(const XMFLOAT2& position)
{
	m_position = position;
}

void Flag::SetRotation(float rotation)
{
	m_rotation = rotation;
}

void Flag::SetUniformScale(float uniformScale)
{
	m_uniformScale = uniformScale;
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
