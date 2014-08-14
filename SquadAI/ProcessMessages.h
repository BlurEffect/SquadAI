/* 
*  Kevin Meergans, SquadAI, 2014
*  ProcessMessages.h
*  An action behaviour that has the entity process all queued up messages
*  it received since the last update.
*/

#ifndef PROCESS_MESSAGES_H
#define PROCESS_MESSAGES_H

// Includes
#include "UniversalIndividualBehaviour.h"

class ProcessMessages : public UniversalIndividualBehaviour
{
public:
	ProcessMessages(const char* name, Entity* pEntity);
	~ProcessMessages(void);

private:
	BehaviourStatus Update(float deltaTime);
};

#endif // PROCESS_MESSAGES_H