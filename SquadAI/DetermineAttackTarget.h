/* 
*  Kevin Meergans, SquadAI, 2014
*  DetermineAttackTarget.h
*  An action behaviour that determines the position that the entity should
*  attack.
*/

#ifndef DETERMINE_ATTACK_TARGET_H
#define DETERMINE_ATTACK_TARGET_H

// Includes
#include "UniversalIndividualBehaviour.h"

class DetermineAttackTarget : public UniversalIndividualBehaviour
{
public:
	DetermineAttackTarget(const char* name, Entity* pEntity);
	~DetermineAttackTarget(void);

private:
	BehaviourStatus Update(float deltaTime);
};

#endif // DETERMINE_ATTACK_TARGET_H