/* 
*  Kevin Meergans, SquadAI, 2014
*  DeterminePathToTarget.h
*  An action behaviour that determines a path to a movement target for the entity and sets it as 
*  the active path to that target.
*/

#ifndef DETERMINE_PATH_TO_TARGET_H
#define DETERMINE_PATH_TO_TARGET_H

// Includes
#include "UniversalIndividualBehaviour.h"

class DeterminePathToTarget : public UniversalIndividualBehaviour
{
public:
	DeterminePathToTarget(const char* name, Entity* pEntity);
	~DeterminePathToTarget(void);

private:
	BehaviourStatus Update(float deltaTime);
};

#endif // DETERMINE_PATH_TO_TARGET_H