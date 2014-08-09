/* 
*  Kevin Meergans, SquadAI, 2014
*  Idle.cpp
*  An action behaviour that has the entity perform an idle behaviour.
*/

// Includes
#include "Idle.h"
#include "Entity.h"

Idle::Idle(Entity* pEntity, const char* name) : Behaviour(pEntity, name)
{
}

Idle::~Idle(void)
{
}

//--------------------------------------------------------------------------------------
// Updates the behaviour.
// Param1: The time in seconds passed since the last frame.
// Returns the state of the behaviour.
//--------------------------------------------------------------------------------------
BehaviourStatus Idle::Update(float deltaTime)
{
	return GetEntity()->Idle(deltaTime);
}
