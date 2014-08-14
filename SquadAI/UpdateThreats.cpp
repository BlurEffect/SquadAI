/* 
*  Kevin Meergans, SquadAI, 2014
*  UpdateThreats.cpp
*  An action behaviour that updates the threats to the entity.
*/

// Includes
#include "UpdateThreats.h"
#include "Entity.h"

UpdateThreats::UpdateThreats(const char* name, Entity* pEntity) : UniversalIndividualBehaviour(name, pEntity)
{
}

UpdateThreats::~UpdateThreats(void)
{
}

//--------------------------------------------------------------------------------------
// Updates the behaviour.
// Param1: The time in seconds passed since the last frame.
// Returns the state of the behaviour.
//--------------------------------------------------------------------------------------
BehaviourStatus UpdateThreats::Update(float deltaTime)
{
	BehaviourStatus result = GetEntity()->UpdateThreats(deltaTime);
	
	return result;
}
