/* 
*  Kevin Meergans, SquadAI, 2014
*  DetermineObservationTarget.h
*  An action behaviour that determines an observation target for the entity and sets it as 
*  the active observation target.
*/

#ifndef DETERMINE_OBSERVATION_TARGET_H
#define DETERMINE_OBSERVATION_TARGET_H

// Includes
#include "UniversalIndividualBehaviour.h"

class DetermineObservationTarget : public UniversalIndividualBehaviour
{
public:
	DetermineObservationTarget(const char* name, Entity* pEntity);
	~DetermineObservationTarget(void);

private:
	BehaviourStatus Update(float deltaTime);
};

#endif // DETERMINE_OBSERVATION_TARGET_H