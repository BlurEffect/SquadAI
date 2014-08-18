/* 
*  Kevin Meergans, SquadAI, 2014
*  TeamAllDefend.h
*  Send all team members to defend a certain position. Mostly used as test behaviour.
*/

#ifndef TEAM_ALL_DEFEND_H
#define TEAM_ALL_DEFEND_H

// Includes
#include "MultiflagCTFTeamBehaviour.h"

class TeamAllDefend : public MultiflagCTFTeamBehaviour
{
public:
	TeamAllDefend(const char* name, MultiflagCTFTeamAI* pTeamAI);
	~TeamAllDefend(void);

private:
	BehaviourStatus Update(float deltaTime);

};

#endif // TEAM_ALL_DEFEND_H