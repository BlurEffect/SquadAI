/* 
*  Kevin Meergans, SquadAI, 2014
*  DetermineGreatestSuspectedThreat.cpp
*  An action behaviour that determines and sets the greatest suspected
*  threat for the entity.
*/

// Includes
#include "DetermineGreatestSuspectedThreat.h"
#include "Entity.h"

DetermineGreatestSuspectedThreat::DetermineGreatestSuspectedThreat(const char* name, Entity* pEntity) : UniversalIndividualBehaviour(name, pEntity)
{
}

DetermineGreatestSuspectedThreat::~DetermineGreatestSuspectedThreat(void)
{
}

//--------------------------------------------------------------------------------------
// Updates the behaviour.
// Param1: The time in seconds passed since the last frame.
// Returns the state of the behaviour.
//--------------------------------------------------------------------------------------
BehaviourStatus DetermineGreatestSuspectedThreat::Update(float deltaTime)
{
	return GetEntity()->DetermineGreatestSuspectedThreat(deltaTime);
}
