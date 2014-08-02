/* 
*  Kevin Meergans, SquadAI, 2014
*  UpdateThreats.cpp
*  An action behaviour that updates the threats to the entity.
*/

// Includes
#include "UpdateThreats.h"
#include "Entity.h"

UpdateThreats::UpdateThreats(Entity* pEntity, const char* name) : Behaviour(pEntity, name)
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

//--------------------------------------------------------------------------------------
// Initialises the behaviour.
//--------------------------------------------------------------------------------------
void UpdateThreats::OnInitialise(void)
{

}

//--------------------------------------------------------------------------------------
// Terminates the behaviour.
//--------------------------------------------------------------------------------------
void UpdateThreats::OnTerminate(BehaviourStatus status)
{

}