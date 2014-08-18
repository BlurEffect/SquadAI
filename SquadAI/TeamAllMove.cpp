/* 
*  Kevin Meergans, SquadAI, 2014
*  TeamAllMove.cpp
*  Send all team members to move to a certain position. Mostly used as test behaviour.
*/

// Includes
#include "TeamAllMove.h"
#include "MultiflagCTFTeamAI.h"

TeamAllMove::TeamAllMove(const char* name, MultiflagCTFTeamAI* pTeamAI) : MultiflagCTFTeamBehaviour(name, pTeamAI)
{
}

TeamAllMove::~TeamAllMove(void)
{
}

//--------------------------------------------------------------------------------------
// Updates the behaviour.
// Param1: The time in seconds passed since the last frame.
// Returns the state of the behaviour.
//--------------------------------------------------------------------------------------
BehaviourStatus TeamAllMove::Update(float deltaTime)
{
	return GetMultiflagCTFTeamAI()->AllMove(deltaTime);
}
