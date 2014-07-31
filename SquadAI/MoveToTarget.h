/* 
*  Kevin Meergans, SquadAI, 2014
*  MoveToTarget.h
*  An action behaviour that has the entity move to the current movement
*  target position.
*/

#ifndef MOVE_TO_TARGET_H
#define MOVE_TO_TARGET_H

// Includes
#include "Behaviour.h"

class MoveToTarget : public Behaviour
{
public:
	MoveToTarget(Entity* pEntity, const char* name);
	~MoveToTarget(void);

private:
	BehaviourStatus Update(float deltaTime);

	void OnInitialise(void);
	void OnTerminate(BehaviourStatus status);
};

#endif // MOVE_TO_TARGET_H