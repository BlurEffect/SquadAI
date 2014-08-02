/* 
*  Kevin Meergans, SquadAI, 2014
*  DetermineGreatestThreats.cpp
*  An action behaviour that determines and sets the greatest known and suspected
*  threat for the entity.
*/

// Includes
#include "DetermineGreatestThreats.h"
#include "Entity.h"

DetermineGreatestThreats::DetermineGreatestThreats(Entity* pEntity, const char* name) : Behaviour(pEntity, name)
{
}

DetermineGreatestThreats::~DetermineGreatestThreats(void)
{
}

//--------------------------------------------------------------------------------------
// Updates the behaviour.
// Param1: The time in seconds passed since the last frame.
// Returns the state of the behaviour.
//--------------------------------------------------------------------------------------
BehaviourStatus DetermineGreatestThreats::Update(float deltaTime)
{
	return GetEntity()->DetermineGreatestThreats(deltaTime);
}

//--------------------------------------------------------------------------------------
// Initialises the behaviour.
//--------------------------------------------------------------------------------------
void DetermineGreatestThreats::OnInitialise(void)
{

}

//--------------------------------------------------------------------------------------
// Terminates the behaviour.
//--------------------------------------------------------------------------------------
void DetermineGreatestThreats::OnTerminate(BehaviourStatus status)
{

}