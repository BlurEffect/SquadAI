/* 
*  Kevin Meergans, SquadAI, 2014
*  TeamReturnSpecificStatus.cpp
*  A team decorator that always returns the specific status code that was set on it
*  despite its actual state.
*/

// Includes
#include "TeamReturnSpecificStatus.h"
#include "TeamAI.h"

TeamReturnSpecificStatus::TeamReturnSpecificStatus(const char* name, TeamAI* pTeamAI, TeamBehaviour* pChild, BehaviourStatus returnStatus) 
						: TeamDecorator(name, pTeamAI, pChild),
						  m_returnStatus(returnStatus)
{
}

TeamReturnSpecificStatus::~TeamReturnSpecificStatus(void)
{
}

//--------------------------------------------------------------------------------------
// Initialises the return specific status decorator.
//--------------------------------------------------------------------------------------
void TeamReturnSpecificStatus::OnInitialise(void)
{
	SetReturnStatus(m_returnStatus);
}

//--------------------------------------------------------------------------------------
// Updates the return specific status decorator.
// Param1: The time in seconds passed since the last frame.
// Returns the state of the decorator.
//--------------------------------------------------------------------------------------
BehaviourStatus TeamReturnSpecificStatus::Update(float deltaTime)
{
	if(!m_pChild)
	{
		// If the child was not initialised properly, indicate a problem by returning invalid
		return StatusInvalid;
	}

	BehaviourStatus status = m_pChild->Tick(deltaTime);
	return m_returnStatus;
}

void TeamReturnSpecificStatus::SetReturnStatus(BehaviourStatus returnStatus)
{
	m_returnStatus = returnStatus;
}



