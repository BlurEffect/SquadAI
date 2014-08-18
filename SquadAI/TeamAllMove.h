/* 
*  Kevin Meergans, SquadAI, 2014
*  TeamAllMove.h
*  Send all team members to move to a certain position. Mostly used as test behaviour.
*/

#ifndef TEAM_ALL_MOVE_H
#define TEAM_ALL_MOVE_H

// Includes
#include "MultiflagCTFTeamBehaviour.h"

class TeamAllMove : public MultiflagCTFTeamBehaviour
{
public:
	TeamAllMove(const char* name, MultiflagCTFTeamAI* pTeamAI);
	~TeamAllMove(void);

private:
	BehaviourStatus Update(float deltaTime);

};

#endif // TEAM_ALL_MOVE_H