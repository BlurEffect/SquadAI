/* 
*  Kevin Meergans, SquadAI, 2014
*  TeamSequence.h
*  Composite behaviour that represents a sequence of team behaviours, which are
*  executed in order as long as each of them succeeds. If one of the child behaviours
*  fails, the sequence fails as well. The sequence succeeds if all children are
*  executed successfully.
*/

#ifndef TEAM_SEQUENCE_H
#define TEAM_SEQUENCE_H

// Includes
#include "TeamComposite.h"

// Forward declarations
class TeamAI;

class TeamSequence : public TeamComposite
{
public:
	TeamSequence(const char* name, TeamAI* pTeamAI);
	virtual ~TeamSequence(void);

	virtual void CalculateCharacteristicValues(void);

protected:
	virtual void OnInitialise(void);
	virtual BehaviourStatus Update(float deltaTime);

	std::vector<TeamBehaviour*>::iterator m_currentChild; // The currently active child behaviour of the sequence
};

#endif // TEAM_SEQUENCE_H