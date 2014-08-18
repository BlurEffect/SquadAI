/* 
*  Kevin Meergans, SquadAI, 2014
*  Communicator.cpp
*  This file contains an abstract base class that allows derived class to
*  communicate via events and messages.
*/

// Includes
#include "Communicator.h"

Communicator::Communicator(void)
{
}

Communicator::~Communicator(void)
{
	ResetCommunication();
}

//--------------------------------------------------------------------------------------
// Checks all outbox messages and deletes the ones that have been processed by the 
// respective receiver.
//--------------------------------------------------------------------------------------
void Communicator::SortOutProcessedMessages(void)
{
	if(!m_outboxMessages.empty())
	{
		std::vector<Message*>::iterator it = m_outboxMessages.begin();
		while(it != m_outboxMessages.end())
		{
			if((*it)->IsProcessed())
			{
				delete (*it);
				(*it) = nullptr;
				it = m_outboxMessages.erase(it);
			}else
			{
				++it;
			}
		}
	}
}

//--------------------------------------------------------------------------------------
// Sends a message to another communicator.
// Param1: A pointer to the receiving communicator that the message should be sent to.
// Param2: They type of message that should be sent.
// Param2: A pointer to the data that should be contained in the message.
//--------------------------------------------------------------------------------------
bool Communicator::SendMessage(Communicator* pReceiver, MessageType messageType, void* pData)
{
	if(!pData)
	{
		return false;
	}

	Message* pMessage = nullptr;

	switch(messageType)
	{
	case EntityKilledMessageType:
		pMessage = new EntityKilledMessage(*(reinterpret_cast<EntityKilledMessageData*>(pData)));
		break;
	case EnemySpottedMessageType:
		pMessage = new EnemySpottedMessage(*(reinterpret_cast<EnemySpottedMessageData*>(pData)));
		break;
	case LostSightOfEnemyMessageType:
		pMessage = new LostSightOfEnemyMessage(*(reinterpret_cast<LostSightOfEnemyMessageData*>(pData)));
		break;
	case UpdateEnemyPositionMessageType:
		pMessage = new UpdateEnemyPositionMessage(*(reinterpret_cast<UpdateEnemyPositionMessageData*>(pData)));
		break;
	case ScoreUpdateMessageType:
		pMessage = new ScoreUpdateMessage(*(reinterpret_cast<ScoreUpdateMessageData*>(pData)));
		break;
	case TimeUpdateMessageType:
		pMessage = new TimeUpdateMessage(*(reinterpret_cast<TimeUpdateMessageData*>(pData)));
		break;
	case FlagPickedUpMessageType:
		pMessage = new FlagPickedUpMessage(*(reinterpret_cast<FlagPickedUpMessageData*>(pData)));
		break;
	case FlagDroppedMessageType:
		pMessage = new FlagDroppedMessage(*(reinterpret_cast<FlagDroppedMessageData*>(pData)));
		break;
	case FlagReturnedMessageType:
		pMessage = new FlagReturnedMessage(*(reinterpret_cast<FlagReturnedMessageData*>(pData)));
		break;
	case FollowOrderMessageType:
		pMessage = new FollowOrderMessage(*(reinterpret_cast<FollowOrderMessageData*>(pData)));
		break;
	case UpdateOrderStateMessageType:
		pMessage = new UpdateOrderStateMessage(*(reinterpret_cast<UpdateOrderStateMessageData*>(pData)));
		break;
	case CancelOrderMessageType:
		pMessage = new CancelOrderMessage();
		break;
	}

	if(pMessage)
	{
		m_outboxMessages.push_back(pMessage);
		pReceiver->GetInboxMessages().push(pMessage);
		return true;
	}

	return false;
}

//--------------------------------------------------------------------------------------
// Sends an event to another communicator.
// Param1: A pointer to the receiving communicator that the event should be sent to.
// Param2: The type of event that should be sent.
// Param3: A pointer to the data that should be contained in the event.
//--------------------------------------------------------------------------------------
bool Communicator::SendEvent(Communicator* pReceiver, EventType eventType, void* pData)
{
	// Events have to be processed immediatly by the receiver and don't have
	// to be stored by the communicator.
	pReceiver->ProcessEvent(eventType, pData);

	return true;
}

//--------------------------------------------------------------------------------------
// Resets the communicator by clearing all received and sent messages.
//--------------------------------------------------------------------------------------
void Communicator::ResetCommunication(void)
{
	// Make sure to free all dynamically allocated messages
	for(std::vector<Message*>::iterator it = m_outboxMessages.begin(); it != m_outboxMessages.end(); ++it)
	{
		if(*it)
		{
			delete (*it);
			(*it) = nullptr;
		}
	}

	m_outboxMessages.clear();

	// Use swap idiom to clear the inbox
	std::queue<Message*> empty;
	std::swap( m_inboxMessages, empty );

}

//--------------------------------------------------------------------------------------
// Process a given event. Default implementation.
// Param1: The type of event.
// Param2: A pointer to the event data.
//--------------------------------------------------------------------------------------
void Communicator::ProcessEvent(EventType type, void* pEventData)
{
	// Do nothing
}

//--------------------------------------------------------------------------------------
// Processes all inbox messages that the communicator received.
//--------------------------------------------------------------------------------------
void Communicator::ProcessMessages(void)
{
	while(!m_inboxMessages.empty())
	{
		ProcessMessage(m_inboxMessages.front());
		m_inboxMessages.front()->SetProcessed(true);
		m_inboxMessages.pop();
	}
}

//--------------------------------------------------------------------------------------
// Processes a given message. Default implementation.
// Param1: A pointer to the message to process.
//--------------------------------------------------------------------------------------
void Communicator::ProcessMessage(Message* pMessage)
{
	// Do nothing
}

std::queue<Message*>& Communicator::GetInboxMessages(void)
{
	return m_inboxMessages;
}

std::vector<Message*>& Communicator::GetOutboxMessages(void)
{
	return m_outboxMessages;
}



