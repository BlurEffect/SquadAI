/* 
*  Kevin Meergans, SquadAI, 2014
*  Monitor.h
*  Parallel behaviour that ensures that a number of actions is executed only as long until a number
*  of conditions holds true or they succeed.
*  Based on Alex Champandard's presentation "Understanding the Second-Generation of Behavior Trees"
*  and the associated "Behavior Tree Starter Kit" (BTSK).
*/

#ifndef MONITOR_H
#define MONITOR_H

// Includes
#include "Parallel.h"

class Monitor : public Parallel
{
public:

	Monitor(Entity* pEntity, const char* name);
	virtual ~Monitor(void);

	void AddCondition(Behaviour* pCondition);
	void AddAction(Behaviour* pAction);
};

#endif // MONITOR_H