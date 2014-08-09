/* 
*  Kevin Meergans, SquadAI, 2014
*  AttackTarget.h
*  An action behaviour that has the entity attack the current attack target
*  position.
*/

#ifndef ATTACK_TARGET_H
#define ATTACK_TARGET_H

// Includes
#include "Behaviour.h"

class AttackTarget : public Behaviour
{
public:
	AttackTarget(Entity* pEntity, const char* name);
	~AttackTarget(void);

private:
	BehaviourStatus Update(float deltaTime);
	void OnTerminate(BehaviourStatus status);
};

#endif // ATTACK_TARGET_H