/* 
*  Kevin Meergans, SquadAI, 2014
*  TeamReturnSpecificStatus.h
*  A team decorator that always returns the specific status code that was set on it
*  despite its actual state.
*/

#ifndef TEAM_RETURN_SPECIFIC_STATUS_H
#define TEAM_RETURN_SPECIFIC_STATUS_H

// Includes
#include "TeamDecorator.h"

// Forward declarations
class TeamAI;

//--------------------------------------------------------------------------------------
// Contains all additional data required for the creation of a parallel behaviour.
//--------------------------------------------------------------------------------------
struct TeamReturnSpecificStatusInitData
{
	TeamReturnSpecificStatusInitData(TeamBehaviour* pChild, BehaviourStatus returnStatus) : m_pChild(pChild),
																						    m_returnStatus(returnStatus)
	{}

	TeamBehaviour*  m_pChild;
	BehaviourStatus m_returnStatus;
};

class TeamReturnSpecificStatus : public TeamDecorator
{
public:
	TeamReturnSpecificStatus(const char* name, TeamAI* pTeamAI, TeamBehaviour* pChild, BehaviourStatus returnStatus);
	~TeamReturnSpecificStatus(void);

	void SetReturnStatus(BehaviourStatus returnStatus);

private:
	void OnInitialise(void);
	BehaviourStatus Update(float deltaTime);

	BehaviourStatus m_returnStatus;
};

#endif // TEAM_RETURN_SPECIFIC_STATUS_H