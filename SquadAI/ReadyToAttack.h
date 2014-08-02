/* 
*  Kevin Meergans, SquadAI, 2014
*  ReadyToAttack.h
*  A condition behaviour that checks whether the entity is ready for attack.
*/

#ifndef READY_TO_ATTACK_H
#define READY_TO_ATTACK_H

// Includes
#include "Behaviour.h"

class ReadyToAttack : public Behaviour
{
public:
	ReadyToAttack(Entity* pEntity, const char* name);
	~ReadyToAttack(void);

private:
	BehaviourStatus Update(float deltaTime);

	void OnInitialise(void);
	void OnTerminate(BehaviourStatus status);
};

#endif // READY_TO_ATTACK_H