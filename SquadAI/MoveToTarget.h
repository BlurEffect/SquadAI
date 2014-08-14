/* 
*  Kevin Meergans, SquadAI, 2014
*  MoveToTarget.h
*  An action behaviour that has the entity move to the current movement
*  target position.
*/

#ifndef MOVE_TO_TARGET_H
#define MOVE_TO_TARGET_H

// Includes
#include "UniversalIndividualBehaviour.h"

class MoveToTarget : public UniversalIndividualBehaviour
{
public:
	MoveToTarget(const char* name, Entity* pEntity);
	~MoveToTarget(void);

private:
	BehaviourStatus Update(float deltaTime);
	void OnTerminate(BehaviourStatus status);
};

#endif // MOVE_TO_TARGET_H