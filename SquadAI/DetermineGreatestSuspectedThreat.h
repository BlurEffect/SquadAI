/* 
*  Kevin Meergans, SquadAI, 2014
*  DetermineGreatestSuspectedThreat.h
*  An action behaviour that determines and sets the greatest suspected
*  threat for the entity.
*/

#ifndef DETERMINE_GREATEST_SUSPECTED_THREAT_H
#define DETERMINE_GREATEST_SUSPECTED_THREAT_H

// Includes
#include "UniversalIndividualBehaviour.h"

class DetermineGreatestSuspectedThreat : public UniversalIndividualBehaviour
{
public:
	DetermineGreatestSuspectedThreat(const char* name, Entity* pEntity);
	~DetermineGreatestSuspectedThreat(void);

private:
	BehaviourStatus Update(float deltaTime);
};

#endif // DETERMINE_GREATEST_SUSPECTED_THREAT_H