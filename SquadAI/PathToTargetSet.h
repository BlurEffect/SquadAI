/* 
*  Kevin Meergans, SquadAI, 2014
*  PathToTargetSet.h
*  A condition behaviour that checks whether a path to the movement target was set
*  for the entity or not.
*/

#ifndef PATH_TO_TARGET_SET_H
#define PATH_TO_TARGET_SET_H

// Includes
#include "UniversalIndividualBehaviour.h"

class PathToTargetSet : public UniversalIndividualBehaviour
{
public:
	PathToTargetSet(const char* name, Entity* pEntity);
	~PathToTargetSet(void);

private:
	BehaviourStatus Update(float deltaTime);
};

#endif // PATH_TO_TARGET_SET_H