/* 
*  Kevin Meergans, SquadAI, 2014
*  Parallel.cpp
*  Composite behaviour that executes all of its child behaviours in parallel. Success and failure
*  of the parallel composite depend on policies that have to be set by the user.
*  Based on Alex Champandard's presentation "Understanding the Second-Generation of Behavior Trees"
*  and the associated "Behavior Tree Starter Kit" (BTSK).
*/

// Includes
#include "Parallel.h"

Parallel::Parallel(const char* name, ParallelPolicy successPolicy, ParallelPolicy failurePolicy) : Composite(name),
																		  						   m_successPolicy(successPolicy),
																							   	   m_failurePolicy(failurePolicy)
{
}

Parallel::~Parallel(void)
{
}

//--------------------------------------------------------------------------------------
// Updates the sequence.
// Param1: The time in seconds passed since the last frame.
// Returns the state of the sequence.
//--------------------------------------------------------------------------------------
BehaviourStatus Parallel::Update(float deltaTime)
{
	if(m_children.size() < 1)
	{
		return StatusInvalid;
	}

	unsigned int successCount = 0;
	unsigned int failureCount = 0;

    for(std::vector<Behaviour*>::iterator it = m_children.begin(); it != m_children.end(); ++it)
    {
		//if(!(*it)->IsTerminated())
        //{
		BehaviourStatus childStatus =   (*it)->Tick(deltaTime);
        //}

		if(childStatus == StatusSuccess)
        {
			// The child succeeded.
            ++successCount;
            if (m_successPolicy == RequireOne)
            {
				return StatusSuccess;
            }
        }

		if(childStatus == StatusFailure)
        {
			// The child failed.
            ++failureCount;
            if (m_failurePolicy == RequireOne)
            {
				return StatusFailure;
            }
        }
    }

    if(m_failurePolicy == RequireAll && failureCount == m_children.size())
    {
		// All children failed.
		return StatusFailure;
    }

    if(m_successPolicy == RequireAll && successCount == m_children.size())
    {
		// All children succeeded.
        return StatusSuccess;
    }

	return StatusRunning;
}


