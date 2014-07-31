/* 
*  Kevin Meergans, SquadAI, 2014
*  Decorator.h
*  Abstract base class for decorator nodes of the behaviour tree.
*  Each decorator has a single child behaviour, which it decorates that is it might
*  modify the return code of the child control how often it is run or other actions that
*  however do not directly modify the behaviour of the child.
*  Based on Alex Champandard's presentation "Understanding the Second-Generation of Behavior Trees"
*  and the associated "Behavior Tree Starter Kit" (BTSK).
*/

#ifndef DECORATOR_H
#define DECORATOR_H

// Includes
#include "Behaviour.h"

class Decorator : public Behaviour
{
public:
	Decorator(Entity* pEntity, const char* name, Behaviour* pChild);
	virtual ~Decorator(void) = 0;

protected:
	Behaviour* m_pChild; // The single child behaviour that is being decorated

};

#endif // DECORATOR_H