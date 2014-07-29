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


Decorator::Decorator(const char* name, Behaviour* pChild) : Behaviour(name),
															m_pChild(pChild)
{

}

Decorator::~Decorator(void)
{

}
