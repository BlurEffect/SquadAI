/* 
*  Kevin Meergans, SquadAI, 2014
*  MoveToTarget.cpp
*  An action behaviour that has the entity move to the current movement
*  target position.
*/

// Includes
#include "MoveToTarget.h"
#include "Entity.h"

MoveToTarget::MoveToTarget(const char* name, Entity* pEntity) : UniversalIndividualBehaviour(name, pEntity)
{
}

MoveToTarget::~MoveToTarget(void)
{
}

//--------------------------------------------------------------------------------------
// Updates the behaviour.
// Param1: The time in seconds passed since the last frame.
// Returns the state of the behaviour.
//--------------------------------------------------------------------------------------
BehaviourStatus MoveToTarget::Update(float deltaTime)
{
	return GetEntity()->MoveToTarget(deltaTime);
}

//--------------------------------------------------------------------------------------
// Terminates the behaviour.
//--------------------------------------------------------------------------------------
void MoveToTarget::OnTerminate(BehaviourStatus status)
{
	GetEntity()->SetMovementTargetSet(false);
	Behaviour::OnTerminate(status);
}