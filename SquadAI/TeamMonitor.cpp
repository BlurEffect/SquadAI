/* 
*  Kevin Meergans, SquadAI, 2014
*  TeamMonitor.cpp
*  Parallel team behaviour that ensures that a number of actions is executed only as long until a number
*  of conditions holds true or they succeed.
*/

// Includes
#include "TeamMonitor.h"
#include "TeamAI.h"


TeamMonitor::TeamMonitor(const char* name, TeamAI* pTeamAI) : TeamParallel(name, pTeamAI, TeamRequireAll, TeamRequireOne)
{
}

TeamMonitor::~TeamMonitor(void)
{
}

//--------------------------------------------------------------------------------------
// Adds a new condition to the monitor. The condition is inserted at the start of 
// the array of children to ensure that all conditions are checked before the actions
// are evaluated/updated.
// Param1: A pointer to a condition behaviour.
//--------------------------------------------------------------------------------------
void TeamMonitor::AddCondition(TeamBehaviour* pCondition)
{
	if(pCondition != nullptr)
	{
		m_children.insert(m_children.begin(), pCondition);
	}
}

//--------------------------------------------------------------------------------------
// Adds a new action to the monitor, which is added at the end of the array of children.
// Param1: A pointer to an action behaviour.
//--------------------------------------------------------------------------------------
void TeamMonitor::AddAction(TeamBehaviour* pAction)
{
	if(pAction != nullptr)
	{
		m_children.push_back(pAction);
	}
}

/*
//--------------------------------------------------------------------------------------
// Updates the monitor.
// Param1: The time in seconds passed since the last frame.
// Returns the state of the monitor.
//--------------------------------------------------------------------------------------
BehaviourStatus TeamMonitor::Update(float deltaTime)
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
		BehaviourStatus childStatus = (*it)->Tick(deltaTime);
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
			AbortChildren();
			return StatusFailure;
        }
    }

    if(successCount == m_children.size())
    {
		// All children succeeded.
        return StatusSuccess;
    }

	return StatusRunning;
}
*/

