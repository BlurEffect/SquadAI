/* 
*  Kevin Meergans, SquadAI, 2014
*  ManoeuvrePreconditionsFulfilled.h
*  Team behaviour condition that checks whether all preconditions for the execution
*  of a certain manoeuvre are fulfilled. This mostly consists of checks of the game
*  state and availability of entities to execute the manoeuvre.
*/

#ifndef MANOEUVRE_PRECONDITIONS_FULFILLED_H
#define MANOEUVRE_PRECONDITIONS_FULFILLED_H

// Includes
#include "TeamBehaviour.h"

// Forward declarations
class TeamAI;

//--------------------------------------------------------------------------------------
// Contains all additional data required for the creation of an execute manoeuvre behaviour.
//--------------------------------------------------------------------------------------
struct ManoeuvrePreconditionsFulfilledInitData
{
	ManoeuvrePreconditionsFulfilledInitData(TeamManoeuvreType manoeuvreType) 
		: m_manoeuvreType(manoeuvreType)
	{}

	TeamManoeuvreType m_manoeuvreType;  // The manoeuvre, the preconditions of which should be checked
};


class ManoeuvrePreconditionsFulfilled : public TeamBehaviour
{
public:
	ManoeuvrePreconditionsFulfilled(const char* name, TeamAI* pTeamAI, TeamManoeuvreType manoeuvreType, float aggressiveness, float defensiveness);
	~ManoeuvrePreconditionsFulfilled(void);

	// Data access functions

	TeamManoeuvreType GetManoeuvreType(void) const;
	void SetManoeuvreType(TeamManoeuvreType manoeuvreType);

private:
	BehaviourStatus Update(float deltaTime);

	TeamManoeuvreType m_manoeuvreType; // The manoeuvre associated to this behaviour

};

#endif // MANOEUVRE_PRECONDITIONS_FULFILLED_H