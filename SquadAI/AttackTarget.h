/* 
*  Kevin Meergans, SquadAI, 2014
*  AttackTarget.h
*  An action behaviour that has the entity attack the current attack target
*  position.
*/

#ifndef ATTACK_TARGET_H
#define ATTACK_TARGET_H

// Includes
#include "UniversalIndividualBehaviour.h"

class AttackTarget : public UniversalIndividualBehaviour
{
public:
	AttackTarget(const char* name, Entity* pEntity);
	~AttackTarget(void);

private:
	BehaviourStatus Update(float deltaTime);
	void OnTerminate(BehaviourStatus status);
};

#endif // ATTACK_TARGET_H