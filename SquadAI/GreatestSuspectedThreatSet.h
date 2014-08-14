/* 
*  Kevin Meergans, SquadAI, 2014
*  GreatestSuspectedThreatSet.h
*  A condition behaviour that checks whether one of the entity's suspected
*  threats was selected and set as greatest suspected threat.
*/

#ifndef GREATEST_SUSPECTED_THREAT_SET_H
#define GREATEST_SUSPECTED_THREAT_SET_H

// Includes
#include "UniversalIndividualBehaviour.h"

class GreatestSuspectedThreatSet : public UniversalIndividualBehaviour
{
public:
	GreatestSuspectedThreatSet(const char* name, Entity* pEntity);
	~GreatestSuspectedThreatSet(void);

private:
	BehaviourStatus Update(float deltaTime);
};

#endif // GREATEST_SUSPECTED_THREAT_SET_H