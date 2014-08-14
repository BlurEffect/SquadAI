/* 
*  Kevin Meergans, SquadAI, 2014
*  EntityAlive.h
*  A condition behaviour that checks whether the entity is alive or dead.
*/

#ifndef ENTITY_ALIVE_H
#define ENTITY_ALIVE_H

// Includes
#include "UniversalIndividualBehaviour.h"

class EntityAlive : public UniversalIndividualBehaviour
{
public:
	EntityAlive(const char* name, Entity* pEntity);
	~EntityAlive(void);

private:
	BehaviourStatus Update(float deltaTime);
};

#endif // ENTITY_ALIVE_H