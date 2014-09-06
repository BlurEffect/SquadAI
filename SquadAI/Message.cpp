/* 
*  Kevin Meergans, SquadAI, 2014
*  Message.cpp
*  This file contains an enumeration of available message types and the abstract base
*  class, from which all specific messages have to inherit. Messages contain information
*  that either entities and team AI exchange to simulate human communication within a team 
*  or information that is shared by the game itself, such as notifications of entities that
*  were killed or objectives that were reached.
*/

// Includes
#include "Message.h"

Message::Message(MessageType messageType) : m_messageType(messageType),
											m_numberOfReceivers(0),
											m_processedCount(0)
{
}

Message::~Message(void)
{
}

MessageType Message::GetType(void) const 
{ 
	return m_messageType; 
}

unsigned int Message::GetNumberOfReceivers(void) const 
{ 
	return m_numberOfReceivers; 
}

unsigned int Message::GetProcessedCount(void) const 
{ 
	return m_processedCount; 
}

bool Message::IsProcessed(void) const 
{ 
	return m_processedCount == m_numberOfReceivers; 
}

void Message::IncreaseProcessedCount(void) 
{ 
	++m_processedCount; 
}

void Message::IncreaseReceiverCount(void) 
{ 
	++m_numberOfReceivers; 
}
