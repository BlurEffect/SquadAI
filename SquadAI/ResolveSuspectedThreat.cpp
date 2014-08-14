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

ResolveSuspectedThreat::ResolveSuspectedThreat(const char* name, Entity* pEntity) : UniversalIndividualBehaviour(name, pEntity)
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
