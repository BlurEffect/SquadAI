/* 
*  Kevin Meergans, SquadAI, 2014
*  LookAtTarget.cpp
*  An action behaviour that has the entity look at the currently set observation target position.
*/

// Includes
#include "LookAtTarget.h"
#include "Entity.h"

LookAtTarget::LookAtTarget(const char* name, Entity* pEntity) : UniversalIndividualBehaviour(name, pEntity)
{
}

LookAtTarget::~LookAtTarget(void)
{
}

//--------------------------------------------------------------------------------------
// Updates the behaviour.
// Param1: The time in seconds passed since the last frame.
// Returns the state of the behaviour.
//--------------------------------------------------------------------------------------
BehaviourStatus LookAtTarget::Update(float deltaTime)
{
	return GetEntity()->LookAtTarget(deltaTime);
}
