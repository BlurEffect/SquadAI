/* 
*  Kevin Meergans, SquadAI, 2014
*  UniversalTeamBehaviour.cpp
*  Abstract base class for behaviours that can be used for all kinds of 
*  team AIs.
*/

// Includes
#include "UniversalTeamBehaviour.h"
#include "TeamAI.h"

UniversalTeamBehaviour::UniversalTeamBehaviour(const char* name, TeamAI* pTeamAI) : Behaviour(name),
																					m_pTeamAI(pTeamAI)													
{
}

UniversalTeamBehaviour::~UniversalTeamBehaviour(void)
{
}

// Data access functions

TeamAI* UniversalTeamBehaviour::GetTeamAI(void)
{
	return m_pTeamAI;
}