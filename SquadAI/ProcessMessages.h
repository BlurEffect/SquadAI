/* 
*  Kevin Meergans, SquadAI, 2014
*  ProcessMessages.h
*  An action behaviour that has the entity process any queued up messages/events.
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

	void OnInitialise(void);
	void OnTerminate(BehaviourStatus status);
};

#endif // PROCESS_MESSAGES_H