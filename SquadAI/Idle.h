/* 
*  Kevin Meergans, SquadAI, 2014
*  Idle.h
*  An action behaviour that has the entity perform an idle behaviour.
*/

#ifndef IDLE_H
#define IDLE_H

// Includes
#include "UniversalIndividualBehaviour.h"

class Idle : public UniversalIndividualBehaviour
{
public:
	Idle(const char* name, Entity* pEntity);
	~Idle(void);

private:
	BehaviourStatus Update(float deltaTime);
};

#endif // IDLE_H