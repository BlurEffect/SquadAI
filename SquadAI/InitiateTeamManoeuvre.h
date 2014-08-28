/* 
*  Kevin Meergans, SquadAI, 2014
*  InitiateTeamManoeuvre.h
*  Team behaviour that prepares the execution of a team manoeuvre by the associated
*  team AI.
*/

#ifndef INITIATE_TEAM_MANOEUVRE_H
#define INITIATE_TEAM_MANOEUVRE_H

// Includes
#include "TeamBehaviour.h"

// Forward declarations
class TeamAI;

//--------------------------------------------------------------------------------------
// Contains all additional data required for the creation of an initiate manoeuvre behaviour.
//--------------------------------------------------------------------------------------
struct InitiateTeamManoeuvreInitData
{
	InitiateTeamManoeuvreInitData(TeamManoeuvreType manoeuvreType) 
		: m_manoeuvreType(manoeuvreType)
	{}

	TeamManoeuvreType m_manoeuvreType;  // The manoeuvre that should be prepared by the team AI when the behaviour becomes active
};


class InitiateTeamManoeuvre : public TeamBehaviour
{
public:
	InitiateTeamManoeuvre(const char* name, TeamAI* pTeamAI, TeamManoeuvreType manoeuvreType, float aggressiveness, float defensiveness);
	~InitiateTeamManoeuvre(void);

	// Data access functions

	TeamManoeuvreType GetManoeuvreType(void) const;
	void SetManoeuvreType(TeamManoeuvreType manoeuvreType);

private:

	BehaviourStatus Update(float deltaTime);

	TeamManoeuvreType m_manoeuvreType; // The manoeuvre associated to this behaviour

};

#endif // INITIATE_TEAM_MANOEUVRE_H