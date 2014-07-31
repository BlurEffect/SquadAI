/* 
*  Kevin Meergans, SquadAI, 2014
*  Idle.h
*  An action behaviour that has the entity perform an idle behaviour.
*/

#ifndef IDLE_H
#define IDLE_H

// Includes
#include "Behaviour.h"

class Idle : public Behaviour
{
public:
	Idle(Entity* pEntity, const char* name);
	~Idle(void);

private:
	BehaviourStatus Update(float deltaTime);

	void OnInitialise(void);
	void OnTerminate(BehaviourStatus status);
};

#endif // IDLE_H