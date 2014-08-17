/* 
*  Kevin Meergans, SquadAI, 2014
*  DetermineObservationTarget.cpp
*  An action behaviour that determines an observation target for the entity and sets it as 
*  the active observation target.
*/

// Includes
#include "DetermineObservationTarget.h"
#include "Entity.h"

DetermineObservationTarget::DetermineObservationTarget(const char* name, Entity* pEntity) : UniversalIndividualBehaviour(name, pEntity)
{
}

DetermineObservationTarget::~DetermineObservationTarget(void)
{
}

//--------------------------------------------------------------------------------------
// Updates the behaviour.
// Param1: The time in seconds passed since the last frame.
// Returns the state of the behaviour.
//--------------------------------------------------------------------------------------
BehaviourStatus DetermineObservationTarget::Update(float deltaTime)
{
	return GetEntity()->DetermineObservationTarget(deltaTime);
}

