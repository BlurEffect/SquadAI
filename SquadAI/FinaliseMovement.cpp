/* 
*  Kevin Meergans, SquadAI, 2014
*  FinaliseMovement.cpp
*  An action behaviour that can be used to set the behaviour of an entity when it reaches
*  its movement target. Can also be used to reset movement data etc.
*/

// Includes
#include "FinaliseMovement.h"
#include "Entity.h"

FinaliseMovement::FinaliseMovement(const char* name, Entity* pEntity) : UniversalIndividualBehaviour(name, pEntity)
{
}

FinaliseMovement::~FinaliseMovement(void)
{
}

//--------------------------------------------------------------------------------------
// Updates the behaviour.
// Param1: The time in seconds passed since the last frame.
// Returns the state of the behaviour.
//--------------------------------------------------------------------------------------
BehaviourStatus FinaliseMovement::Update(float deltaTime)
{
	return GetEntity()->FinaliseMovement(deltaTime);
}

