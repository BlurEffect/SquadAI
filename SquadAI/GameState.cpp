/* 
*  Kevin Meergans, SquadAI, 2014
*  GameState.cpp
*  This class bundles the current state of a Multiflag CTF game.
*/

// Includes
#include "GameState.h"

GameState::GameState(void) : m_terminated(false),
							 m_maxTime(0.0f),
							 m_time(0.0f),
							 m_maxScore(0.0f)
{
	for(unsigned int i = 0; i < NumberOfTeams-1; ++i)
	{
		m_score[i]      = 0;
		m_kills[i]      = 0;
		m_shotsFired[i] = 0;
	}
}

GameState::~GameState(void)
{
}

//--------------------------------------------------------------------------------------
// Adds a certain score to the total score of a specified team.
// Param1: The time a round of the game lasts.
// Param2: The score required by a team to win the game.
//--------------------------------------------------------------------------------------
void GameState::Initialise(float maxTime, unsigned int winScore)
{
	m_maxTime  = maxTime;
	m_maxScore = winScore;
}

//--------------------------------------------------------------------------------------
// Adds a certain score to the total score of a specified team.
// Param1: The team, for which to increase the score.
// Param2: The score to add to the total score of the specified team.
//--------------------------------------------------------------------------------------
void GameState::AddScore(EntityTeam team, unsigned int score)
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
void GameState::AddKills(EntityTeam team, unsigned int kills)
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
void GameState::AddShotFired(EntityTeam team, unsigned int shotsFired)
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
void GameState::UpdateTimer(float deltaTime)
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
void GameState::Reset(void)
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

bool GameState::IsTerminated(void) const
{
	return m_terminated;
}

float GameState::GetMaxTime(void) const
{
	return m_maxTime;
}

float GameState::GetTime(void) const
{
	return m_time;
}

float GameState::GetTimeLeft(void) const
{
	return (m_maxTime - m_time);
}

unsigned int GameState::GetMaxScore(void) const
{
	return m_maxScore;
}

unsigned int GameState::GetScore(EntityTeam team) const
{
	return m_score[team];
}

unsigned int GameState::GetKills(EntityTeam team) const
{
	return m_kills[team];
}

unsigned int GameState::GetShotsFired(EntityTeam team) const
{
	return m_shotsFired[team];
}

void GameState::SetMaxTime(float time)
{
	m_maxTime = time;
}

void GameState::SetMaxScore(float score)
{
	m_maxScore = score;
}
