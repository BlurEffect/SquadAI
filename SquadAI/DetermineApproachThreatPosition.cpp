/* 
*  Kevin Meergans, SquadAI, 2014
*  DetermineApproachThreatPosition.cpp
*  An action behaviour that determines a target position for the entity to move
*  towards in order to investigate a threat.
*/

// Includes
#include "DetermineApproachThreatPosition.h"
#include "Entity.h"

DetermineApproachThreatPosition::DetermineApproachThreatPosition(const char* name, Entity* pEntity) : UniversalIndividualBehaviour(name, pEntity)
{
}

DetermineApproachThreatPosition::~DetermineApproachThreatPosition(void)
{
}

//--------------------------------------------------------------------------------------
// Updates the behaviour.
// Param1: The time in seconds passed since the last frame.
// Returns the state of the behaviour.
//--------------------------------------------------------------------------------------
BehaviourStatus DetermineApproachThreatPosition::Update(float deltaTime)
{
	return GetEntity()->DetermineApproachThreatTarget(deltaTime);
}
