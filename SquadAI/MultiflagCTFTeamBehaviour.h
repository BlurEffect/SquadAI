/* 
*  Kevin Meergans, SquadAI, 2014
*  MultiflagCTFTeamBehaviour.h
*  Abstract base class for specific behaviours that can be used by a team AI in
*  a Multiflag CTF match.
*/

#ifndef MULTIFLAG_CTF_TEAM_BEHAVIOUR_H
#define MULTIFLAG_CTF_TEAM_BEHAVIOUR_H

// Includes
#include "Behaviour.h"

// Forward declarations
class MultiflagCTFTeamAI;

class MultiflagCTFTeamBehaviour : public Behaviour
{
public:
	MultiflagCTFTeamBehaviour(const char* name, MultiflagCTFTeamAI* pTeamAI);
	virtual ~MultiflagCTFTeamBehaviour(void);
	
	// Data access functions
	MultiflagCTFTeamAI* GetMultiflagCTFTeamAI(void);

private:
	MultiflagCTFTeamAI* m_pMultiflagCTFTeamAI; // The specific team AI that this behaviour belongs to

};

#endif // MULTIFLAG_CTF_TEAM_BEHAVIOUR_H