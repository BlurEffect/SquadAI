/* 
*  Kevin Meergans, SquadAI, 2014
*  PathToTargetSet.cpp
*  A condition behaviour that checks whether a path to the movement target was set
*  for the entity or not.
*/

// Includes
#include "PathToTargetSet.h"
#include "Entity.h"

PathToTargetSet::PathToTargetSet(const char* name, Entity* pEntity) : UniversalIndividualBehaviour(name, pEntity)
{
}

PathToTargetSet::~PathToTargetSet(void)
{
}

//--------------------------------------------------------------------------------------
// Updates the behaviour.
// Param1: The time in seconds passed since the last frame.
// Returns the state of the behaviour.
//--------------------------------------------------------------------------------------
BehaviourStatus PathToTargetSet::Update(float deltaTime)
{
	if(GetEntity()->IsPathSet())
	{
		return StatusSuccess;
	}else
	{
		return StatusFailure;
	}
}
