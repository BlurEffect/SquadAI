/* 
*  Kevin Meergans, SquadAI, 2014
*  TeamAI.h
*  Abstract base class for all team AIs.
*/

#ifndef TEAM_AI_H
#define TEAM_AI_H

// Includes
#include <unordered_map>
#include <set>
#include <vector>
#include <list>
#include <algorithm>
#include <DirectXMath.h>

#include "ObjectTypes.h"
#include "Message.h"
#include "EntityToTeamAIMessages.h"
#include "GameContextToTeamAIMessages.h"
#include "Communicator.h"
#include "Behaviour.h"
#include "Order.h"
#include "TeamManoeuvre.h"

using namespace DirectX;

// Forward declarations
class Entity;
class TestEnvironment;

//--------------------------------------------------------------------------------------
// Available characteristics for the team AI. A characteristic influences the way a 
// team AI plays, that is the tactics and manoeuvres it chooses during a game.
//--------------------------------------------------------------------------------------
enum TeamAICharacteristic
{
	CharNone,
	CharAggressive,
	CharDefensive
};

//--------------------------------------------------------------------------------------
// The team AI uses these records to bundle the available information about certain
// enemy entities.
//--------------------------------------------------------------------------------------
struct EnemyRecord
{
	EnemyRecord(const XMFLOAT2& lastKnownPosition) : m_lastKnownPosition(lastKnownPosition)	
	{
	}

	XMFLOAT2 m_lastKnownPosition; // The position, where the enemy was last seen
};


class TeamAI : public Communicator
{
public:

	TeamAI(void);
	virtual ~TeamAI(void) = 0;

	virtual bool Initialise(EntityTeam team, TestEnvironment* pEnvironment, TeamAICharacteristic characteristic);

	void AddTeamMember(Entity* pEntity);
	void RemoveTeamMember(unsigned long id);

	virtual void			PrepareForSimulation(void);
	virtual void			Update(float deltaTime);
	virtual void			Reset(void);
	
	virtual void			ProcessEvent(EventType type, void* pEventData);

	virtual bool			ManoeuvrePreconditionsFulfilled(TeamManoeuvreType manoeuvre);
	virtual bool			ManoeuvreStillValid(TeamManoeuvreType manoeuvre);
	virtual BehaviourStatus InitiateManoeuvre(TeamManoeuvreType manoeuvre);
	virtual BehaviourStatus UpdateManoeuvre(TeamManoeuvreType manoeuvre, float deltaTime);
	virtual void			TerminateManoeuvre(TeamManoeuvreType manoeuvre);
	virtual void			ActivateManoeuvre(TeamManoeuvreType manoeuvre);
	virtual void			RegisterObjective(Objective* pObjective) = 0;
	void					ReleaseEntityFromManoeuvre(unsigned long entityId);


	// Data access functions

	EntityTeam															GetTeam(void) const;
	TeamAICharacteristic												GetCharacteristic(void) const;
	TestEnvironment*													GetTestEnvironment(void);
	std::vector<Entity*>&												GetTeamMembers(void);
	std::unordered_map<unsigned long, EnemyRecord>&						GetEnemyRecords(void);
	const std::unordered_map<unsigned long, std::set<unsigned long>>&	GetSpottedEnemies(void) const;
	float																GetScore(EntityTeam team) const;
	float																GetTimeLeft(void) const;
	
	void SetTeam(EntityTeam team);
	void SetCharacteristic(TeamAICharacteristic characteristic);
	void SetTestEnvironment(TestEnvironment* pEnvironment);

protected:

	virtual bool InitialiseManoeuvres(void);
	virtual void ProcessMessage(Message* pMessage);
	void		 ForwardMessageToActiveManoeuvers(Message* pMessage);

	Behaviour*													m_pBehaviour;									 // The behaviour tree controlling the decisions of the team AI
	std::unordered_map<TeamManoeuvreType, TeamManoeuvre*>		m_manoeuvres;								     // The available team manoeuvres for this team AI
	std::unordered_map<unsigned long, TeamManoeuvre*>			m_entityManoeuvreMap;						     // Keeps track of which team members are currently engaged in which manoeuvres
	TeamManoeuvre*												m_activeManoeuvres[NumberOfManoeuvreCategories]; // Keeps track of the active manoeuvre for each category
	std::unordered_map<unsigned long, std::set<unsigned long>>	m_spottedEnemies;								 // Keeps track of the enemies in view for each team member

private:
	EntityTeam										m_team;					   // The team that the AI is controlling
	TeamAICharacteristic							m_characteristic;		   // The characteristic of the team AI
	std::vector<Entity*>							m_teamMembers;			   // The entities being controlled by this team AI
	std::unordered_map<unsigned long, EnemyRecord>	m_enemyRecords;			   // The team AI creates and obtains a record for every enemy spotted in the environment
	TestEnvironment*								m_pTestEnvironment;        // The environment, in which the current match plays
	float											m_scores[NumberOfTeams-1]; // The current score for each team as a percentual value in relation to the score required for victory
	float											m_timeLeft;                // The time left as a percentual value in relation to the maximal time a round can last

};

#endif // TEAM_AI_H