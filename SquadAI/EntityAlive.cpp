/* 
*  Kevin Meergans, SquadAI, 2014
*  EntityAlive.h
*  A condition behaviour that checks whether the entity is alive or dead.
*/

// Includes
#include "EntityAlive.h"
#include "Entity.h"

EntityAlive::EntityAlive(const char* name, Entity* pEntity) : UniversalIndividualBehaviour(name, pEntity)
{
}

EntityAlive::~EntityAlive(void)
{
}

//--------------------------------------------------------------------------------------
// Updates the behaviour.
// Param1: The time in seconds passed since the last frame.
// Returns the state of the behaviour.
//--------------------------------------------------------------------------------------
BehaviourStatus EntityAlive::Update(float deltaTime)
{
	if(GetEntity()->IsAlive())
	{
		return StatusSuccess;
	}else
	{
		return StatusFailure;
	}
}
