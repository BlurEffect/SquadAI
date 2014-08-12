/* 
*  Kevin Meergans, SquadAI, 2014
*  GameContext.h
*  Abstract base class for all game contexts. Game context objects keep track
*  of the state of the game and control the game mode specific parts of the 
*  game in collaboration with the test environment.
*/

#ifndef GAME_CONTEXT_H
#define GAME_CONTEXT_H

// Includes
#include "ObjectTypes.h"
#include "Message.h"
#include "TestEnvironmentMessages.h"

//--------------------------------------------------------------------------------------
// Identifies the specific type of game context that is the game mode it is associated to.
//--------------------------------------------------------------------------------------
enum GameMode
{
	MultiflagCTF  // Identifier for Capture-The-Flag matches, in which each team has a flag to protect
};

class GameContext
{
public:
	GameContext(GameMode mode, float maxTime, unsigned int winScore);
	virtual ~GameContext(void) = 0;

	virtual void Update(float deltaTime);
	virtual void Reset(void);

	void AddScore(EntityTeam team, unsigned int score);
	void AddKills(EntityTeam team, unsigned int kills);
	void AddShotFired(EntityTeam team, unsigned int shotsFired);

	virtual void ProcessMessage(Message* pMessage) = 0;

	// Data access functions

	GameMode     GetGameMode(void) const;
	bool		 IsTerminated(void) const;
	float		 GetMaxTime(void) const;
	float        GetTime(void) const;
	float        GetTimeLeft(void) const;
	unsigned int GetMaxScore(void) const;
	unsigned int GetScore(EntityTeam team) const;
	unsigned int GetKills(EntityTeam team) const;
	unsigned int GetShotsFired(EntityTeam team) const;

	void SetMaxTime(float time);
	void SetMaxScore(unsigned int score);

private:

	GameMode     m_gameMode;                    // The game mode the game context is associated to
	bool         m_terminated;					// Tells whether the game is terminated or not
	float        m_maxTime;						// The time limit for a round of the game
	float		 m_time;						// The time spent in the current game
	unsigned int m_maxScore;					// The score required by a team for the win
	unsigned int m_score[NumberOfTeams-1];		// The current scores of the teams
	unsigned int m_kills[NumberOfTeams-1];		// The number of enemies killed by team
	unsigned int m_shotsFired[NumberOfTeams-1];	// The number of shots fired by each team
};

#endif // GAME_CONTEXT_H