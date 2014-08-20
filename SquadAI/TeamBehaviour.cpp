/* 
*  Kevin Meergans, SquadAI, 2014
*  TeamBehaviour.cpp
*  Abstract base class for behaviours that can be used by team AIs.
*/

// Includes
#include "TeamBehaviour.h"
#include "TeamAI.h"

TeamBehaviour::TeamBehaviour(const char* name, TeamAI* pTeamAI, float aggressiveness, float defensiveness) : Behaviour(name),
																									  	     m_pTeamAI(pTeamAI),
																										     m_aggressiveness(aggressiveness),
																										     m_defensiveness(defensiveness)
{
}

TeamBehaviour::~TeamBehaviour(void)
{
}

//--------------------------------------------------------------------------------------
// Used to calculate the characteristic values for this behaviour and all children of it.
// Should be called for initialisation and when characteristics of certain behaviour 
// change or when behaviours are added respectively removed from the tree.
//--------------------------------------------------------------------------------------
void TeamBehaviour::CalculateCharacteristicValues(void)
{
	// Do nothing
}

// Data access functions

TeamAI* TeamBehaviour::GetTeamAI(void)
{
	return m_pTeamAI;
}

float TeamBehaviour::GetAggressiveness(void) const
{
	return m_aggressiveness;
}

float TeamBehaviour::GetDefensiveness(void) const
{
	return m_defensiveness;
}

void TeamBehaviour::SetAggressiveness(float aggressiveness)
{
	m_aggressiveness = aggressiveness;
}

void TeamBehaviour::SetDefensiveness(float defensiveness)
{
	m_defensiveness = defensiveness;
}