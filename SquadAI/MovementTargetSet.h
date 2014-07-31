/* 
*  Kevin Meergans, SquadAI, 2014
*  MovementTargetSet.h
*  An condition behaviour that checks whether a movement target was set
*  for the entity or not.
*/

#ifndef MOVEMENT_TARGET_SET_H
#define MOVEMENT_TARGET_SET_H

// Includes
#include "Behaviour.h"

class MovementTargetSet : public Behaviour
{
public:
	MovementTargetSet(Entity* pEntity, const char* name);
	~MovementTargetSet(void);

private:
	BehaviourStatus Update(float deltaTime);

	void OnInitialise(void);
	void OnTerminate(BehaviourStatus status);
};

#endif // MOVEMENT_TARGET_SET_H