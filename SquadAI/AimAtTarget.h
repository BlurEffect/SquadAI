/* 
*  Kevin Meergans, SquadAI, 2014
*  AimAtTarget.h
*  An action behaviour that has the entity aim at the currently set attack target position.
*/

#ifndef AIM_AT_TARGET_H
#define AIM_AT_TARGET_H

// Includes
#include "UniversalIndividualBehaviour.h"

class AimAtTarget : public UniversalIndividualBehaviour
{
public:
	AimAtTarget(const char* name, Entity* pEntity);
	~AimAtTarget(void);

private:
	BehaviourStatus Update(float deltaTime);
};

#endif // AIM_AT_TARGET_H