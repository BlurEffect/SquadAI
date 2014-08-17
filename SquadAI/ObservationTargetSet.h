/* 
*  Kevin Meergans, SquadAI, 2014
*  ObservationTargetSet.h
*  A condition behaviour that checks whether a observation target was set
*  for the entity or not.
*/

#ifndef OBSERVATION_TARGET_SET_H
#define OBSERVATION_TARGET_SET_H

// Includes
#include "UniversalIndividualBehaviour.h"

class ObservationTargetSet : public UniversalIndividualBehaviour
{
public:
	ObservationTargetSet(const char* name, Entity* pEntity);
	~ObservationTargetSet(void);

private:
	BehaviourStatus Update(float deltaTime);
};

#endif // OBSERVATION_TARGET_SET_H