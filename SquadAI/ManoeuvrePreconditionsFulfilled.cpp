/* 
*  Kevin Meergans, SquadAI, 2014
*  ManoeuvrePreconditionsFulfilled.cpp
*  Team behaviour condition that checks whether all preconditions for the execution
*  of a certain manoeuvre are fulfilled. This mostly consists of checks of the game
*  state and availability of entities to execute the manoeuvre.
*/

// Includes
#include "ManoeuvrePreconditionsFulfilled.h"
#include "TeamAI.h"


ManoeuvrePreconditionsFulfilled::ManoeuvrePreconditionsFulfilled(const char* name, TeamAI* pTeamAI, TeamManoeuvreType manoeuvreType, float aggressiveness, float defensiveness)
	: TeamBehaviour(name, pTeamAI, aggressiveness, defensiveness),
	  m_manoeuvreType(manoeuvreType)
{
}

ManoeuvrePreconditionsFulfilled::~ManoeuvrePreconditionsFulfilled(void)
{
}

//--------------------------------------------------------------------------------------
// Updates the selector.
// Param1: The time in seconds passed since the last frame.
// Returns the state of the selector.
//--------------------------------------------------------------------------------------
BehaviourStatus ManoeuvrePreconditionsFulfilled::Update(float deltaTime)
{
	if(GetTeamAI()->ManoeuvrePreconditionsFulfilled(GetManoeuvreType()))
	{
		return StatusSuccess;
	}else
	{
		return StatusFailure;
	}
}

// Data access functions

TeamManoeuvreType ManoeuvrePreconditionsFulfilled::GetManoeuvreType(void) const
{
	return m_manoeuvreType;
}

void ManoeuvrePreconditionsFulfilled::SetManoeuvreType(TeamManoeuvreType manoeuvreType)
{
	m_manoeuvreType = manoeuvreType;
}


