/* 
*  Kevin Meergans, SquadAI, 2014
*  MoveToTarget.cpp
*  An action behaviour that has the entity move to the current movement
*  target position.
*/

// Includes
#include "MoveToTarget.h"
#include "Entity.h"

MoveToTarget::MoveToTarget(Entity* pEntity, const char* name) : Behaviour(pEntity, name)
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
	if(GetEntity()->GetTeam() == TeamRed)
	{
		int a  = 2;
	}else
	{
		int b = 3;
	}

	return GetEntity()->MoveToTarget(deltaTime);
}

//--------------------------------------------------------------------------------------
// Initialises the behaviour.
//--------------------------------------------------------------------------------------
void MoveToTarget::OnInitialise(void)
{

}

//--------------------------------------------------------------------------------------
// Terminates the behaviour.
//--------------------------------------------------------------------------------------
void MoveToTarget::OnTerminate(BehaviourStatus status)
{
	if(status == StatusSuccess || status == StatusAborted)
	{
		GetEntity()->SetMovementTargetSet(false);
	}

	Reset();
}