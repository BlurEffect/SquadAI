/* 
*  Kevin Meergans, SquadAI, 2014
*  GameContext.cpp
*  Abstract base class for all game contexts. Game context objects keep track
*  of the state of the game and control the game mode specific parts of the 
*  game in collaboration with the test environment.
*/

// Includes
#include "GameContext.h"

GameContext::GameContext(GameMode mode, float maxTime, unsigned int winScore) : m_gameMode(mode),
																			    m_terminated(false),
																			    m_maxTime(maxTime),
																			    m_time(0.0f),
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
	}
}

//--------------------------------------------------------------------------------------
// Adds a a number of kills to the total killcount of a team.
// Param1: The team, for which to increase the killcount.
// Param2: The number of kills add to the total killcount of the specified team.
//--------------------------------------------------------------------------------------
void GameContext::AddKills(EntityTeam team, unsigned int kills)
{
	if(!m_terminated)
	{
		m_kills[team] += kills;
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
// Updates the game timer.
// Param1: The time to add to the timer. Usually the time passed since the last game update.
//--------------------------------------------------------------------------------------
void GameContext::Update(float deltaTime)
{
	m_time += deltaTime;
	
	if(m_time >= m_maxTime)
	{
		m_terminated = true;
	}
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

void GameContext::SetMaxScore(float score)
{
	m_maxScore = score;
}
