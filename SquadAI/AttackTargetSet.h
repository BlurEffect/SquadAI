/* 
*  Kevin Meergans, SquadAI, 2014
*  AttackTargetSet.h
*  A condition behaviour that checks whether a target to attack was set
*  for the entity or not.
*/

#ifndef ATTACK_TARGET_SET_H
#define ATTACK_TARGET_SET_H

// Includes
#include "Behaviour.h"

class AttackTargetSet : public Behaviour
{
public:
	AttackTargetSet(Entity* pEntity, const char* name);
	~AttackTargetSet(void);

private:
	BehaviourStatus Update(float deltaTime);

	void OnInitialise(void);
	void OnTerminate(BehaviourStatus status);
};

#endif // ATTACK_TARGET_SET_H