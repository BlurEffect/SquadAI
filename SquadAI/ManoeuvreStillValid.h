/* 
*  Kevin Meergans, SquadAI, 2014
*  ManoeuvreStillValid.h
*  Team behaviour condition that checks whether a certain manoeuvre is still valid
*  that is whether certain conditions are still fulfilled.
*/

#ifndef MANOEUVRE_STILL_VALID_H
#define MANOEUVRE_STILL_VALID_H

// Includes
#include "TeamBehaviour.h"

// Forward declarations
class TeamAI;

//--------------------------------------------------------------------------------------
// Contains all additional data required for the creation of an execute manoeuvre behaviour.
//--------------------------------------------------------------------------------------
struct ManoeuvreStillValidInitData
{
	ManoeuvreStillValidInitData(TeamManoeuvreType manoeuvreType) 
		: m_manoeuvreType(manoeuvreType)
	{}

	TeamManoeuvreType m_manoeuvreType;  // The manoeuvre, the preconditions of which should be checked
};


class ManoeuvreStillValid : public TeamBehaviour
{
public:
	ManoeuvreStillValid(const char* name, TeamAI* pTeamAI, TeamManoeuvreType manoeuvreType, float aggressiveness, float defensiveness);
	~ManoeuvreStillValid(void);

	// Data access functions

	TeamManoeuvreType GetManoeuvreType(void) const;
	void SetManoeuvreType(TeamManoeuvreType manoeuvreType);

private:
	BehaviourStatus Update(float deltaTime);

	TeamManoeuvreType m_manoeuvreType; // The manoeuvre associated to this behaviour

};

#endif // MANOEUVRE_STILL_VALID_H