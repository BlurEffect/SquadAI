/* 
*  Kevin Meergans, SquadAI, 2014
*  MultiflagCTFTeamBehaviour.cpp
*  Abstract base class for specific behaviours that can be used by a team AI in
*  a Multiflag CTF match.
*/

// Includes
#include "MultiflagCTFTeamBehaviour.h"
#include "MultiflagCTFTeamAI.h"

MultiflagCTFTeamBehaviour::MultiflagCTFTeamBehaviour(const char* name, MultiflagCTFTeamAI* pTeamAI) : Behaviour(name),
																								      m_pMultiflagCTFTeamAI(pTeamAI)													
{
}

MultiflagCTFTeamBehaviour::~MultiflagCTFTeamBehaviour(void)
{
}

// Data access functions

MultiflagCTFTeamAI* MultiflagCTFTeamBehaviour::GetMultiflagCTFTeamAI(void)
{
	return m_pMultiflagCTFTeamAI;
}