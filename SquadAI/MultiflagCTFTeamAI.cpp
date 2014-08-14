/* 
*  Kevin Meergans, SquadAI, 2014
*  MultiflagCTFTeamAI.cpp
*  Team AI that controls its entities to play Multiflag Capture-the-Flag matches.
*/

#include "MultiflagCTFTeamAI.h"
#include "TestEnvironment.h"

MultiflagCTFTeamAI::MultiflagCTFTeamAI(void) : TeamAI()
{
}

MultiflagCTFTeamAI::~MultiflagCTFTeamAI(void)
{
}

//--------------------------------------------------------------------------------------
// Initialises the multiflag CTF AI.
// Param1: The team that this Ai will control.
// Param2: A pointer to the test environment, in which the matches take place.
//--------------------------------------------------------------------------------------
bool MultiflagCTFTeamAI::Initialise(EntityTeam team, TestEnvironment* pEnvironment)
{
	if(TeamAI::Initialise(team, pEnvironment))
	{
		m_pBehaviour = BehaviourFactory::CreateBehaviourTree(SimpleTeamMultiflagCTFTree, this);
		if(!m_pBehaviour)
		{
			return false;
		}
	}

	return false;
}

//--------------------------------------------------------------------------------------
// Processes all inbox messages that the team AI received.
// Param1: A pointer to the message to process.
//--------------------------------------------------------------------------------------
void MultiflagCTFTeamAI::ProcessMessage(Message* pMessage)
{
	switch(pMessage->GetType())
	{
	case FlagPickedUpMessageType:
		{
		FlagPickedUpMessage* pMsg = reinterpret_cast<FlagPickedUpMessage*>(pMessage);
		m_flagData[pMsg->GetData().m_flagOwner].m_state     = Stolen;
		m_flagData[pMsg->GetData().m_flagOwner].m_carrierId = pMsg->GetData().m_carrierId;
		break;
		}
	case FlagDroppedMessageType:
		{
		FlagDroppedMessage* pMsg = reinterpret_cast<FlagDroppedMessage*>(pMessage);
		m_flagData[pMsg->GetData().m_flagOwner].m_state     = Dropped;
		m_flagData[pMsg->GetData().m_flagOwner].m_position  = pMsg->GetData().m_dropPosition;
		m_flagData[pMsg->GetData().m_flagOwner].m_carrierId = 0;
		break;
		}
	case FlagReturnedMessageType:
		{
		FlagReturnedMessage* pMsg = reinterpret_cast<FlagReturnedMessage*>(pMessage);
		m_flagData[pMsg->GetData().m_flagOwner].m_state     = InBase;
		m_flagData[pMsg->GetData().m_flagOwner].m_position  = m_flagData[pMsg->GetData().m_flagOwner].m_basePosition;
		m_flagData[pMsg->GetData().m_flagOwner].m_carrierId = 0;
		break;
		}
	default:
		// Forward other messages to the base class implementation of the function
		TeamAI::ProcessMessage(pMessage);
	}
}

//--------------------------------------------------------------------------------------
// Process a given event. Default implementation.
// Param1: The type of event.
// Param2: A pointer to the event data.
//--------------------------------------------------------------------------------------
void MultiflagCTFTeamAI::ProcessEvent(EventType type, void* pEventData)
{
	// Not expecting any events, forward to the base class
	TeamAI::ProcessEvent(type, pEventData);
}
/*
//--------------------------------------------------------------------------------------
// Processes a message sent to the team AI.
// Param1: A pointer to the message to process.
//--------------------------------------------------------------------------------------
void MultiflagCTFTeamAI::ProcessMessage(Message* pMessage)
{
	switch(pMessage->GetType())
	{
	case FlagPickedUpMessageType:
		{
		FlagPickedUpMessage* pMsg = reinterpret_cast<FlagPickedUpMessage*>(pMessage);
		m_flagData[pMsg->GetFlagOwner()].m_state     = Stolen;
		m_flagData[pMsg->GetFlagOwner()].m_carrierId = pMsg->GetCarrierId();
		break;
		}
	case FlagDroppedMessageType:
		{
		FlagDroppedMessage* pMsg = reinterpret_cast<FlagDroppedMessage*>(pMessage);
		m_flagData[pMsg->GetFlagOwner()].m_state     = Dropped;
		m_flagData[pMsg->GetFlagOwner()].m_position  = pMsg->GetDropPosition();
		m_flagData[pMsg->GetFlagOwner()].m_carrierId = 0;
		break;
		}
	case FlagReturnedMessageType:
		{
		FlagReturnedMessage* pMsg = reinterpret_cast<FlagReturnedMessage*>(pMessage);
		m_flagData[pMsg->GetFlagOwner()].m_state     = InBase;
		m_flagData[pMsg->GetFlagOwner()].m_position  = m_flagData[pMsg->GetFlagOwner()].m_basePosition;
		m_flagData[pMsg->GetFlagOwner()].m_carrierId = 0;
		break;
		}
	default:
		// Forward other messages to the base class implementation of the function
		TeamAI::ProcessMessage(pMessage);
	}
}
*/




//--------------------------------------------------------------------------------------
// Resets the team AI after a completed match.
//--------------------------------------------------------------------------------------
void MultiflagCTFTeamAI::Reset(void)
{
	for(unsigned int i = 0; i < NumberOfTeams-1; ++i)
	{
		m_flagData[i].m_state     = InBase;
		m_flagData[i].m_position  = m_flagData[i].m_basePosition;
		m_flagData[i].m_carrierId = 0;
	}

	TeamAI::Reset();
}