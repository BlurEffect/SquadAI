/* 
*  Kevin Meergans, SquadAI, 2014
*  TeamAllAttack.h
*  Send all team members to attack a certain enemy. Mostly used as test behaviour.
*/

#ifndef TEAM_ALL_ATTACK_H
#define TEAM_ALL_ATTACK_H

// Includes
#include "MultiflagCTFTeamBehaviour.h"

class TeamAllAttack : public MultiflagCTFTeamBehaviour
{
public:
	TeamAllAttack(const char* name, MultiflagCTFTeamAI* pTeamAI);
	~TeamAllAttack(void);

private:
	BehaviourStatus Update(float deltaTime);

};

#endif // TEAM_ALL_ATTACK_H