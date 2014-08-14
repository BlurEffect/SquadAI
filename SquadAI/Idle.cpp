/* 
*  Kevin Meergans, SquadAI, 2014
*  Idle.cpp
*  An action behaviour that has the entity perform an idle behaviour.
*/

// Includes
#include "Idle.h"
#include "Entity.h"

Idle::Idle(const char* name, Entity* pEntity) : UniversalIndividualBehaviour(name, pEntity)
{
}

Idle::~Idle(void)
{
}

//--------------------------------------------------------------------------------------
// Updates the behaviour.
// Param1: The time in seconds passed since the last frame.
// Returns the state of the behaviour.
//--------------------------------------------------------------------------------------
BehaviourStatus Idle::Update(float deltaTime)
{
	return GetEntity()->Idle(deltaTime);
}
