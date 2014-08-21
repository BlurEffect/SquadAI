/* 
*  Kevin Meergans, SquadAI, 2014
*  GameContext.cpp
*  Abstract base class for all game contexts. Game context objects keep track
*  of the state of the game and control the game mode specific parts of the 
*  game in collaboration with the test environment.
*/

// Includes
#include "GameContext.h"
#include "TeamAI.h"

GameContext::GameContext(GameMode mode, float maxTime, float notifyTimeInterval, unsigned int winScore) : m_gameMode(mode),
																									  	  m_terminated(false),
																										  m_maxTime(maxTime),
																										  m_time(0.0f),
																										  m_notifyTimeInterval(0.0f),
																										  m_notifyTimer(0.0f),
																										  m_maxScore(winScore)
{
	for(unsigned int i = 0; i < NumberOfTeams-1; ++i)
	{
		m_score[i]      = 0;
		m_kills[i]      = 0;
		m_shotsFired[i] = 0;
	}
}

GameContext::~GameContext(void)
{
}

//--------------------------------------------------------------------------------------
// Adds a certain score to the total score of a specified team.
// Param1: The team, for which to increase the score.
// Param2: The score to add to the total score of the specified team.
//--------------------------------------------------------------------------------------
void GameContext::AddScore(EntityTeam team, unsigned int score)
{
	if(!m_terminated)
	{
		m_score[team] += score;

		// Notify the teams of the new score.
		ScoreUpdateMessageData data(team, m_score[team], GetMaxScore());
		BroadcastMessage(ScoreUpdateMessageType, &data);
	}
}

//--------------------------------------------------------------------------------------
// Adds a a number of kills to the total killcount of a team.
// Param1: The team, for which to increase the killcount.
// Param1: The team, a member of which was killed.
// Param2: The id of the entity that was killed.
//--------------------------------------------------------------------------------------
void GameContext::AddKill(EntityTeam team, EntityTeam victimTeam, unsigned long entityId)
{
	if(!m_terminated)
	{
		m_kills[team] += 1;

		// Notify the team AIs
		EntityKilledMessageData data(victimTeam, entityId);
		BroadcastMessage(EntityKilledMessageType, &data);
	}
}

//--------------------------------------------------------------------------------------
// Adds a certain number of fired shots the total shot count of a specified team.
// Param1: The team, for which to increase the shot count.
// Param2: The number of shots fired to add to the total shot count of the specified team.
//--------------------------------------------------------------------------------------
void GameContext::AddShotFired(EntityTeam team, unsigned int shotsFired)
{
	if(!m_terminated)
	{
		m_shotsFired[team] += shotsFired;
	}
}

//--------------------------------------------------------------------------------------
// Registers a certain team AI with the game context. Registered team AIs are informed
// by the context of game related events through messages.
// Param1: A pointer to the team AI to register.
//--------------------------------------------------------------------------------------
void GameContext::RegisterTeamAI(TeamAI* pTeamAI)
{
	m_teamAIs[pTeamAI->GetTeam()] = pTeamAI;
}

//--------------------------------------------------------------------------------------
// Broadcasts a message to all registered team AIs.
// Param1: The type of the message.
// Param2: A pointer to the contents of the message
//--------------------------------------------------------------------------------------
void GameContext::BroadcastMessage(MessageType type, void* pMessageData)
{
	for(unsigned int i = 0; i < NumberOfTeams-1; ++i)
	{
		if(m_teamAIs[i])
		{
			SendMessage(m_teamAIs[i], type, pMessageData);
		}
	}
}

//--------------------------------------------------------------------------------------
// Updates the game timer.
// Param1: The time to add to the timer. Usually the time passed since the last game update.
//--------------------------------------------------------------------------------------
void GameContext::Update(float deltaTime)
{
	m_time += deltaTime;
	m_notifyTimer += deltaTime;

	if(m_notifyTimer >= m_notifyTimeInterval)
	{
		// Notify teams of the time passed

		TimeUpdateMessageData data(GetTimeLeft(), GetMaxTime());
		BroadcastMessage(TimeUpdateMessageType, &data);

		m_notifyTimer = 0.0f;
	}

	if(m_time >= m_maxTime)
	{
		m_terminated = true;
	}

	SortOutProcessedMessages();
}

//--------------------------------------------------------------------------------------
// Resets the game state.
//--------------------------------------------------------------------------------------
void GameContext::Reset(void)
{
	m_terminated = false;
	m_time       = 0.0f;

	for(unsigned int i = 0; i < NumberOfTeams-1; ++i)
	{
		m_score[i]	    = 0;
		m_kills[i]		= 0;
		m_shotsFired[i] = 0;
	}

	ResetCommunication();
}

//--------------------------------------------------------------------------------------
// Process a given event. Default implementation.
// Param1: The type of event.
// Param2: A pointer to the event data.
//--------------------------------------------------------------------------------------
void GameContext::ProcessEvent(EventType type, void* pEventData)
{
	// No events expected, forward call to default implementation
	Communicator::ProcessEvent(type, pEventData);
}

//--------------------------------------------------------------------------------------
// Processes a given message.
// Param1: A pointer to the message to process.
// Returns true if this was the final communicator to process the message, false if the
// message was forwarded to another one.
//--------------------------------------------------------------------------------------
bool GameContext::ProcessMessage(Message* pMessage)
{
	// No messages expected, forward call to default implementation
	return Communicator::ProcessMessage(pMessage);
}

// Data access functions

GameMode GameContext::GetGameMode(void) const
{
	return m_gameMode;
}

bool GameContext::IsTerminated(void) const
{
	return m_terminated;
}

float GameContext::GetMaxTime(void) const
{
	return m_maxTime;
}

float GameContext::GetTime(void) const
{
	return m_time;
}

float GameContext::GetTimeLeft(void) const
{
	return (m_maxTime - m_time);
}

float GameContext::GetNotifyTimeInterval(void) const
{
	return m_notifyTimeInterval;
}

unsigned int GameContext::GetMaxScore(void) const
{
	return m_maxScore;
}

unsigned int GameContext::GetScore(EntityTeam team) const
{
	return m_score[team];
}

unsigned int GameContext::GetKills(EntityTeam team) const
{
	return m_kills[team];
}

unsigned int GameContext::GetShotsFired(EntityTeam team) const
{
	return m_shotsFired[team];
}

void GameContext::SetMaxTime(float time)
{
	m_maxTime = time;
}

void GameContext::SetNotifyTimeInterval(float timeInterval)
{
	m_notifyTimeInterval = timeInterval;
}

void GameContext::SetMaxScore(unsigned int score)
{
	m_maxScore = score;
}
