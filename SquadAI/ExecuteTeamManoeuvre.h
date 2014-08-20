/* 
*  Kevin Meergans, SquadAI, 2014
*  ExecuteTeamManoeuvre.h
*  Team behaviour that leads to the execution of a certain manoeuvre of the associated
*  team AI.
*/

#ifndef EXECUTE_TEAM_MANOEUVRE_H
#define EXECUTE_TEAM_MANOEUVRE_H

// Includes
#include "TeamBehaviour.h"

// Forward declarations
class TeamAI;

//--------------------------------------------------------------------------------------
// Contains all additional data required for the creation of an execute manoeuvre behaviour.
//--------------------------------------------------------------------------------------
struct ExecuteTeamManoeuvreInitData
{
	ExecuteTeamManoeuvreInitData(TeamManoeuvreType manoeuvreType) 
		: m_manoeuvreType(manoeuvreType)
	{}

	TeamManoeuvreType m_manoeuvreType;  // The manoeuvre that should be executed by the team AI when the behaviour becomes active
};


class ExecuteTeamManoeuvre : public TeamBehaviour
{
public:
	ExecuteTeamManoeuvre(const char* name, TeamAI* pTeamAI, TeamManoeuvreType manoeuvreType, float aggressiveness, float defensiveness);
	~ExecuteTeamManoeuvre(void);

	// Data access functions

	TeamManoeuvreType GetManoeuvreType(void) const;
	void SetManoeuvreType(TeamManoeuvreType manoeuvreType);

private:
	void		    OnInitialise(void);
	BehaviourStatus Update(float deltaTime);
	void			OnTerminate(BehaviourStatus status);

	TeamManoeuvreType m_manoeuvreType; // The manoeuvre associated to this behaviour

};

#endif // EXECUTE_TEAM_MANOEUVRE_H