/* 
*  Kevin Meergans, SquadAI, 2014
*  DetermineGreatestKnownThreat.h
*  An action behaviour that determines and sets the greatest known
*  threat for the entity.
*/

#ifndef DETERMINE_GREATEST_KNOWN_THREAT_H
#define DETERMINE_GREATEST_KNOWN_THREAT_H

// Includes
#include "Behaviour.h"

class DetermineGreatestKnownThreat : public Behaviour
{
public:
	DetermineGreatestKnownThreat(Entity* pEntity, const char* name);
	~DetermineGreatestKnownThreat(void);

private:
	BehaviourStatus Update(float deltaTime);
};

#endif // DETERMINE_GREATEST_KNOWN_THREAT_H