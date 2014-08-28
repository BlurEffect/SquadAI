/* 
*  Kevin Meergans, SquadAI, 2014
*  InitiateTeamManoeuvre.cpp
*  Team behaviour that prepares the execution of a team manoeuvre by the associated
*  team AI.
*/

// Includes
#include "InitiateTeamManoeuvre.h"
#include "TeamAI.h"


InitiateTeamManoeuvre::InitiateTeamManoeuvre(const char* name, TeamAI* pTeamAI, TeamManoeuvreType manoeuvreType, float aggressiveness, float defensiveness)
	: TeamBehaviour(name, pTeamAI, aggressiveness, defensiveness),
	  m_manoeuvreType(manoeuvreType)
{
}

InitiateTeamManoeuvre::~InitiateTeamManoeuvre(void)
{
}

//--------------------------------------------------------------------------------------
// Updates the behaviour.
// Param1: The time in seconds passed since the last frame.
// Returns the state of the behaviour.
//--------------------------------------------------------------------------------------
BehaviourStatus InitiateTeamManoeuvre::Update(float deltaTime)
{
	return GetTeamAI()->InitiateManoeuvre(GetManoeuvreType());
}

// Data access functions

TeamManoeuvreType InitiateTeamManoeuvre::GetManoeuvreType(void) const
{
	return m_manoeuvreType;
}

void InitiateTeamManoeuvre::SetManoeuvreType(TeamManoeuvreType manoeuvreType)
{
	m_manoeuvreType = manoeuvreType;
}


