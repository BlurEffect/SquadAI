/* 
*  Kevin Meergans, SquadAI, 2014
*  TeamProcessMessages.h
*  An action behaviour that has the team AI process all queued up messages
*  it received since the last update.
*/

#ifndef TEAM_PROCESS_MESSAGES_H
#define TEAM_PROCESS_MESSAGES_H

// Includes
#include "UniversalTeamBehaviour.h"

class TeamProcessMessages : public UniversalTeamBehaviour
{
public:
	TeamProcessMessages(const char* name, TeamAI* pTeamAI);
	~TeamProcessMessages(void);

private:
	BehaviourStatus Update(float deltaTime);
};

#endif // TEAM_PROCESS_MESSAGES_H