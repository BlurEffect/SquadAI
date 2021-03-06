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
#include "UniversalIndividualBehaviour.h"

class ResolveSuspectedThreat : public UniversalIndividualBehaviour
{
public:
	ResolveSuspectedThreat(const char* name, Entity* pEntity);
	~ResolveSuspectedThreat(void);

private:
	BehaviourStatus Update(float deltaTime);
};

#endif // RESOLVE_SUSPECTED_THREAT_H