/* 
*  Kevin Meergans, SquadAI, 2014
*  MovingToHighestPriorityTarget.cpp
*  A condition behaviour that checks whether the entity is still moving towards the highest
*  priority movement target or if there is a new one available with higher priority that the
*  entity should move towards instead.
*/

// Includes
#include "MovingToHighestPriorityTarget.h"
#include "Entity.h"

MovingToHighestPriorityTarget::MovingToHighestPriorityTarget(const char* name, Entity* pEntity) : UniversalIndividualBehaviour(name, pEntity)
{
}

MovingToHighestPriorityTarget::~MovingToHighestPriorityTarget(void)
{
}

//--------------------------------------------------------------------------------------
// Updates the behaviour.
// Param1: The time in seconds passed since the last frame.
// Returns the state of the behaviour.
//--------------------------------------------------------------------------------------
BehaviourStatus MovingToHighestPriorityTarget::Update(float deltaTime)
{
	if(GetEntity()->IsMovingToHighestPriorityTarget())
	{
		return StatusSuccess;
	}else
	{
		return StatusFailure;
	}
}
