/* 
*  Kevin Meergans, SquadAI, 2014
*  DetermineGreatestKnownThreat.cpp
*  An action behaviour that determines and sets the greatest known
*  threat for the entity.
*/

// Includes
#include "DetermineGreatestKnownThreat.h"
#include "Entity.h"

DetermineGreatestKnownThreat::DetermineGreatestKnownThreat(Entity* pEntity, const char* name) : Behaviour(pEntity, name)
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

//--------------------------------------------------------------------------------------
// Initialises the behaviour.
//--------------------------------------------------------------------------------------
void DetermineGreatestKnownThreat::OnInitialise(void)
{

}

//--------------------------------------------------------------------------------------
// Terminates the behaviour.
//--------------------------------------------------------------------------------------
void DetermineGreatestKnownThreat::OnTerminate(BehaviourStatus status)
{

}