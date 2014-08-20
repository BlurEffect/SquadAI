/* 
*  Kevin Meergans, SquadAI, 2014
*  TeamDecorator.h
*  Abstract base class for team decorator nodes of the behaviour tree.
*  Each decorator has a single child team behaviour, which it decorates that is it might
*  modify the return code of the child control how often it is run or other actions that
*  however do not directly modify the behaviour of the child.
*/

#ifndef TEAM_DECORATOR_H
#define TEAM_DECORATOR_H

// Includes
#include "TeamBehaviour.h"

// Forward declarations
class TeamAI;

class TeamDecorator : public TeamBehaviour
{
public:
	TeamDecorator(const char* name, TeamAI* pTeamAI, TeamBehaviour* pChild);
	virtual ~TeamDecorator(void) = 0;

	virtual void CalculateCharacteristicValues(void);

protected:
	virtual void OnTerminate(BehaviourStatus status);

	TeamBehaviour* m_pChild; // The single child behaviour that is being decorated
};

#endif // TEAM_DECORATOR_H