/* 
*  Kevin Meergans, SquadAI, 2014
*  DetermineAttackTarget.h
*  An action behaviour that determines the position that the entity should
*  attack.
*/

#ifndef DETERMINE_ATTACK_TARGET_H
#define DETERMINE_ATTACK_TARGET_H

// Includes
#include "Behaviour.h"

class DetermineAttackTarget : public Behaviour
{
public:
	DetermineAttackTarget(Entity* pEntity, const char* name);
	~DetermineAttackTarget(void);

private:
	BehaviourStatus Update(float deltaTime);

	void OnInitialise(void);
	void OnTerminate(BehaviourStatus status);
};

#endif // DETERMINE_ATTACK_TARGET_H