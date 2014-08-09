/* 
*  Kevin Meergans, SquadAI, 2014
*  GameState.h
*  This class bundles the current state of a Multiflag CTF game.
*/

#ifndef GAME_STATE_H
#define GAME_STATE_H

// Includes
#include "ObjectTypes.h"
#include "Flag.h"


class GameState
{
public:
	GameState(void);
	~GameState(void);

	void Initialise(float maxTime, unsigned int winScore);
	void AddScore(EntityTeam team, unsigned int score);
	void AddKills(EntityTeam team, unsigned int kills);
	void AddShotFired(EntityTeam team, unsigned int shotsFired);
	void UpdateTimer(float deltaTime);
	void Reset(void);

	// Data access functions

	bool		 IsTerminated(void) const;
	float		 GetMaxTime(void) const;
	float        GetTime(void) const;
	float        GetTimeLeft(void) const;
	unsigned int GetMaxScore(void) const;
	unsigned int GetScore(EntityTeam team) const;
	unsigned int GetKills(EntityTeam team) const;
	unsigned int GetShotsFired(EntityTeam team) const;

	void SetMaxTime(float time);
	void SetMaxScore(float score);

private:

	bool         m_terminated;					// Tells whether the game is terminated or not
	float        m_maxTime;						// The time limit for a round of the game
	float		 m_time;						// The time spent in the current game
	unsigned int m_maxScore;					// The score required by a team for the win
	unsigned int m_score[NumberOfTeams-1];		// The current scores of the teams
	unsigned int m_kills[NumberOfTeams-1];		// The number of enemies killed by team
	unsigned int m_shotsFired[NumberOfTeams-1];	// The number of shots fired by each team

};

#endif // GAME_STATE_H