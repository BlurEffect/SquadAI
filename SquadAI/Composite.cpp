/* 
*  Kevin Meergans, SquadAI, 2014
*  Composite.cpp
*  Abstract base class for composite behaviours.
*  Based on Alex Champandard's presentation "Understanding the Second-Generation of Behavior Trees"
*  and the associated "Behavior Tree Starter Kit" (BTSK).
*/

// Includes
#include "Composite.h"

Composite::Composite(Entity* pEntity, const char* name) : Behaviour(pEntity, name)
{
}

Composite::~Composite(void)
{
	for(std::vector<Behaviour*>::iterator it = m_children.begin(); it != m_children.end(); ++it)
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
void Composite::AddChild(Behaviour* pChild)
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
void Composite::RemoveChild(Behaviour* pChild)
{
	if(pChild != nullptr)
	{
		std::vector<Behaviour*>::iterator foundIt = std::find_if(m_children.begin(), m_children.end(), FindBehaviourById(pChild->GetId()));
		
		if(foundIt != m_children.end())
		{
			m_children.erase(foundIt);
		}
	}
}

//--------------------------------------------------------------------------------------
// Removes all child behaviours associated to this composite.
//--------------------------------------------------------------------------------------
void Composite::ClearChildren(void)
{
	m_children.clear();
}

//--------------------------------------------------------------------------------------
// Terminates the behaviour.
// Param1: The return code of the behaviour that is terminating.
//--------------------------------------------------------------------------------------
void Composite::OnTerminate(BehaviourStatus status)
{
	Behaviour::Reset();
	if(status == StatusAborted)
	{
		ResetChildren();
	}//else
	//{
	//	Behaviour::Reset();
	//}
	//ResetChildren();
}

//--------------------------------------------------------------------------------------
// Resets all children of the composite.
//--------------------------------------------------------------------------------------
void Composite::ResetChildren(void)
{
	for(std::vector<Behaviour*>::iterator it = m_children.begin(); it != m_children.end(); ++it)
	{
		(*it)->Reset();
	}
}
