/* 
*  Kevin Meergans, SquadAI, 2014
*  AttackTargetSet.cpp
*  A condition behaviour that checks whether a target to attack was set
*  for the entity or not.
*/

// Includes
#include "AttackTargetSet.h"
#include "Entity.h"

AttackTargetSet::AttackTargetSet(Entity* pEntity, const char* name) : Behaviour(pEntity, name)
{
}

AttackTargetSet::~AttackTargetSet(void)
{
}

//--------------------------------------------------------------------------------------
// Updates the behaviour.
// Param1: The time in seconds passed since the last frame.
// Returns the state of the behaviour.
//--------------------------------------------------------------------------------------
BehaviourStatus AttackTargetSet::Update(float deltaTime)
{
	if(GetEntity()->IsAttackTargetSet())
	{
		return StatusSuccess;
	}else
	{
		return StatusFailure;
	}
}
