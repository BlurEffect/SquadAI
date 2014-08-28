/* 
*  Kevin Meergans, SquadAI, 2014
*  TeamMonitor.h
*  Parallel team behaviour that ensures that a number of actions is executed only as long until a number
*  of conditions holds true or they succeed.
*/

#ifndef TEAM_MONITOR_H
#define TEAM_MONITOR_H

// Includes
#include "TeamParallel.h"

// Forward declarations
class TeamBehaviour;

class TeamMonitor : public TeamParallel
{
public:

	TeamMonitor(const char* name, TeamAI* pTeamAI);
	virtual ~TeamMonitor(void);

	void AddCondition(TeamBehaviour* pCondition);
	void AddAction(TeamBehaviour* pAction);

protected:
	//virtual BehaviourStatus Update(float deltaTime);
};

#endif // TEAM_MONITOR_H