/* 
*  Kevin Meergans, SquadAI, 2014
*  GreatestKnownThreatSet.h
*  A condition behaviour that checks whether one of the entity's known
*  threats was selected and set as greatest known threat.
*/

#ifndef GREATEST_KNOWN_THREAT_SET_H
#define GREATEST_KNOWN_THREAT_SET_H

// Includes
#include "UniversalIndividualBehaviour.h"

class GreatestKnownThreatSet : public UniversalIndividualBehaviour
{
public:
	GreatestKnownThreatSet(const char* name, Entity* pEntity);
	~GreatestKnownThreatSet(void);

private:
	BehaviourStatus Update(float deltaTime);
};

#endif // GREATEST_KNOWN_THREAT_SET_H