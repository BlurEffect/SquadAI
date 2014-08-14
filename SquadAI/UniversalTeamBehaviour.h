/* 
*  Kevin Meergans, SquadAI, 2014
*  UniversalTeamBehaviour.h
*  Abstract base class for behaviours that can be used for all kinds of 
*  team AIs.
*/

#ifndef UNIVERSAL_TEAM_BEHAVIOUR_H
#define UNIVERSAL_TEAM_BEHAVIOUR_H

// Includes
#include "Behaviour.h"

// Forward declarations
class TeamAI;

class UniversalTeamBehaviour : public Behaviour
{
public:
	UniversalTeamBehaviour(const char* name, TeamAI* pTeamAI);
	virtual ~UniversalTeamBehaviour(void);
	
	// Data access functions
	TeamAI* GetTeamAI(void);

private:
	TeamAI* m_pTeamAI; // The team AI that this behaviour belongs to

};

#endif // UNIVERSAL_TEAM_BEHAVIOUR_H