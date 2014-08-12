/* 
*  Kevin Meergans, SquadAI, 2014
*  GameContextToTeamAIMessages.h
*  This file contains all messages that are sent from the game context to the team AI.
*/

#ifndef GAME_CONTEXT_TO_TEAM_AI_MESSAGES_H
#define GAME_CONTEXT_TO_TEAM_AI_MESSAGES_H

// Includes
#include "Message.h"
#include "ObjectTypes.h"

//--------------------------------------------------------------------------------------
// Contains data required for a message sent by the game context to update the team AI's
// knowledge of the scores for the competing teams.
//--------------------------------------------------------------------------------------
class ScoreUpdateMessage : public Message
{
public:
	ScoreUpdateMessage(EntityTeam team, unsigned int score, unsigned int maxScore) : Message(ScoreUpdateMessageType),
																					 m_team(team),
																					 m_score(score),
																					 m_maxScore(maxScore)
	{}
	
	EntityTeam	 GetTeam(void) const { return m_team; }
	unsigned int GetScore(void) const { return m_score; }
	unsigned int GetMaxScore(void) const { return m_maxScore; }

private:
	EntityTeam   m_team;	 // The team, whose score is being updated
	unsigned int m_score;	 // The new score of the team
	unsigned int m_maxScore; // The score required by a team for victory
};

//--------------------------------------------------------------------------------------
// Contains data required for a message by the game context to the team AI at regular
// intervals to inform it about the time left for the current match.
//--------------------------------------------------------------------------------------
class TimeUpdateMessage : public Message
{
public:
	TimeUpdateMessage(float timeLeft, float maxTime) : Message(TimeUpdateMessageType),
													   m_timeLeft(timeLeft),
													   m_maxTime(maxTime)
	{}
	
	float GetTimeLeft(void) const { return m_timeLeft; }
	float GetMaxTime(void) const { return m_maxTime; }

private:
	float m_timeLeft; // The time left for the current round
	float m_maxTime;  // The time a round lasts in total

};

// Messages specific to Multiflag CTF matches

//--------------------------------------------------------------------------------------
// Contains data required for a message by the game context to the team AI to notify it
// of a flag having been picked up, that is stolen by a member of a hostile team.
//--------------------------------------------------------------------------------------
class FlagPickedUpMessage : public Message
{
public:
	FlagPickedUpMessage(EntityTeam flagOwner, unsigned long carrierId) : Message(FlagPickedUpMessageType),
																		 m_flagOwner(flagOwner),
																		 m_carrierId(carrierId)
	{}
	
	EntityTeam    GetFlagOwner(void) const { return m_flagOwner; }
	unsigned long GetCarrierId(void) const { return m_carrierId; }

private:
	EntityTeam	  m_flagOwner; // The team, whose flag has been stolen
	unsigned long m_carrierId; // The id of the entity that stole the flag

};

//--------------------------------------------------------------------------------------
// Contains data required for a message by the game context to the team AI to notify it
// of a flag having been dropped.
//--------------------------------------------------------------------------------------
class FlagDroppedMessage : public Message
{
public:
	FlagDroppedMessage(EntityTeam flagOwner, const XMFLOAT2& dropPosition) : Message(FlagDroppedMessageType),
																			 m_flagOwner(flagOwner),
																			 m_dropPosition(dropPosition)
	{}
	
	EntityTeam      GetFlagOwner(void) const { return m_flagOwner; }
	const XMFLOAT2& GetDropPosition(void) const { return m_dropPosition; }

private:
	EntityTeam m_flagOwner;    // The team, whose flag has been dropped
	XMFLOAT2   m_dropPosition; // The position, at which the flag was dropped
};

//--------------------------------------------------------------------------------------
// Contains data required for a message by the game context to the team AI to notify it
// of a flag having been returned to its base.
//--------------------------------------------------------------------------------------
class FlagReturnedMessage : public Message
{
public:
	FlagReturnedMessage(EntityTeam flagOwner) : Message(FlagReturnedMessageType),
												m_flagOwner(flagOwner)
	{}
	
	EntityTeam GetFlagOwner(void) const { return m_flagOwner; }

private:
	EntityTeam m_flagOwner; // The team, whose flag has been returned
};


#endif // GAME_CONTEXT_TO_TEAM_AI_MESSAGES_H