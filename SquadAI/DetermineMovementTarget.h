/* 
*  Kevin Meergans, SquadAI, 2014
*  DetermineMovementTarget.h
*  An action behaviour that determines a movement target for the entity and sets it as 
*  the active movement target.
*/

#ifndef DETERMINE_MOVEMENT_TARGET_H
#define DETERMINE_MOVEMENT_TARGET_H

// Includes
#include "UniversalIndividualBehaviour.h"

class DetermineMovementTarget : public UniversalIndividualBehaviour
{
public:
	DetermineMovementTarget(const char* name, Entity* pEntity);
	~DetermineMovementTarget(void);

private:
	BehaviourStatus Update(float deltaTime);
};

#endif // DETERMINE_MOVEMENT_TARGET_H