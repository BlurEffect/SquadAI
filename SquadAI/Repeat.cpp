/* 
*  Kevin Meergans, SquadAI, 2014
*  Repeat.cpp
*  A decorator that repeatedly executes its child for a specified number of times.
*  Based on Alex Champandard's presentation "Understanding the Second-Generation of Behavior Trees"
*  and the associated "Behavior Tree Starter Kit" (BTSK).
*/

// Includes
#include "Repeat.h"


Repeat::Repeat(const char* name, Behaviour* pChild, unsigned int numberOfRepeats) : Decorator(name, pChild),
																				    m_numberOfRepeats(numberOfRepeats),
																					m_counter(0)		
{
}

Repeat::~Repeat(void)
{
}

//--------------------------------------------------------------------------------------
// Initialises the repeat decorator.
//--------------------------------------------------------------------------------------
void Repeat::OnInitialise(void)
{
	m_counter = 0;
}

//--------------------------------------------------------------------------------------
// Updates the repeat decorator.
// Param1: The time in seconds passed since the last frame.
// Returns the state of the repeat decorator.
//--------------------------------------------------------------------------------------
BehaviourStatus Repeat::Update(float deltaTime)
{
	if(!m_pChild)
	{
		// If the child was not initialised properly, indicate a problem by returning invalid
		return StatusInvalid;
	}

	while(true)
	{
		BehaviourStatus status = m_pChild->Tick(deltaTime);

		// If the child failed or is still running this is also the return state
		// of the repeat decorator.
		if(status != StatusSuccess)
		{
			return status;
		}

		// Increase the count and check if the number of repeats has been reached.
		// In latter case the repeat decorator succeeds.
		if(++m_counter == m_numberOfRepeats)
		{
			return StatusSuccess;
		}

		// The count was increased but the number of repeats not yet reached. 
		// Reset the child behaviour to make sure it is started all over again.
		m_pChild->Reset();
	}
	
	// Shouldn't be reached, an error occurred.
	return StatusInvalid;

}

void Repeat::SetNumberOfRepeats(unsigned int count)
{
	m_numberOfRepeats = count;
}



