/* 
*  Kevin Meergans, SquadAI, 2014
*  DetermineGreatestSuspectedThreat.h
*  An action behaviour that determines and sets the greatest suspected
*  threat for the entity.
*/

#ifndef DETERMINE_GREATEST_SUSPECTED_THREAT_H
#define DETERMINE_GREATEST_SUSPECTED_THREAT_H

// Includes
#include "Behaviour.h"

class DetermineGreatestSuspectedThreat : public Behaviour
{
public:
	DetermineGreatestSuspectedThreat(Entity* pEntity, const char* name);
	~DetermineGreatestSuspectedThreat(void);

private:
	BehaviourStatus Update(float deltaTime);

	void OnInitialise(void);
	void OnTerminate(BehaviourStatus status);
};

#endif // DETERMINE_GREATEST_SUSPECTED_THREAT_H