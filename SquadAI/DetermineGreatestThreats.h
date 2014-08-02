/* 
*  Kevin Meergans, SquadAI, 2014
*  DetermineGreatestThreats.h
*  An action behaviour that determines and sets the greatest known and suspected
*  threat for the entity.
*/

#ifndef DETERMINE_GREATEST_THREAT_H
#define DETERMINE_GREATEST_THREAT_H

// Includes
#include "Behaviour.h"

class DetermineGreatestThreats : public Behaviour
{
public:
	DetermineGreatestThreats(Entity* pEntity, const char* name);
	~DetermineGreatestThreats(void);

private:
	BehaviourStatus Update(float deltaTime);

	void OnInitialise(void);
	void OnTerminate(BehaviourStatus status);
};

#endif // DETERMINE_GREATEST_THREAT_H