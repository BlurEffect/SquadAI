/* 
*  Kevin Meergans, SquadAI, 2014
*  DeterminePatrolTarget.cpp
*  An action behaviour that determines a patrol target for the entity and sets it as 
*  the active movement target.
*/

// Includes
#include "DeterminePatrolTarget.h"
#include "Entity.h"

DeterminePatrolTarget::DeterminePatrolTarget(Entity* pEntity, const char* name) : Behaviour(pEntity, name)
{
}

DeterminePatrolTarget::~DeterminePatrolTarget(void)
{
}

//--------------------------------------------------------------------------------------
// Updates the behaviour.
// Param1: The time in seconds passed since the last frame.
// Returns the state of the behaviour.
//--------------------------------------------------------------------------------------
BehaviourStatus DeterminePatrolTarget::Update(float deltaTime)
{
	return GetEntity()->DeterminePatrolTarget(deltaTime);
}

