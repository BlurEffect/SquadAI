/* 
*  Kevin Meergans, SquadAI, 2014
*  TeamAllAttack.cpp
*  Send all team members to attack a certain enemy. Mostly used as test behaviour.
*/

// Includes
#include "TeamAllAttack.h"
#include "MultiflagCTFTeamAI.h"

TeamAllAttack::TeamAllAttack(const char* name, MultiflagCTFTeamAI* pTeamAI) : MultiflagCTFTeamBehaviour(name, pTeamAI)
{
}

TeamAllAttack::~TeamAllAttack(void)
{
}

//--------------------------------------------------------------------------------------
// Updates the behaviour.
// Param1: The time in seconds passed since the last frame.
// Returns the state of the behaviour.
//--------------------------------------------------------------------------------------
BehaviourStatus TeamAllAttack::Update(float deltaTime)
{
	return GetMultiflagCTFTeamAI()->AllAttack(deltaTime);
}
