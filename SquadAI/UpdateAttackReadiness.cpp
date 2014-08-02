/* 
*  Kevin Meergans, SquadAI, 2014
*  UpdateAttackReadiness.cpp
*  An action behaviour that updates and sets the attack readiness for
*  the entity.
*/

// Includes
#include "UpdateAttackReadiness.h"
#include "Entity.h"

UpdateAttackReadiness::UpdateAttackReadiness(Entity* pEntity, const char* name) : Behaviour(pEntity, name)
{
}

UpdateAttackReadiness::~UpdateAttackReadiness(void)
{
}

//--------------------------------------------------------------------------------------
// Updates the behaviour.
// Param1: The time in seconds passed since the last frame.
// Returns the state of the behaviour.
//--------------------------------------------------------------------------------------
BehaviourStatus UpdateAttackReadiness::Update(float deltaTime)
{
	return GetEntity()->UpdateAttackReadiness(deltaTime);
}

//--------------------------------------------------------------------------------------
// Initialises the behaviour.
//--------------------------------------------------------------------------------------
void UpdateAttackReadiness::OnInitialise(void)
{

}

//--------------------------------------------------------------------------------------
// Terminates the behaviour.
//--------------------------------------------------------------------------------------
void UpdateAttackReadiness::OnTerminate(BehaviourStatus status)
{

}