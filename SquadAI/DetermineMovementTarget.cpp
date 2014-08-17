/* 
*  Kevin Meergans, SquadAI, 2014
*  DetermineMovementTarget.cpp
*  An action behaviour that determines a movement target for the entity and sets it as 
*  the active movement target.
*/

// Includes
#include "DetermineMovementTarget.h"
#include "Entity.h"

DetermineMovementTarget::DetermineMovementTarget(const char* name, Entity* pEntity) : UniversalIndividualBehaviour(name, pEntity)
{
}

DetermineMovementTarget::~DetermineMovementTarget(void)
{
}

//--------------------------------------------------------------------------------------
// Updates the behaviour.
// Param1: The time in seconds passed since the last frame.
// Returns the state of the behaviour.
//--------------------------------------------------------------------------------------
BehaviourStatus DetermineMovementTarget::Update(float deltaTime)
{
	return GetEntity()->DetermineMovementTarget(deltaTime);
}

