/* 
*  Kevin Meergans, SquadAI, 2014
*  EntityAlive.h
*  A condition behaviour that checks whether the entity is alive or dead.
*/

#ifndef ENTITY_ALIVE_H
#define ENTITY_ALIVE_H

// Includes
#include "Behaviour.h"

class EntityAlive : public Behaviour
{
public:
	EntityAlive(Entity* pEntity, const char* name);
	~EntityAlive(void);

private:
	BehaviourStatus Update(float deltaTime);

	void OnInitialise(void);
	void OnTerminate(BehaviourStatus status);
};

#endif // ENTITY_ALIVE_H