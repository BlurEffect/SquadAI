/* 
*  Kevin Meergans, SquadAI, 2014
*  Repeat.h
*  A decorator that repeatedly executes its child for a specified number of times.
*  Based on Alex Champandard's presentation "Understanding the Second-Generation of Behavior Trees"
*  and the associated "Behavior Tree Starter Kit" (BTSK).
*/

#ifndef REPEAT_H
#define REPEAT_H

// Includes
#include "Decorator.h"

class Repeat : public Decorator
{
public:
	Repeat(Behaviour* pChild);
	~Repeat(void);

	void SetNumberOfRepeats(unsigned int count);

private:
	void OnInitialise(void);
	BehaviourStatus Update(void);

	unsigned int m_numberOfRepeats;
	unsigned int m_counter;
};

#endif // REPEAT_H