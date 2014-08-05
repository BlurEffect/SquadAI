/* 
*  Kevin Meergans, SquadAI, 2014
*  ResolveSuspectedThreat.h
*  An action behaviour that removes a suspected threat from the entity's
*  memory. Should be used after investigating the last known position of a 
*  threat without finding it.
*/

#ifndef RESOLVE_SUSPECTED_THREAT_H
#define RESOLVE_SUSPECTED_THREAT_H

// Includes
#include "Behaviour.h"

class ResolveSuspectedThreat : public Behaviour
{
public:
	ResolveSuspectedThreat(Entity* pEntity, const char* name);
	~ResolveSuspectedThreat(void);

private:
	BehaviourStatus Update(float deltaTime);

	void OnInitialise(void);
	void OnTerminate(BehaviourStatus status);
};

#endif // RESOLVE_SUSPECTED_THREAT_H