/* 
*  Kevin Meergans, SquadAI, 2014
*  Selector.h
*  Composite behaviour that represents a conditional selection from a range of
*  available behaviours. Starting with the first child behaviour, the selector 
*  checks its children fpr execution until one successfully completes. At that point
*  the selector succeeds as well. The selector fails when none of the children 
*  completes successfully.
*  Based on Alex Champandard's presentation "Understanding the Second-Generation of Behavior Trees"
*  and the associated "Behavior Tree Starter Kit" (BTSK).
*/

// Includes
#include "Selector.h"

Selector::Selector(const char* name) : Composite(name)
{
}

Selector::~Selector(void)
{
}

//--------------------------------------------------------------------------------------
// Initialises the selector.
//--------------------------------------------------------------------------------------
void Selector::OnInitialise(void)
{
	// Set the first child as the one to start the selection with.
	m_currentChild = m_children.begin();
}

//--------------------------------------------------------------------------------------
// Updates the selector.
// Returns the state of the selector.
//--------------------------------------------------------------------------------------
BehaviourStatus Selector::Update(void)
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
		BehaviourStatus status = (*m_currentChild)->Tick();

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