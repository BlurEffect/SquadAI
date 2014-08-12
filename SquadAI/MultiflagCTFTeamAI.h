/* 
*  Kevin Meergans, SquadAI, 2014
*  MultiflagCTFTeamAI.h
*  Team AI that controls its entities to play Multiflag Capture-the-Flag matches.
*/

#ifndef MULTIFLAG_CTF_TEAM_AI_H
#define MULTIFLAG_CTF_TEAM_AI_H

// Includes
#include <DirectXMath.h>
#include "TeamAI.h"
#include "ObjectTypes.h"
#include "MultiflagCTFGameContext.h" // needs to know flag state -> move flag state somewhere else (game mode spceifc data structs?)

// Forward declarations
class TestEnvironment;

using namespace DirectX;

//--------------------------------------------------------------------------------------
// Bundles the team AI's knowlegde of a flag in the flag.
//--------------------------------------------------------------------------------------
struct FlagData
{
	FlagData(void) : m_state(InBase),
				     m_position(0.0f, 0.0f),
					 m_basePosition(0.0f, 0.0f),
					 m_carrierId(0)
	{}

	FlagState	  m_state;			// The current state of the flag
	XMFLOAT2	  m_position;		// The current position of the flag
	XMFLOAT2	  m_basePosition;	// The original position of the flag in the homebase of the respective team
	unsigned long m_carrierId;		// The id of the entity currently carrying the flag, 0 if not being carried
};


class MultiflagCTFTeamAI : public TeamAI
{
public:

	MultiflagCTFTeamAI(EntityTeam team, TestEnvironment* pEnvironment);
	~MultiflagCTFTeamAI(void);

	void ProcessMessage(Message* pMessage);

	void Reset(void);

private:

	FlagData m_flagData[NumberOfTeams-1]; // Data about the flags of the two teams
};

#endif // MULTIFLAG_CTF_TEAM_AI_H