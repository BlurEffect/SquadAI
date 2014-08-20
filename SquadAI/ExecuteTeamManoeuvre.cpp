/* 
*  Kevin Meergans, SquadAI, 2014
*  ExecuteTeamManoeuvre.cpp
*  Team behaviour that leads to the execution of a certain manoeuvre of the associated
*  team AI.
*/

// Includes
#include "ExecuteTeamManoeuvre.h"
#include "TeamAI.h"


ExecuteTeamManoeuvre::ExecuteTeamManoeuvre(const char* name, TeamAI* pTeamAI, TeamManoeuvreType manoeuvreType, float aggressiveness, float defensiveness)
	: TeamBehaviour(name, pTeamAI, aggressiveness, defensiveness),
	  m_manoeuvreType(manoeuvreType)
{
}

ExecuteTeamManoeuvre::~ExecuteTeamManoeuvre(void)
{
}

//--------------------------------------------------------------------------------------
// Initialises the behaviour.
//--------------------------------------------------------------------------------------
void ExecuteTeamManoeuvre::OnInitialise(void)
{
	TeamBehaviour::OnInitialise();

	GetTeamAI()->InitiateManoeuvre(GetManoeuvreType());
}

//--------------------------------------------------------------------------------------
// Updates the selector.
// Param1: The time in seconds passed since the last frame.
// Returns the state of the selector.
//--------------------------------------------------------------------------------------
BehaviourStatus ExecuteTeamManoeuvre::Update(float deltaTime)
{
	return GetTeamAI()->UpdateManoeuvre(GetManoeuvreType(), deltaTime);
}

//--------------------------------------------------------------------------------------
// Terminates the behaviour.
// Param1: The status of the behaviour on termination.
//--------------------------------------------------------------------------------------
void ExecuteTeamManoeuvre::OnTerminate(BehaviourStatus status)
{
	TeamBehaviour::OnTerminate(status);

	GetTeamAI()->TerminateManoeuvre(GetManoeuvreType());
}

// Data access functions

TeamManoeuvreType ExecuteTeamManoeuvre::GetManoeuvreType(void) const
{
	return m_manoeuvreType;
}

void ExecuteTeamManoeuvre::SetManoeuvreType(TeamManoeuvreType manoeuvreType)
{
	m_manoeuvreType = manoeuvreType;
}


