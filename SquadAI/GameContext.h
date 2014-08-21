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
#include "Communicator.h"
#include "GameContextToTeamAIMessages.h"

// Forward declarations
class TeamAI;

//--------------------------------------------------------------------------------------
// Identifies the specific type of game context that is the game mode it is associated to.
//--------------------------------------------------------------------------------------
enum GameMode
{
	MultiflagCTF  // Identifier for Capture-The-Flag matches, in which each team has a flag to protect
};

class GameContext : public Communicator
{
public:
	GameContext(GameMode mode, float maxTime, float notifyTimeInterval, unsigned int winScore);
	virtual ~GameContext(void) = 0;

	virtual void Update(float deltaTime);
	virtual void Reset(void);

	void AddScore(EntityTeam team, unsigned int score);
	void AddKill(EntityTeam team, EntityTeam victimTeam, unsigned long entityId);
	void AddShotFired(EntityTeam team, unsigned int shotsFired);

	void RegisterTeamAI(TeamAI* pTeamAI);
	void BroadcastMessage(MessageType type, void* pMessageData);

	// Data access functions

	GameMode     GetGameMode(void) const;
	bool		 IsTerminated(void) const;
	float		 GetMaxTime(void) const;
	float        GetTime(void) const;
	float        GetTimeLeft(void) const;
	float        GetNotifyTimeInterval(void) const;
	unsigned int GetMaxScore(void) const;
	unsigned int GetScore(EntityTeam team) const;
	unsigned int GetKills(EntityTeam team) const;
	unsigned int GetShotsFired(EntityTeam team) const;

	void SetMaxTime(float time);
	void SetNotifyTimeInterval(float timeInterval);
	void SetMaxScore(unsigned int score);

	virtual void ProcessEvent(EventType type, void* pEventData);
protected:
	
	virtual bool ProcessMessage(Message* pMessage);

private:

	GameMode     m_gameMode;                    // The game mode the game context is associated to
	bool         m_terminated;					// Tells whether the game is terminated or not
	float        m_maxTime;						// The time limit for a round of the game
	float		 m_time;						// The time spent in the current game
	float        m_notifyTimeInterval;			// Determines how often the teams are notified of the time left in the current round
	float        m_notifyTimer;                 // Keeps track of the time passed since the team AIs were notified the last time
	unsigned int m_maxScore;					// The score required by a team for the win
	unsigned int m_score[NumberOfTeams-1];		// The current scores of the teams
	unsigned int m_kills[NumberOfTeams-1];		// The number of enemies killed by team
	unsigned int m_shotsFired[NumberOfTeams-1];	// The number of shots fired by each team
	TeamAI*      m_teamAIs[NumberOfTeams-1];    // The team AIs controlling the different teams
};

#endif // GAME_CONTEXT_H