/* 
*  Kevin Meergans, SquadAI, 2014
*  TestManoeuvre.h
*  Just for testing.
*/

#ifndef TEST_MANOEUVRE_H
#define TEST_MANOEUVRE_H

// Includes
#include <algorithm>
#include "TeamManoeuvre.h"

// Forward declarations
class Entity;
class Order;
class TeamAI;
enum BehaviourStatus;

class TestManoeuvre : public TeamManoeuvre
{
public:
	TestManoeuvre(unsigned int minNumberParticipants, unsigned int maxNumberParticipants, TeamAI* pTeamAI);
	~TestManoeuvre(void);

	// To be overwritten by derived manoeuvre classes

	void			Initiate(void);
	BehaviourStatus Update(float deltaTime);
	void			Terminate(void);

protected:
	
	virtual bool ProcessMessage(Message* pMessage);

private:
	TeamAI* m_pTeamAI; // The team AI able to use this manoeuvre

	unsigned int m_entitiesReachedDestinationCount; // Keeps track of how many entities have reached the destination
};

#endif // TEAM_MANOEUVRE_H