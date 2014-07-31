/* 
*  Kevin Meergans, SquadAI, 2014
*  Decorator.cpp
*  Abstract base class for decorator nodes of the behaviour tree.
*  Each decorator has a single child behaviour, which it decorates that is it might
*  modify the return code of the child control how often it is run or other actions that
*  however do not directly modify the behaviour of the child.
*  Based on Alex Champandard's presentation "Understanding the Second-Generation of Behavior Trees"
*  and the associated "Behavior Tree Starter Kit" (BTSK).
*/

// Includes
#include "Decorator.h"


Decorator::Decorator(Entity* pEntity, const char* name, Behaviour* pChild) : Behaviour(pEntity, name),
																		     m_pChild(pChild)
{

}

Decorator::~Decorator(void)
{
	if(m_pChild)
	{
		delete m_pChild;
		m_pChild = nullptr;
	}
}

//--------------------------------------------------------------------------------------
// Terminates the behaviour.
// Param1: The return code of the behaviour that is terminating.
//--------------------------------------------------------------------------------------
void Decorator::OnTerminate(BehaviourStatus status)
{
	Behaviour::Reset();
	//m_pChild->Reset();
}
