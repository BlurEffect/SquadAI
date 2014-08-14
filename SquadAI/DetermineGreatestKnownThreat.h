/* 
*  Kevin Meergans, SquadAI, 2014
*  DetermineGreatestKnownThreat.h
*  An action behaviour that determines and sets the greatest known
*  threat for the entity.
*/

#ifndef DETERMINE_GREATEST_KNOWN_THREAT_H
#define DETERMINE_GREATEST_KNOWN_THREAT_H

// Includes
#include "UniversalIndividualBehaviour.h"

class DetermineGreatestKnownThreat : public UniversalIndividualBehaviour
{
public:
	DetermineGreatestKnownThreat(const char* name, Entity* pEntity);
	~DetermineGreatestKnownThreat(void);

private:
	BehaviourStatus Update(float deltaTime);
};

#endif // DETERMINE_GREATEST_KNOWN_THREAT_H