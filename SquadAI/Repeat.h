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

//--------------------------------------------------------------------------------------
// Contains all additional data required for the creation of a parallel behaviour.
//--------------------------------------------------------------------------------------
struct RepeatInitData
{
	RepeatInitData(Behaviour* pChild, unsigned int numberOfRepeats) : m_pChild(pChild),
																	  m_numberOfRepeats(numberOfRepeats)
	{}

	Behaviour* m_pChild;
	unsigned int m_numberOfRepeats;
};

class Repeat : public Decorator
{
public:
	Repeat(Entity* pEntity, const char* name, Behaviour* pChild, unsigned int numberOfRepeats);
	~Repeat(void);

	void SetNumberOfRepeats(unsigned int count);

private:
	void OnInitialise(void);
	BehaviourStatus Update(float deltaTime);

	unsigned int m_numberOfRepeats;
	unsigned int m_counter;
};

#endif // REPEAT_H