/* 
*  Kevin Meergans, SquadAI, 2014
*  GreatestSuspectedThreatSet.h
*  A condition behaviour that checks whether one of the entity's suspected
*  threats was selected and set as greatest suspected threat.
*/

#ifndef GREATEST_SUSPECTED_THREAT_SET_H
#define GREATEST_SUSPECTED_THREAT_SET_H

// Includes
#include "Behaviour.h"

class GreatestSuspectedThreatSet : public Behaviour
{
public:
	GreatestSuspectedThreatSet(Entity* pEntity, const char* name);
	~GreatestSuspectedThreatSet(void);

private:
	BehaviourStatus Update(float deltaTime);

	void OnInitialise(void);
	void OnTerminate(BehaviourStatus status);
};

#endif // GREATEST_SUSPECTED_THREAT_SET_H