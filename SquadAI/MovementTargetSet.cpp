/* 
*  Kevin Meergans, SquadAI, 2014
*  MovementTargetSet.cpp
*  An condition behaviour that checks whether a movement target was set
*  for the entity or not.
*/

// Includes
#include "MovementTargetSet.h"
#include "Entity.h"

MovementTargetSet::MovementTargetSet(const char* name, Entity* pEntity) : UniversalIndividualBehaviour(name, pEntity)
{
}

MovementTargetSet::~MovementTargetSet(void)
{
}

//--------------------------------------------------------------------------------------
// Updates the behaviour.
// Param1: The time in seconds passed since the last frame.
// Returns the state of the behaviour.
//--------------------------------------------------------------------------------------
BehaviourStatus MovementTargetSet::Update(float deltaTime)
{
	if(GetEntity()->IsMovementTargetSet())
	{
		return StatusSuccess;
	}else
	{
		return StatusFailure;
	}
}
