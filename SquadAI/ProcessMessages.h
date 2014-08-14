/* 
*  Kevin Meergans, SquadAI, 2014
*  ProcessMessages.h
*  An action behaviour that has the entity process all queued up messages
*  it received since the last update.
*/

#ifndef PROCESS_MESSAGES_H
#define PROCESS_MESSAGES_H

// Includes
#include "Behaviour.h"

class ProcessMessages : public Behaviour
{
public:
	ProcessMessages(Entity* pEntity, const char* name);
	~ProcessMessages(void);

private:
	BehaviourStatus Update(float deltaTime);
};

#endif // PROCESS_MESSAGES_H