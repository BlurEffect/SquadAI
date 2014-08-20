/* 
*  Kevin Meergans, SquadAI, 2014
*  TeamSelector.cpp
*  Composite behaviour that represents a conditional selection from a range of
*  available team behaviours. Starting with the first child behaviour, the selector 
*  checks its children fpr execution until one successfully completes. At that point
*  the selector succeeds as well. The selector fails when none of the children 
*  completes successfully.
*/

// Includes
#include "TeamSelector.h"
#include "TeamAI.h"

TeamSelector::TeamSelector(const char* name, TeamAI* pTeamAI) : TeamComposite(name, pTeamAI)
{
}

TeamSelector::~TeamSelector(void)
{
}

//--------------------------------------------------------------------------------------
// Used to calculate the characteristic values for this behaviour and all children of it.
// Should be called for initialisation and when characteristics of certain behaviour 
// change or when behaviours are added respectively removed from the tree.
//--------------------------------------------------------------------------------------
void TeamSelector::CalculateCharacteristicValues(void)
{
	// Forward the call further down the tree.
	for(std::vector<TeamBehaviour*>::iterator it = m_children.begin(); it != m_children.end(); ++it)
	{
		(*it)->CalculateCharacteristicValues();
	}

	
	// Calculate the new values as the average of the values of all children (as it can't be told, which one is going to be executed)
	float sumAggressiveness = 0;
	float sumDefensiveness  = 0;

	for(std::vector<TeamBehaviour*>::iterator it = m_children.begin(); it != m_children.end(); ++it)
	{
		sumAggressiveness += (*it)->GetAggressiveness();
		sumDefensiveness  += (*it)->GetDefensiveness();
	}

	// The decorator has the same characteristic values as its single children.
	SetAggressiveness(sumAggressiveness/m_children.size());
	SetDefensiveness(sumDefensiveness/m_children.size());
}

//--------------------------------------------------------------------------------------
// Initialises the selector.
//--------------------------------------------------------------------------------------
void TeamSelector::OnInitialise(void)
{
	// Set the first child as the one to start the selection with.
	m_currentChild = m_children.begin();
}

//--------------------------------------------------------------------------------------
// Updates the selector.
// Param1: The time in seconds passed since the last frame.
// Returns the state of the selector.
//--------------------------------------------------------------------------------------
BehaviourStatus TeamSelector::Update(float deltaTime)
{
	if(m_children.size() < 1)
	{
		return StatusInvalid;
	}

	// Keep executing child behaviours until one returns a running
	// status. This is required as some children, such as conditions,
	// might complete within a single update.
	while(true)
	{
		BehaviourStatus status = (*m_currentChild)->Tick(deltaTime);

		// If the current child succeeded or is still running, this equals the 
		// current state of the whole selector.
		if(status != StatusFailure)
		{
			return status;
		}

		// Move on to the next child and check if the last child of the selector was
		// reached. In latter case, the selector as a whole fails as none
		// of the children completed successfully.
		if(++m_currentChild == m_children.end())
		{
			return StatusFailure;
		}
	}

	// Shouldn't be reached, an error occurred.
	return StatusInvalid;
}