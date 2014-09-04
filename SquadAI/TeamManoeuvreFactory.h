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
#include "RushBaseAttack.h"
#include "RunTheFlagHome.h"
#include "ReturnDroppedFlag.h"
#include "CoordinatedBaseAttack.h"
#include "DistractionBaseAttack.h"
#include "SimpleBaseAttack.h"
#include "PickUpDroppedFlag.h"
#include "SimpleBaseDefence.h"
#include "ActiveBaseDefence.h"
#include "GuardedFlagCapture.h"
#include "InterceptFlagCarrier.h"

// Forward declarations
class TeamAI;

class TeamManoeuvreFactory
{
public:
	static TeamManoeuvre* CreateTeamManoeuvre(TeamManoeuvreType type, unsigned int minNumberOfParticipants, unsigned int maxNumberOfParticipants, TeamAI* pTeamAI, void* pAdditionalData);
};

#endif // TEAM_MANOEUVRE_FACTORY_H
