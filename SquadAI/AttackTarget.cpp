/* 
*  Kevin Meergans, SquadAI, 2014
*  AttackTarget.cpp
*  An action behaviour that has the entity attack the current attack target
*  position.
*/

// Includes
#include "AttackTarget.h"
#include "Entity.h"

AttackTarget::AttackTarget(Entity* pEntity, const char* name) : Behaviour(pEntity, name)
{
}

AttackTarget::~AttackTarget(void)
{
}

//--------------------------------------------------------------------------------------
// Updates the behaviour.
// Param1: The time in seconds passed since the last frame.
// Returns the state of the behaviour.
//--------------------------------------------------------------------------------------
BehaviourStatus AttackTarget::Update(float deltaTime)
{
	return GetEntity()->Attack(deltaTime);
}

//--------------------------------------------------------------------------------------
// Initialises the behaviour.
//--------------------------------------------------------------------------------------
void AttackTarget::OnInitialise(void)
{

}

//--------------------------------------------------------------------------------------
// Terminates the behaviour.
//--------------------------------------------------------------------------------------
void AttackTarget::OnTerminate(BehaviourStatus status)
{

}