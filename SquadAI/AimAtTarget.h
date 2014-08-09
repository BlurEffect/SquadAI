/* 
*  Kevin Meergans, SquadAI, 2014
*  AimAtTarget.h
*  An action behaviour that has the entity aim at the currently set attack target position.
*/

#ifndef AIM_AT_TARGET_H
#define AIM_AT_TARGET_H

// Includes
#include "Behaviour.h"

class AimAtTarget : public Behaviour
{
public:
	AimAtTarget(Entity* pEntity, const char* name);
	~AimAtTarget(void);

private:
	BehaviourStatus Update(float deltaTime);
};

#endif // AIM_AT_TARGET_H