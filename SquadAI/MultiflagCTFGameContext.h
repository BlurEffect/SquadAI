/* 
*  Kevin Meergans, SquadAI, 2014
*  MultiflagCTFGameContext.h
*  This game context contains the state of a running multiflag Capture-the-flag game
*  and is in charge of controlling the game mode specific flag objects.
*/

#ifndef MULTIFLAG_CTF_GAME_CONTEXT_H
#define MULTIFLAG_CTF_GAME_CONTEXT_H

// Includes
#include <DirectXMath.h>
#include "GameContext.h"
#include "Objective.h"
#include "RenderContext.h"

// Forward declarations
class Entity;

using namespace DirectX;

//--------------------------------------------------------------------------------------
// Identifies the current state of a flag.
//--------------------------------------------------------------------------------------
enum FlagState
{
	InBase, // The flag is placed at its original position within the base of the team it belongs to
	Stolen, // The flag was stolen and is being carried by a player of the opposing team
	Dropped // The flag was stolen but the carrier was killed and the flag was dropped
};

class MultiflagCTFGameContext : public GameContext
{
public:
	MultiflagCTFGameContext(float maxTime, float notifyTimeInterval, unsigned int winScore, float flagResetTimeLimit);
	~MultiflagCTFGameContext(void);
	
	void Update(float deltaTime);
	void Reset(void);

	void FlagPickedUp(EntityTeam flagOwner, Entity* pCarrier);
	void FlagDropped(EntityTeam flagOwner);
	void FlagReturned(EntityTeam flagOwner);
	void FlagCaptured(EntityTeam flagOwner);

	void ProcessEvent(EventType type, void* pEventData);

	// Data access functions

	float           GetFlagResetTimeLimit(void) const;
	float           GetFlagResetTimeLeft(EntityTeam team) const;
	FlagState		GetFlagState(EntityTeam team) const;
	const XMFLOAT2& GetFlagBasePosition(EntityTeam team) const;
	const XMFLOAT2& GetFlagPosition(EntityTeam team) const;
	const Entity*	GetFlagCarrier(EntityTeam team) const;

	void SetFlagResetTimeLimit(float timeLimit);
	void SetFlagBasePosition(EntityTeam team, const XMFLOAT2& basePosition);
	void SetFlag(EntityTeam flagOwner, Objective* pCollidableObject);

protected:
	void ProcessMessage(Message* pMessage);

private:

	float	   m_flagResetTimeLimit;				 // The time it takes a flag to be returned to its base position once dropped
	float      m_flagResetTimers[NumberOfTeams-1];   // Keeps track of how long it will take for each flag to be reset
	FlagState  m_flagStates[NumberOfTeams-1];	     // The current state of the flags of all teams
	XMFLOAT2   m_flagBasePositions[NumberOfTeams-1]; // The base positions of the flags they will be returned to when dropped for too long or when returned/captured
	XMFLOAT2   m_flagPositions[NumberOfTeams-1];     // The current positions of the flags of all teams
	Entity*    m_flagCarriers[NumberOfTeams-1];      // Pointers to the current flag carriers, nullptr if the flag is not being carried
	Objective* m_flags[NumberOfTeams-1];			 // The flags of the teams
};

#endif // MULTIFLAG_CTF_GAME_CONTEXT_H