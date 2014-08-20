/* 
*  Kevin Meergans, SquadAI, 2014
*  TeamRepeat.h
*  A decorator that repeatedly executes its child team behaviour for a specified number of times.
*/

#ifndef TEAM_REPEAT_H
#define TEAM_REPEAT_H

// Includes
#include "TeamDecorator.h"

// Forward declarations
class TeamAI;

//--------------------------------------------------------------------------------------
// Contains all additional data required for the creation of a repeat team behaviour.
//--------------------------------------------------------------------------------------
struct TeamRepeatInitData
{
	TeamRepeatInitData(TeamBehaviour* pChild, unsigned int numberOfRepeats) : m_pChild(pChild),
																	          m_numberOfRepeats(numberOfRepeats)
	{}

	TeamBehaviour* m_pChild;
	unsigned int   m_numberOfRepeats;
};

class TeamRepeat : public TeamDecorator
{
public:
	TeamRepeat(const char* name, TeamAI* pTeamAI, TeamBehaviour* pChild, unsigned int numberOfRepeats);
	~TeamRepeat(void);

	void SetNumberOfRepeats(unsigned int count);

private:
	void OnInitialise(void);
	BehaviourStatus Update(float deltaTime);

	unsigned int m_numberOfRepeats;
	unsigned int m_counter;
};

#endif // TEAM_REPEAT_H