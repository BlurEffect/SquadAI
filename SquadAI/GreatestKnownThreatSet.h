/* 
*  Kevin Meergans, SquadAI, 2014
*  GreatestKnownThreatSet.h
*  A condition behaviour that checks whether one of the entity's known
*  threats was selected and set as greatest known threat.
*/

#ifndef GREATEST_KNOWN_THREAT_SET_H
#define GREATEST_KNOWN_THREAT_SET_H

// Includes
#include "Behaviour.h"

class GreatestKnownThreatSet : public Behaviour
{
public:
	GreatestKnownThreatSet(Entity* pEntity, const char* name);
	~GreatestKnownThreatSet(void);

private:
	BehaviourStatus Update(float deltaTime);

	void OnInitialise(void);
	void OnTerminate(BehaviourStatus status);
};

#endif // GREATEST_KNOWN_THREAT_SET_H