/* 
*  Kevin Meergans, SquadAI, 2014
*  LookAtTarget.h
*  An action behaviour that has the entity look at the currently set observation target position.
*/

#ifndef LOOK_AT_TARGET_H
#define LOOK_AT_TARGET_H

// Includes
#include "UniversalIndividualBehaviour.h"

class LookAtTarget : public UniversalIndividualBehaviour
{
public:
	LookAtTarget(const char* name, Entity* pEntity);
	~LookAtTarget(void);

private:
	BehaviourStatus Update(float deltaTime);
};

#endif // LOOK_AT_TARGET_H