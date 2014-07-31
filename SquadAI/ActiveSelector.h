/* 
*  Kevin Meergans, SquadAI, 2014
*  ActiveSelector.h
*  Specialisation of the selector composite that aborts a running child behaviour if another
*  with a higher priority becomes executable.
*  Based on Alex Champandard's presentation "Understanding the Second-Generation of Behavior Trees"
*  and the associated "Behavior Tree Starter Kit" (BTSK).
*/

#ifndef ACTIVE_SELECTOR_H
#define ACTIVE_SELECTOR_H

// Includes
#include "Selector.h"

class ActiveSelector : public Selector
{
public:
	ActiveSelector(Entity* pEntity, const char* name);
	virtual ~ActiveSelector(void);

protected:
	virtual void OnInitialise(void);
	virtual BehaviourStatus Update(float deltaTime);

};

#endif // ACTIVE_SELECTOR_H