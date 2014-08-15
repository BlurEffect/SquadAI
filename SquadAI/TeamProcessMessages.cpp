/* 
*  Kevin Meergans, SquadAI, 2014
*  TeamProcessMessages.h
*  An action behaviour that has the team AI process all queued up messages
*  it received since the last update.
*/

// Includes
#include "TeamProcessMessages.h"
#include "TeamAI.h"

TeamProcessMessages::TeamProcessMessages(const char* name, TeamAI* pTeamAI) : UniversalTeamBehaviour(name, pTeamAI)
{
}

TeamProcessMessages::~TeamProcessMessages(void)
{
}

//--------------------------------------------------------------------------------------
// Updates the behaviour.
// Param1: The time in seconds passed since the last frame.
// Returns the state of the behaviour.
//--------------------------------------------------------------------------------------
BehaviourStatus TeamProcessMessages::Update(float deltaTime)
{
	GetTeamAI()->ProcessMessages();
	GetTeamAI()->SortOutProcessedMessages();
	return StatusSuccess;
}
