/* 
*  Kevin Meergans, SquadAI, 2014
*  Selector.h
*  Composite behaviour that represents a conditional selection from a range of
*  available behaviours. Starting with the first child behaviour, the selector 
*  checks its children fpr execution until one successfully completes. At that point
*  the selector succeeds as well. The selector fails when none of the children 
*  completes successfully.
*  Based on Alex Champandard's presentation "Understanding the Second-Generation of Behavior Trees"
*  and the associated "Behavior Tree Starter Kit" (BTSK).
*/

#ifndef SELECTOR_H
#define SELECTOR_H

// Includes
#include "Composite.h"

class Selector : public Composite
{
public:
	Selector(const char* name);
	virtual ~Selector(void);

protected:
	virtual void OnInitialise(void);
	virtual BehaviourStatus Update(float deltaTime);

	std::vector<Behaviour*>::iterator m_currentChild; // The currently active child behaviour of the sequence
};

#endif // SELECTOR_H