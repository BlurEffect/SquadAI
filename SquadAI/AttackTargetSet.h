/* 
*  Kevin Meergans, SquadAI, 2014
*  AttackTargetSet.h
*  A condition behaviour that checks whether a target to attack was set
*  for the entity or not.
*/

#ifndef ATTACK_TARGET_SET_H
#define ATTACK_TARGET_SET_H

// Includes
#include "UniversalIndividualBehaviour.h"

class AttackTargetSet : public UniversalIndividualBehaviour
{
public:
	AttackTargetSet(const char* name, Entity* pEntity);
	~AttackTargetSet(void);

private:
	BehaviourStatus Update(float deltaTime);
};

#endif // ATTACK_TARGET_SET_H