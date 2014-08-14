/* 
*  Kevin Meergans, SquadAI, 2014
*  TeamAI.h
*  Abstract base class for all team AIs.
*/

#ifndef TEAM_AI_H
#define TEAM_AI_H

// Includes
#include <map>
#include <set>
#include <vector>
#include <algorithm>
#include <DirectXMath.h>

#include "ObjectTypes.h"
#include "Message.h"
#include "EntityToTeamAIMessages.h"
#include "GameContextToTeamAIMessages.h"
#include "Communicator.h"

using namespace DirectX;

// Forward declarations
class Entity;
class TestEnvironment;

//--------------------------------------------------------------------------------------
// The team AI uses these records to bundle the available information about certain
// enemy entities.
//--------------------------------------------------------------------------------------
struct EnemyRecord
{
	EnemyRecord(const XMFLOAT2& lastKnownPosition, unsigned long spotterId) : m_lastKnownPosition(lastKnownPosition)	
	{
		m_spotterIds.insert(spotterId);
	}

	XMFLOAT2			    m_lastKnownPosition; // The position, where the enemy was last seen
	std::set<unsigned long> m_spotterIds;		 // The IDs of the friendly entities currently seeing this enemy
};


class TeamAI : public Communicator
{
public:

	TeamAI(EntityTeam team, TestEnvironment* pEnvironment);
	virtual ~TeamAI(void) = 0;

	//virtual void ProcessMessage(Message* pMessage);
	void AddTeamMember(Entity* pEntity);
	void RemoveTeamMember(unsigned long id);

	virtual void Reset(void);

	// Data access functions

	EntityTeam			   GetTeam(void) const;
	const TestEnvironment* GetTestEnvironment(void) const;

	void SetTeam(EntityTeam team);
	void SetTestEnvironment(TestEnvironment* pEnvironment);

	virtual void ProcessEvent(EventType type, void* pEventData);
protected:
	virtual void ProcessMessage(Message* pMessage);
	

private:
	EntityTeam						     m_team;					// The team that the AI is controlling
	std::vector<Entity*>                 m_teamMembers;				// The entities being controlled by this team AI
	std::map<unsigned long, EnemyRecord> m_enemyRecords;			// The team AI creates and obtains a record for every enemy spotted in the environment
	TestEnvironment*                     m_pTestEnvironment;        // The environment, in which the current match plays
	float								 m_scores[NumberOfTeams-1]; // The current score for each team as a percentual value in relation to the score required for victory
	float                                m_timeLeft;                // The time left as a percentual value in relation to the maximal time a round can last
};

#endif // TEAM_AI_H