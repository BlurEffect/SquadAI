/* 
*  Kevin Meergans, SquadAI, 2014
*  TeamManoeuvreFactory.h
*  Class with a static member function used to create team manoeuvres
*  for use by team AIs.
*/

#ifndef TEAM_MANOEUVRE_FACTORY_H
#define TEAM_MANOEUVRE_FACTORY_H

// Includes
#include "TeamManoeuvre.h"
#include "TestManoeuvre.h"
#include "DefendBaseEntrances.h"

// Forward declarations
class TeamAI;

class TeamManoeuvreFactory
{
public:
	static TeamManoeuvre* CreateTeamManoeuvre(TeamManoeuvreType type, unsigned int minNumberOfParticipants, unsigned int maxNumberOfParticipants, TeamAI* pTeamAI, void* pAdditionalData);
};

#endif // TEAM_MANOEUVRE_FACTORY_H
