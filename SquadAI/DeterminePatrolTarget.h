/* 
*  Kevin Meergans, SquadAI, 2014
*  DeterminePatrolTarget.h
*  An action behaviour that determines a patrol target for the entity and sets it as 
*  the active movement target.
*/

#ifndef DETERMINE_PATROL_TARGET_H
#define DETERMINE_PATROL_TARGET_H

// Includes
#include "Behaviour.h"

class DeterminePatrolTarget : public Behaviour
{
public:
	DeterminePatrolTarget(Entity* pEntity, const char* name);
	~DeterminePatrolTarget(void);

private:
	BehaviourStatus Update(float deltaTime);

	void OnInitialise(void);
	void OnTerminate(BehaviourStatus status);
};

#endif // DETERMINE_PATROL_TARGET_H