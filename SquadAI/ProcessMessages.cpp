/* 
*  Kevin Meergans, SquadAI, 2014
*  ProcessMessages.cpp
*  An action behaviour that has the entity process all queued up messages
*  it received since the last update.
*/

// Includes
#include "ProcessMessages.h"
#include "Entity.h"

ProcessMessages::ProcessMessages(const char* name, Entity* pEntity) : UniversalIndividualBehaviour(name, pEntity)
{
}

ProcessMessages::~ProcessMessages(void)
{
}

//--------------------------------------------------------------------------------------
// Updates the behaviour.
// Param1: The time in seconds passed since the last frame.
// Returns the state of the behaviour.
//--------------------------------------------------------------------------------------
BehaviourStatus ProcessMessages::Update(float deltaTime)
{
	GetEntity()->ProcessMessages();
	GetEntity()->SortOutProcessedMessages();
	return StatusSuccess;
}
