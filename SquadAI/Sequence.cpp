/* 
*  Kevin Meergans, SquadAI, 2014
*  Sequence.cpp
*  Composite behaviour that represents a sequence of behaviours, which are
*  executed in order as long as each of them succeeds. If one of the child behaviours
*  fails, the sequence fails as well. The sequence succeeds if all children are
*  executed successfully.
*  Based on Alex Champandard's presentation "Understanding the Second-Generation of Behavior Trees"
*  and the associated "Behavior Tree Starter Kit" (BTSK).
*/

// Includes
#include "Sequence.h"

Sequence::Sequence(const char* name) : Composite(name)
{
}

Sequence::~Sequence(void)
{
}

//--------------------------------------------------------------------------------------
// Initialises the sequence.
//--------------------------------------------------------------------------------------
void Sequence::OnInitialise(void)
{
	// Set the first child as the one to start the sequence with.
	m_currentChild = m_children.begin();
}

//--------------------------------------------------------------------------------------
// Updates the sequence.
// Param1: The time in seconds passed since the last frame.
// Returns the state of the sequence.
//--------------------------------------------------------------------------------------
BehaviourStatus Sequence::Update(float deltaTime)
{
	const char* name = GetName();

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

		// If the current child failed or is still running, this equals the 
		// current state of the whole sequence.
		if(status != StatusSuccess)
		{
			return status;
		}

		// Move on to the next child and check if the end of the sequence was
		// reached. In latter case, the sequence as a whole succeeded.
		if(++m_currentChild == m_children.end())
		{
			return StatusSuccess;
		}
	}

	// Shouldn't be reached, an error occurred.
	return StatusInvalid;
}