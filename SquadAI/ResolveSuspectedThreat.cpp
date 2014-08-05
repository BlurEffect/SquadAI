/* 
*  Kevin Meergans, SquadAI, 2014
*  ResolveSuspectedThreat.cpp
*  An action behaviour that removes a suspected threat from the entity's
*  memory. Should be used after investigating the last known position of a 
*  threat without finding it.
*/

// Includes
#include "ResolveSuspectedThreat.h"
#include "Entity.h"

ResolveSuspectedThreat::ResolveSuspectedThreat(Entity* pEntity, const char* name) : Behaviour(pEntity, name)
{
}

ResolveSuspectedThreat::~ResolveSuspectedThreat(void)
{
}

//--------------------------------------------------------------------------------------
// Updates the behaviour.
// Param1: The time in seconds passed since the last frame.
// Returns the state of the behaviour.
//--------------------------------------------------------------------------------------
BehaviourStatus ResolveSuspectedThreat::Update(float deltaTime)
{
	return GetEntity()->ResolveSuspectedThreat(deltaTime);
}

//--------------------------------------------------------------------------------------
// Initialises the behaviour.
//--------------------------------------------------------------------------------------
void ResolveSuspectedThreat::OnInitialise(void)
{

}

//--------------------------------------------------------------------------------------
// Terminates the behaviour.
//--------------------------------------------------------------------------------------
void ResolveSuspectedThreat::OnTerminate(BehaviourStatus status)
{

}