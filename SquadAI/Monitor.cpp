/* 
*  Kevin Meergans, SquadAI, 2014
*  Monitor.cpp
*  Parallel behaviour that ensures that a number of actions is executed only as long until a number
*  of conditions holds true or they succeed.
*  Based on Alex Champandard's presentation "Understanding the Second-Generation of Behavior Trees"
*  and the associated "Behavior Tree Starter Kit" (BTSK).
*/

// Includes
#include "Monitor.h"


Monitor::Monitor(Entity* pEntity, const char* name) : Parallel(pEntity, name, RequireAll, RequireOne)
{
}

Monitor::~Monitor(void)
{
}

//--------------------------------------------------------------------------------------
// Adds a new condition to the monitor. The condition is inserted at the start of 
// the array of children to ensure that all conditions are checked before the actions
// are evaluated/updated.
// Param1: A pointer to a condition behaviour.
//--------------------------------------------------------------------------------------
void Monitor::AddCondition(Behaviour* pCondition)
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
void Monitor::AddAction(Behaviour* pAction)
{
	if(pAction != nullptr)
	{
		m_children.push_back(pAction);
	}
}
