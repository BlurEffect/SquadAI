/* 
*  Kevin Meergans, SquadAI, 2014
*  ReadyToAttack.h
*  A condition behaviour that checks whether the entity is ready for attack.
*/

// Includes
#include "ReadyToAttack.h"
#include "Entity.h"

ReadyToAttack::ReadyToAttack(Entity* pEntity, const char* name) : Behaviour(pEntity, name)
{
}

ReadyToAttack::~ReadyToAttack(void)
{
}

//--------------------------------------------------------------------------------------
// Updates the behaviour.
// Param1: The time in seconds passed since the last frame.
// Returns the state of the behaviour.
//--------------------------------------------------------------------------------------
BehaviourStatus ReadyToAttack::Update(float deltaTime)
{
	if(GetEntity()->IsReadyForAttack())
	{
		return StatusSuccess;
	}else
	{
		return StatusFailure;
	}
}

//--------------------------------------------------------------------------------------
// Initialises the behaviour.
//--------------------------------------------------------------------------------------
void ReadyToAttack::OnInitialise(void)
{

}

//--------------------------------------------------------------------------------------
// Terminates the behaviour.
//--------------------------------------------------------------------------------------
void ReadyToAttack::OnTerminate(BehaviourStatus status)
{

}