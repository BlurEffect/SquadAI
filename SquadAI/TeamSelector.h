/* 
*  Kevin Meergans, SquadAI, 2014
*  TeamSelector.h
*  Composite behaviour that represents a conditional selection from a range of
*  available team behaviours. Starting with the first child behaviour, the selector 
*  checks its children fpr execution until one successfully completes. At that point
*  the selector succeeds as well. The selector fails when none of the children 
*  completes successfully.
*/

#ifndef TEAM_SELECTOR_H
#define TEAM_SELECTOR_H

// Includes
#include "TeamComposite.h"

// Forward declarations
class TeamAI;

class TeamSelector : public TeamComposite
{
public:
	TeamSelector(const char* name, TeamAI* pTeamAI);
	virtual ~TeamSelector(void);

	virtual void CalculateCharacteristicValues(void);

protected:
	virtual void OnInitialise(void);
	virtual BehaviourStatus Update(float deltaTime);

	std::vector<TeamBehaviour*>::iterator m_currentChild; // The currently active child behaviour of the sequence
};

#endif // TEAM_SELECTOR_H