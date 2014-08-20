/* 
*  Kevin Meergans, SquadAI, 2014
*  TeamProcessMessages.h
*  An action behaviour that has the team AI process all queued up messages
*  it received since the last update.
*/

#ifndef TEAM_PROCESS_MESSAGES_H
#define TEAM_PROCESS_MESSAGES_H

// Includes
#include "TeamBehaviour.h"

// Forward declarations
class TeamAI;

class TeamProcessMessages : public TeamBehaviour
{
public:
	TeamProcessMessages(const char* name, TeamAI* pTeamAI, float aggressiveness, float defensiveness);
	~TeamProcessMessages(void);

private:
	BehaviourStatus Update(float deltaTime);
};

#endif // TEAM_PROCESS_MESSAGES_H