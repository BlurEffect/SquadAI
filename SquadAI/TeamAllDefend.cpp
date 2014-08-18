/* 
*  Kevin Meergans, SquadAI, 2014
*  TeamAllDefend.cpp
*  Send all team members to defend a certain position. Mostly used as test behaviour.
*/

// Includes
#include "TeamAllDefend.h"
#include "MultiflagCTFTeamAI.h"

TeamAllDefend::TeamAllDefend(const char* name, MultiflagCTFTeamAI* pTeamAI) : MultiflagCTFTeamBehaviour(name, pTeamAI)
{
}

TeamAllDefend::~TeamAllDefend(void)
{
}

//--------------------------------------------------------------------------------------
// Updates the behaviour.
// Param1: The time in seconds passed since the last frame.
// Returns the state of the behaviour.
//--------------------------------------------------------------------------------------
BehaviourStatus TeamAllDefend::Update(float deltaTime)
{
	return GetMultiflagCTFTeamAI()->AllDefend(deltaTime);
}
