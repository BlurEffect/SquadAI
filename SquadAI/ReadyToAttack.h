/* 
*  Kevin Meergans, SquadAI, 2014
*  ReadyToAttack.h
*  A condition behaviour that checks whether the entity is ready for attack.
*/

#ifndef READY_TO_ATTACK_H
#define READY_TO_ATTACK_H

// Includes
#include "UniversalIndividualBehaviour.h"

class ReadyToAttack : public UniversalIndividualBehaviour
{
public:
	ReadyToAttack(const char* name, Entity* pEntity);
	~ReadyToAttack(void);

private:
	BehaviourStatus Update(float deltaTime);
};

#endif // READY_TO_ATTACK_H