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
#include "TeamAIToEntityMessages.h"
#include "TeamManoeuvreFactory.h"


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

	MultiflagCTFTeamAI(void);
	~MultiflagCTFTeamAI(void);

	bool Initialise(EntityTeam team, TestEnvironment* pEnvironment, TeamAICharacteristic characteristic);
	virtual void PrepareForSimulation(void);
	//void ProcessMessage(Message* pMessage);
	void Reset(void);

	void ProcessEvent(EventType type, void* pEventData);

	virtual bool ManoeuvreStillValid(TeamManoeuvreType manoeuvre);
	virtual bool ManoeuvrePreconditionsFulfilled(TeamManoeuvreType manoeuvre);
	virtual BehaviourStatus InitiateManoeuvre(TeamManoeuvreType manoeuvre);
	virtual BehaviourStatus UpdateManoeuvre(TeamManoeuvreType manoeuvre, float deltaTime);
	virtual void TerminateManoeuvre(TeamManoeuvreType manoeuvre);

	const FlagData& GetFlagData(EntityTeam team) const;

	virtual void RegisterObjective(Objective* pObjective);

protected:

	virtual bool InitialiseManoeuvres(void);

	void ProcessMessage(Message* pMessage);
	
	// Available manoeuvres
	BehaviourStatus AllAttack(float deltaTime);
	BehaviourStatus AllDefend(float deltaTime);
	BehaviourStatus AllMove(float deltaTime);
private:

	FlagData m_flagData[NumberOfTeams-1]; // Data about the flags of the two teams

	//std::unordered_map<Direction, XMFLOAT2> m_friendlyBaseEntrances; // Contains information about the entrances to the base of team of this AI
	//std::unordered_map<Direction, XMFLOAT2> m_enemyBaseEntrances;    // Contains information about the entrances to the base of the hostile team
};

#endif // MULTIFLAG_CTF_TEAM_AI_H