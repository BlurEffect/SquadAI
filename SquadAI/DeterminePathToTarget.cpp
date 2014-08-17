/* 
*  Kevin Meergans, SquadAI, 2014
*  DeterminePathToTarget.cpp
*  An action behaviour that determines a path to a movement target for the entity and sets it as 
*  the active path to that target.
*/

// Includes
#include "DeterminePathToTarget.h"
#include "Entity.h"

DeterminePathToTarget::DeterminePathToTarget(const char* name, Entity* pEntity) : UniversalIndividualBehaviour(name, pEntity)
{
}

DeterminePathToTarget::~DeterminePathToTarget(void)
{
}

//--------------------------------------------------------------------------------------
// Updates the behaviour.
// Param1: The time in seconds passed since the last frame.
// Returns the state of the behaviour.
//--------------------------------------------------------------------------------------
BehaviourStatus DeterminePathToTarget::Update(float deltaTime)
{
	return GetEntity()->DeterminePathToTarget(deltaTime);
}

