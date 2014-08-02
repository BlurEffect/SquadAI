/* 
*  Kevin Meergans, SquadAI, 2014
*  AimAtTarget.cpp
*  An action behaviour that has the entity aim at the currently set attack target position.
*/

// Includes
#include "AimAtTarget.h"
#include "Entity.h"

AimAtTarget::AimAtTarget(Entity* pEntity, const char* name) : Behaviour(pEntity, name)
{
}

AimAtTarget::~AimAtTarget(void)
{
}

//--------------------------------------------------------------------------------------
// Updates the behaviour.
// Param1: The time in seconds passed since the last frame.
// Returns the state of the behaviour.
//--------------------------------------------------------------------------------------
BehaviourStatus AimAtTarget::Update(float deltaTime)
{
	return GetEntity()->AimAt(deltaTime);
}

//--------------------------------------------------------------------------------------
// Initialises the behaviour.
//--------------------------------------------------------------------------------------
void AimAtTarget::OnInitialise(void)
{

}

//--------------------------------------------------------------------------------------
// Terminates the behaviour.
//--------------------------------------------------------------------------------------
void AimAtTarget::OnTerminate(BehaviourStatus status)
{

}