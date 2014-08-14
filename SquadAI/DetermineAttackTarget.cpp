/* 
*  Kevin Meergans, SquadAI, 2014
*  DetermineAttackTarget.cpp
*  An action behaviour that determines the position that the entity should
*  attack.
*/

// Includes
#include "DetermineAttackTarget.h"
#include "Entity.h"

DetermineAttackTarget::DetermineAttackTarget(const char* name, Entity* pEntity) : UniversalIndividualBehaviour(name, pEntity)
{
}

DetermineAttackTarget::~DetermineAttackTarget(void)
{
}

//--------------------------------------------------------------------------------------
// Updates the behaviour.
// Param1: The time in seconds passed since the last frame.
// Returns the state of the behaviour.
//--------------------------------------------------------------------------------------
BehaviourStatus DetermineAttackTarget::Update(float deltaTime)
{
	return GetEntity()->DetermineAttackTargetPosition(deltaTime);
}
