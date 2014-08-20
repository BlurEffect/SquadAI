/* 
*  Kevin Meergans, SquadAI, 2014
*  TeamActiveCharacteristicSelector.h
*  Specialisation of the active selector composite that sorts its children based on the 
*  characteristic of the associated team AI and thus changes the priority of the 
*  child behaviours.
*/

// Includes
#include "TeamActiveCharacteristicSelector.h"
#include "TeamAI.h"

TeamActiveCharacteristicSelector::TeamActiveCharacteristicSelector(const char* name, TeamAI* pTeamAI) : TeamActiveSelector(name, pTeamAI)
{

}

TeamActiveCharacteristicSelector::~TeamActiveCharacteristicSelector(void)
{

}

//--------------------------------------------------------------------------------------
// Initialises the active selector.
//--------------------------------------------------------------------------------------
void TeamActiveCharacteristicSelector::OnInitialise(void)
{
	m_currentChild = m_children.end();

	// Sort children
	std::sort(m_children.begin(), m_children.end(), SortTeamBehavioursByCharacteristic(GetTeamAI()->GetCharacteristic()));
}

