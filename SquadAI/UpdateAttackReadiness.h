/* 
*  Kevin Meergans, SquadAI, 2014
*  UpdateAttackReadiness.h
*  An action behaviour that updates and sets the attack readiness for
*  the entity.
*/

#ifndef UPDATE_ATTACK_READINESS_H
#define UPDATE_ATTACK_READINESS_H

// Includes
#include "Behaviour.h"

class UpdateAttackReadiness : public Behaviour
{
public:
	UpdateAttackReadiness(Entity* pEntity, const char* name);
	~UpdateAttackReadiness(void);

private:
	BehaviourStatus Update(float deltaTime);

	void OnInitialise(void);
	void OnTerminate(BehaviourStatus status);
};

#endif // UPDATE_ATTACK_READINESS_H