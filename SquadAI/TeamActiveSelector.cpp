/* 
*  Kevin Meergans, SquadAI, 2014
*  TeamActiveSelector.cpp
*  Specialisation of the team selector composite that aborts a running child behaviour if another
*  with a higher priority becomes executable.
*/

// Includes
#include "TeamActiveSelector.h"
#include "TeamAI.h"

TeamActiveSelector::TeamActiveSelector(const char* name, TeamAI* pTeamAI) : TeamSelector(name, pTeamAI)
{

}

TeamActiveSelector::~TeamActiveSelector(void)
{

}

//--------------------------------------------------------------------------------------
// Initialises the active selector.
//--------------------------------------------------------------------------------------
void TeamActiveSelector::OnInitialise(void)
{
	m_currentChild = m_children.end();
}

//--------------------------------------------------------------------------------------
// Updates the active selector.
// Param1: The time in seconds passed since the last frame.
// Returns the state of the active selector.
//--------------------------------------------------------------------------------------
BehaviourStatus TeamActiveSelector::Update(float deltaTime)
{
	std::vector<TeamBehaviour*>::iterator previous = m_currentChild;

	// Run the normal selector behaviour to determine the highest priority
	// child that is currently executable.
    TeamSelector::OnInitialise();
    BehaviourStatus result = TeamSelector::Update(deltaTime);

    if(previous != m_children.end() && m_currentChild != previous)
    {
		// If a higher priority child behaviour is able to run, abort the previous one.
		(*previous)->Abort();
    }
    return result;
}

