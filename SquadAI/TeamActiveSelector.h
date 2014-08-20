/* 
*  Kevin Meergans, SquadAI, 2014
*  TeamActiveSelector.h
*  Specialisation of the team selector composite that aborts a running child behaviour if another
*  with a higher priority becomes executable.
*/

#ifndef TEAM_ACTIVE_SELECTOR_H
#define TEAM_ACTIVE_SELECTOR_H

// Includes
#include "TeamSelector.h"

// Forward declarations
class TeamAI;

class TeamActiveSelector : public TeamSelector
{
public:
	TeamActiveSelector(const char* name, TeamAI* pTeamAI);
	virtual ~TeamActiveSelector(void);

protected:
	virtual void OnInitialise(void);
	virtual BehaviourStatus Update(float deltaTime);

};

#endif // TEAM_ACTIVE_SELECTOR_H