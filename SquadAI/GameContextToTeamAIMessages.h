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
struct ScoreUpdateMessageData
{
	ScoreUpdateMessageData(EntityTeam team, unsigned int score, unsigned int maxScore) : m_team(team),
																						 m_score(score),
																						 m_maxScore(maxScore)
	{}

	EntityTeam   m_team;	 // The team, whose score is being updated
	unsigned int m_score;	 // The new score of the team
	unsigned int m_maxScore; // The score required by a team for victory
};

//--------------------------------------------------------------------------------------
// Message sent by the game context to update the team AI's
// knowledge of the scores for the competing teams.
//--------------------------------------------------------------------------------------
class ScoreUpdateMessage : public Message
{
public:
	ScoreUpdateMessage(const ScoreUpdateMessageData& data) : Message(ScoreUpdateMessageType),
															 m_data(data)
	{}

	const ScoreUpdateMessageData& GetData(void) const { return m_data; }

private:
	ScoreUpdateMessageData m_data; // The contents of the message
};


//--------------------------------------------------------------------------------------
// Contains data required for a message by the game context to the team AI at regular
// intervals to inform it about the time left for the current match.
//--------------------------------------------------------------------------------------
struct TimeUpdateMessageData
{
	TimeUpdateMessageData(float timeLeft, float maxTime) : m_timeLeft(timeLeft),
														   m_maxTime(maxTime)
	{}
	
	float m_timeLeft; // The time left for the current round
	float m_maxTime;  // The time a round lasts in total

};

//--------------------------------------------------------------------------------------
// Message by the game context to the team AI at regular
// intervals to inform it about the time left for the current match.
//--------------------------------------------------------------------------------------
class TimeUpdateMessage : public Message
{
public:
	TimeUpdateMessage(const TimeUpdateMessageData& data) : Message(TimeUpdateMessageType),
														   m_data(data)
	{}

	const TimeUpdateMessageData& GetData(void) const { return m_data; }

private:
	TimeUpdateMessageData m_data; // The contents of the message
};


// Messages specific to Multiflag CTF matches

//--------------------------------------------------------------------------------------
// Contains data required for a message by the game context to the team AI to notify it
// of a flag having been picked up, that is stolen by a member of a hostile team.
//--------------------------------------------------------------------------------------
struct FlagPickedUpMessageData
{
public:
	FlagPickedUpMessageData(EntityTeam flagOwner, unsigned long carrierId) : m_flagOwner(flagOwner),
																			 m_carrierId(carrierId)
	{}
	
	EntityTeam	  m_flagOwner; // The team, whose flag has been stolen
	unsigned long m_carrierId; // The id of the entity that stole the flag

};

//--------------------------------------------------------------------------------------
// Message by the game context to the team AI to notify it
// of a flag having been picked up, that is stolen by a member of a hostile team.
//--------------------------------------------------------------------------------------
class FlagPickedUpMessage : public Message
{
public:
	FlagPickedUpMessage(const FlagPickedUpMessageData& data) : Message(FlagPickedUpMessageType),
														       m_data(data)
	{}

	const FlagPickedUpMessageData& GetData(void) const { return m_data; }

private:
	FlagPickedUpMessageData m_data; // The contents of the message
};


//--------------------------------------------------------------------------------------
// Contains data required for a message by the game context to the team AI to notify it
// of a flag having been dropped.
//--------------------------------------------------------------------------------------
struct FlagDroppedMessageData
{
	FlagDroppedMessageData(EntityTeam flagOwner, const XMFLOAT2& dropPosition) : m_flagOwner(flagOwner),
																				 m_dropPosition(dropPosition)
	{}

	EntityTeam m_flagOwner;    // The team, whose flag has been dropped
	XMFLOAT2   m_dropPosition; // The position, at which the flag was dropped
};

//--------------------------------------------------------------------------------------
// Message by the game context to the team AI to notify it
// of a flag having been dropped.
//--------------------------------------------------------------------------------------
class FlagDroppedMessage : public Message
{
public:
	FlagDroppedMessage(const FlagDroppedMessageData& data) : Message(FlagDroppedMessageType),
														       m_data(data)
	{}

	const FlagDroppedMessageData& GetData(void) const { return m_data; }

private:
	FlagDroppedMessageData m_data; // The contents of the message
};

//--------------------------------------------------------------------------------------
// Contains data required for a message by the game context to the team AI to notify it
// of a flag having been returned to its base.
//--------------------------------------------------------------------------------------
struct FlagReturnedMessageData 
{
public:
	FlagReturnedMessageData(EntityTeam flagOwner) : m_flagOwner(flagOwner)
	{}

	EntityTeam m_flagOwner; // The team, whose flag has been returned
};

//--------------------------------------------------------------------------------------
// Message by the game context to the team AI to notify it
// of a flag having been returned to its base.
//--------------------------------------------------------------------------------------
class FlagReturnedMessage : public Message
{
public:
	FlagReturnedMessage(const FlagReturnedMessageData& data) : Message(FlagReturnedMessageType),
														       m_data(data)
	{}

	const FlagReturnedMessageData& GetData(void) const { return m_data; }

private:
	FlagReturnedMessageData m_data; // The contents of the message
};

#endif // GAME_CONTEXT_TO_TEAM_AI_MESSAGES_H