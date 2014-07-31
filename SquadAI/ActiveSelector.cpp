/* 
*  Kevin Meergans, SquadAI, 2014
*  ActiveSelector.cpp
*  Specialisation of the selector composite that aborts a running child behaviour if another
*  with a higher priority becomes executable.
*  Based on Alex Champandard's presentation "Understanding the Second-Generation of Behavior Trees"
*  and the associated "Behavior Tree Starter Kit" (BTSK).
*/

// Includes
#include "ActiveSelector.h"

ActiveSelector::ActiveSelector(Entity* pEntity, const char* name) : Selector(pEntity, name)
{

}

ActiveSelector::~ActiveSelector(void)
{

}

//--------------------------------------------------------------------------------------
// Initialises the active selector.
//--------------------------------------------------------------------------------------
void ActiveSelector::OnInitialise(void)
{
	m_currentChild = m_children.end();
}

//--------------------------------------------------------------------------------------
// Updates the active selector.
// Param1: The time in seconds passed since the last frame.
// Returns the state of the active selector.
//--------------------------------------------------------------------------------------
BehaviourStatus ActiveSelector::Update(float deltaTime)
{
	std::vector<Behaviour*>::iterator previous = m_currentChild;

	// Run the normal selector behaviour to determine the highest priority
	// child that is currently executable.
    Selector::OnInitialise();
    BehaviourStatus result = Selector::Update(deltaTime);

    if(previous != m_children.end() && m_currentChild != previous)
    {
		// If a higher priority child behaviour is able to run, abort the previous one.
		(*previous)->Abort();
    }
    return result;
}

