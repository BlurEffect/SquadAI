/* 
*  Kevin Meergans, SquadAI, 2014
*  MultiflagCTFTeamAI.cpp
*  Team AI that controls its entities to play Multiflag Capture-the-Flag matches.
*/

#include "MultiflagCTFTeamAI.h"
#include "TestEnvironment.h"

MultiflagCTFTeamAI::MultiflagCTFTeamAI(EntityTeam team, TestEnvironment* pEnvironment) : TeamAI(team, pEnvironment)
{
}

MultiflagCTFTeamAI::~MultiflagCTFTeamAI(void)
{
}

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