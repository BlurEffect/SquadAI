/* 
*  Kevin Meergans, SquadAI, 2014
*  Communicator.h
*  This file contains an abstract base class that allows derived class to
*  communicate via events and messages.
*/

#ifndef COMMUNICATOR_H
#define COMMUNICATOR_H

// Includes
#include <queue>
#include <vector>
#include <algorithm>
#include "Message.h"
#include "EntityToTeamAIMessages.h"
#include "GameContextToTeamAIMessages.h"
#include "TestEnvironmentToEntityMessages.h"
#include "TeamAIToEntityMessages.h"
#include "EventTypes.h"
#include "EventDataStructures.h"

class Communicator
{
public:

	Communicator(void);
	virtual ~Communicator(void);

	void SortOutProcessedMessages(void);
	bool SendMessage(Communicator* pReceiver, MessageType messageType, void* pData);
	void ForwardMessage(Communicator* pReceiver, Message* pMessage);
	bool SendEvent(Communicator* pReceiver, EventType eventType, void* pData);
	void ResetCommunication(void);
	void ProcessMessages(void);

	void ResetInbox(void);

	std::queue<Message*>&  GetInboxMessages(void);
	std::vector<Message*>& GetOutboxMessages(void);

	virtual void ProcessEvent(EventType type, void* pEventData) = 0;

protected:
	virtual void ProcessMessage(Message* pMessage) = 0;

private:

	std::queue<Message*>  m_inboxMessages;  // The unprocessed received messages of this communicator
	std::vector<Message*> m_outboxMessages; // The messages sent by this communicator, which still await processing

};

#endif // COMMUNICATOR_H

