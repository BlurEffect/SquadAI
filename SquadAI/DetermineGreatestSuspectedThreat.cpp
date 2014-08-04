/* 
*  Kevin Meergans, SquadAI, 2014
*  DetermineGreatestSuspectedThreat.cpp
*  An action behaviour that determines and sets the greatest suspected
*  threat for the entity.
*/

// Includes
#include "DetermineGreatestSuspectedThreat.h"
#include "Entity.h"

DetermineGreatestSuspectedThreat::DetermineGreatestSuspectedThreat(Entity* pEntity, const char* name) : Behaviour(pEntity, name)
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

//--------------------------------------------------------------------------------------
// Initialises the behaviour.
//--------------------------------------------------------------------------------------
void DetermineGreatestSuspectedThreat::OnInitialise(void)
{

}

//--------------------------------------------------------------------------------------
// Terminates the behaviour.
//--------------------------------------------------------------------------------------
void DetermineGreatestSuspectedThreat::OnTerminate(BehaviourStatus status)
{

}