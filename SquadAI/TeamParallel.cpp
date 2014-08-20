/* 
*  Kevin Meergans, SquadAI, 2014
*  TeamParallel.cpp
*  Composite behaviour that executes all of its child team behaviours in parallel. Success and failure
*  of the parallel composite depend on policies that have to be set by the user.
*/

// Includes
#include "TeamParallel.h"
#include "TeamAI.h"

TeamParallel::TeamParallel(const char* name, TeamAI* pTeamAI, TeamParallelPolicy successPolicy, TeamParallelPolicy failurePolicy) 
	: TeamComposite(name, pTeamAI),
	  m_successPolicy(successPolicy),
	  m_failurePolicy(failurePolicy)
{
}

TeamParallel::~TeamParallel(void)
{
}

//--------------------------------------------------------------------------------------
// Used to calculate the characteristic values for this behaviour and all children of it.
// Should be called for initialisation and when characteristics of certain behaviour 
// change or when behaviours are added respectively removed from the tree.
//--------------------------------------------------------------------------------------
void TeamParallel::CalculateCharacteristicValues(void)
{
	// Forward the call further down the tree.
	for(std::vector<TeamBehaviour*>::iterator it = m_children.begin(); it != m_children.end(); ++it)
	{
		(*it)->CalculateCharacteristicValues();
	}

	
	// Calculate the new values as the sum of the values of all children (as all are executed in parallel)
	float aggressiveness = 0;
	float defensiveness  = 0;

	for(std::vector<TeamBehaviour*>::iterator it = m_children.begin(); it != m_children.end(); ++it)
	{
		aggressiveness += (*it)->GetAggressiveness();
		defensiveness  += (*it)->GetDefensiveness();
	}

	// The decorator has the same characteristic values as its single children.
	SetAggressiveness(aggressiveness);
	SetDefensiveness(defensiveness);
}

//--------------------------------------------------------------------------------------
// Updates the sequence.
// Param1: The time in seconds passed since the last frame.
// Returns the state of the sequence.
//--------------------------------------------------------------------------------------
BehaviourStatus TeamParallel::Update(float deltaTime)
{
	if(m_children.size() < 1)
	{
		return StatusInvalid;
	}

	unsigned int successCount = 0;
	unsigned int failureCount = 0;

    for(std::vector<TeamBehaviour*>::iterator it = m_children.begin(); it != m_children.end(); ++it)
    {
		//if(!(*it)->IsTerminated())
        //{
		BehaviourStatus childStatus =   (*it)->Tick(deltaTime);
        //}

		if(childStatus == StatusSuccess)
        {
			// The child succeeded.
            ++successCount;
            if (m_successPolicy == TeamRequireOne)
            {
				return StatusSuccess;
            }
        }

		if(childStatus == StatusFailure)
        {
			// The child failed.
            ++failureCount;
            if (m_failurePolicy == TeamRequireOne)
            {
				return StatusFailure;
            }
        }
    }

    if(m_failurePolicy == TeamRequireAll && failureCount == m_children.size())
    {
		// All children failed.
		return StatusFailure;
    }

    if(m_successPolicy == TeamRequireAll && successCount == m_children.size())
    {
		// All children succeeded.
        return StatusSuccess;
    }

	return StatusRunning;
}


