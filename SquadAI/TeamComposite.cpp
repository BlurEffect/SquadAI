/* 
*  Kevin Meergans, SquadAI, 2014
*  TeamComposite.cpp
*  Abstract base class for team composite behaviours.
*/

// Includes
#include "TeamComposite.h"
#include "TeamAI.h"

TeamComposite::TeamComposite(const char* name, TeamAI* pTeamAI) : TeamBehaviour(name, pTeamAI)
{
}

TeamComposite::~TeamComposite(void)
{
	for(std::vector<TeamBehaviour*>::iterator it = m_children.begin(); it != m_children.end(); ++it)
	{
		if(*it)
		{
			delete (*it);
			*it = nullptr;
		}
	}
}

//--------------------------------------------------------------------------------------
// Adds a new child behaviour to this composite. The child is appended at the end of
// the list of child behaviours.
// Param1: A pointer to the new child behaviour that should be added.
//--------------------------------------------------------------------------------------
void TeamComposite::AddChild(TeamBehaviour* pChild)
{
	if(pChild != nullptr)
	{
		m_children.push_back(pChild);
	}
}

//--------------------------------------------------------------------------------------
// Removes a child behaviour from this composite. 
// Param1: A pointer to the new child behaviour that should be added.
//--------------------------------------------------------------------------------------
void TeamComposite::RemoveChild(TeamBehaviour* pChild)
{
	if(pChild != nullptr)
	{
		std::vector<TeamBehaviour*>::iterator foundIt = std::find_if(m_children.begin(), m_children.end(), FindBehaviourById(pChild->GetId()));
		
		if(foundIt != m_children.end())
		{
			m_children.erase(foundIt);
		}
	}
}

//--------------------------------------------------------------------------------------
// Removes all child behaviours associated to this composite.
//--------------------------------------------------------------------------------------
void TeamComposite::ClearChildren(void)
{
	m_children.clear();
}

//--------------------------------------------------------------------------------------
// Terminates the behaviour.
// Param1: The return code of the behaviour that is terminating.
//--------------------------------------------------------------------------------------
void TeamComposite::OnTerminate(BehaviourStatus status)
{
	if(status == StatusAborted)
	{
		AbortChildren();
	}

	ResetChildren();
	TeamBehaviour::Reset();
}

//--------------------------------------------------------------------------------------
// Resets all children of the composite.
//--------------------------------------------------------------------------------------
void TeamComposite::ResetChildren(void)
{
	for(std::vector<TeamBehaviour*>::iterator it = m_children.begin(); it != m_children.end(); ++it)
	{
		(*it)->Reset();
	}
}

//--------------------------------------------------------------------------------------
// Aborts all children of the composite.
//--------------------------------------------------------------------------------------
void TeamComposite::AbortChildren(void)
{
	for(std::vector<TeamBehaviour*>::iterator it = m_children.begin(); it != m_children.end(); ++it)
	{
		(*it)->Abort();
	}
}
