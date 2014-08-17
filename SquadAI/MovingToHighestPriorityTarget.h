/* 
*  Kevin Meergans, SquadAI, 2014
*  MovingToHighestPriorityTarget.h
*  A condition behaviour that checks whether the entity is still moving towards the highest
*  priority movement target or if there is a new one available with higher priority that the
*  entity should move towards instead.
*/

#ifndef MOVING_TO_HIGHEST_PRIORITY_TARGET_H
#define MOVING_TO_HIGHEST_PRIORITY_TARGET_H

// Includes
#include "UniversalIndividualBehaviour.h"

class MovingToHighestPriorityTarget : public UniversalIndividualBehaviour
{
public:
	MovingToHighestPriorityTarget(const char* name, Entity* pEntity);
	~MovingToHighestPriorityTarget(void);

private:
	BehaviourStatus Update(float deltaTime);
};

#endif // MOVING_TO_HIGHEST_PRIORITY_TARGET_H