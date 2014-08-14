/* 
*  Kevin Meergans, SquadAI, 2014
*  UpdateAttackReadiness.h
*  An action behaviour that updates and sets the attack readiness for
*  the entity.
*/

#ifndef UPDATE_ATTACK_READINESS_H
#define UPDATE_ATTACK_READINESS_H

// Includes
#include "UniversalIndividualBehaviour.h"

class UpdateAttackReadiness : public UniversalIndividualBehaviour
{
public:
	UpdateAttackReadiness(const char* name, Entity* pEntity);
	~UpdateAttackReadiness(void);

private:
	BehaviourStatus Update(float deltaTime);
};

#endif // UPDATE_ATTACK_READINESS_H