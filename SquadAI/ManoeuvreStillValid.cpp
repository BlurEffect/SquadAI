/* 
*  Kevin Meergans, SquadAI, 2014
*  ManoeuvreStillValid.cpp
*  Team behaviour condition that checks whether a certain manoeuvre is still valid
*  that is whether certain conditions are still fulfilled.
*/

// Includes
#include "ManoeuvreStillValid.h"
#include "TeamAI.h"


ManoeuvreStillValid::ManoeuvreStillValid(const char* name, TeamAI* pTeamAI, TeamManoeuvreType manoeuvreType, float aggressiveness, float defensiveness)
	: TeamBehaviour(name, pTeamAI, aggressiveness, defensiveness),
	  m_manoeuvreType(manoeuvreType)
{
}

ManoeuvreStillValid::~ManoeuvreStillValid(void)
{
}

//--------------------------------------------------------------------------------------
// Updates the selector.
// Param1: The time in seconds passed since the last frame.
// Returns the state of the selector.
//--------------------------------------------------------------------------------------
BehaviourStatus ManoeuvreStillValid::Update(float deltaTime)
{
	if(GetTeamAI()->ManoeuvreStillValid(GetManoeuvreType()))
	{
		return StatusSuccess;
	}else
	{
		return StatusFailure;
	}
}

// Data access functions

TeamManoeuvreType ManoeuvreStillValid::GetManoeuvreType(void) const
{
	return m_manoeuvreType;
}

void ManoeuvreStillValid::SetManoeuvreType(TeamManoeuvreType manoeuvreType)
{
	m_manoeuvreType = manoeuvreType;
}


