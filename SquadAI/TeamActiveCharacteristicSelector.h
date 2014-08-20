/* 
*  Kevin Meergans, SquadAI, 2014
*  TeamActiveCharacteristicSelector.h
*  Specialisation of the active selector composite that sorts its children based on the 
*  characteristic of the associated team AI and thus changes the priority of the 
*  child behaviours.
*/

#ifndef TEAM_ACTIVE_CHARACTERISTICS_SELECTOR_H
#define TEAM_ACTIVE_CHARACTERISTICS_SELECTOR_H

// Includes
#include "TeamActiveSelector.h"

// Forward declarations
class TeamAI;

class TeamActiveCharacteristicSelector : public TeamActiveSelector
{
public:
	TeamActiveCharacteristicSelector(const char* name, TeamAI* pTeamAI);
	virtual ~TeamActiveCharacteristicSelector(void);

protected:
	virtual void OnInitialise(void);

};

#endif // ACTIVE_SELECTOR_H