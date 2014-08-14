/* 
*  Kevin Meergans, SquadAI, 2014
*  DeterminePatrolTarget.h
*  An action behaviour that determines a patrol target for the entity and sets it as 
*  the active movement target.
*/

#ifndef DETERMINE_PATROL_TARGET_H
#define DETERMINE_PATROL_TARGET_H

// Includes
#include "UniversalIndividualBehaviour.h"

class DeterminePatrolTarget : public UniversalIndividualBehaviour
{
public:
	DeterminePatrolTarget(const char* name, Entity* pEntity);
	~DeterminePatrolTarget(void);

private:
	BehaviourStatus Update(float deltaTime);
};

#endif // DETERMINE_PATROL_TARGET_H