/* 
*  Kevin Meergans, SquadAI, 2014
*  Sequence.h
*  Composite behaviour that represents a sequence of behaviours, which are
*  executed in order as long as each of them succeeds. If one of the child behaviours
*  fails, the sequence fails as well. The sequence succeeds if all children are
*  executed successfully.
*  Based on Alex Champandard's presentation "Understanding the Second-Generation of Behavior Trees"
*  and the associated "Behavior Tree Starter Kit" (BTSK).
*/

#ifndef SEQUENCE_H
#define SEQUENCE_H

// Includes
#include "Composite.h"

class Sequence : public Composite
{
public:
	Sequence(const char* name);
	virtual ~Sequence(void);

protected:
	virtual void OnInitialise(void);
	virtual BehaviourStatus Update(void);

	std::vector<Behaviour*>::iterator m_currentChild; // The currently active child behaviour of the sequence
};

#endif // SEQUENCE_H