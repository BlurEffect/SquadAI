/* 
*  Kevin Meergans, SquadAI, 2014
*  DetermineGreatestKnownThreat.cpp
*  An action behaviour that determines and sets the greatest known
*  threat for the entity.
*/

// Includes
#include "DetermineGreatestKnownThreat.h"
#include "Entity.h"

DetermineGreatestKnownThreat::DetermineGreatestKnownThreat(const char* name, Entity* pEntity) : UniversalIndividualBehaviour(name, pEntity)
{
}

DetermineGreatestKnownThreat::~DetermineGreatestKnownThreat(void)
{
}

//--------------------------------------------------------------------------------------
// Updates the behaviour.
// Param1: The time in seconds passed since the last frame.
// Returns the state of the behaviour.
//--------------------------------------------------------------------------------------
BehaviourStatus DetermineGreatestKnownThreat::Update(float deltaTime)
{
	return GetEntity()->DetermineGreatestKnownThreat(deltaTime);
}
