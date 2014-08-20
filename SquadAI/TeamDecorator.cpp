/* 
*  Kevin Meergans, SquadAI, 2014
*  TeamDecorator.cpp
*  Abstract base class for team decorator nodes of the behaviour tree.
*  Each decorator has a single child team behaviour, which it decorates that is it might
*  modify the return code of the child control how often it is run or other actions that
*  however do not directly modify the behaviour of the child.
*/


// Includes
#include "TeamDecorator.h"
#include "TeamAI.h"

TeamDecorator::TeamDecorator(const char* name, TeamAI* pTeamAI, TeamBehaviour* pChild) : TeamBehaviour(name, pTeamAI),
																						 m_pChild(pChild)
{

}

TeamDecorator::~TeamDecorator(void)
{
	if(m_pChild)
	{
		delete m_pChild;
		m_pChild = nullptr;
	}
}

//--------------------------------------------------------------------------------------
// Used to calculate the characteristic values for this behaviour and all children of it.
// Should be called for initialisation and when characteristics of certain behaviour 
// change or when behaviours are added respectively removed from the tree.
//--------------------------------------------------------------------------------------
void TeamDecorator::CalculateCharacteristicValues(void)
{
	// Forward the call further down the tree.
	m_pChild->CalculateCharacteristicValues();

	// The decorator has the same characteristic values as its single children.
	SetAggressiveness(m_pChild->GetAggressiveness());
	SetDefensiveness(m_pChild->GetDefensiveness());
}

//--------------------------------------------------------------------------------------
// Terminates the behaviour.
// Param1: The return code of the behaviour that is terminating.
//--------------------------------------------------------------------------------------
void TeamDecorator::OnTerminate(BehaviourStatus status)
{
	if(status == StatusAborted)
	{
		m_pChild->Abort();
	}

	TeamBehaviour::Reset();
}
