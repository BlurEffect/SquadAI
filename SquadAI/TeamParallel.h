/* 
*  Kevin Meergans, SquadAI, 2014
*  TeamParallel.h
*  Composite behaviour that executes all of its child team behaviours in parallel. Success and failure
*  of the parallel composite depend on policies that have to be set by the user.
*/

#ifndef TEAM_PARALLEL_H
#define TEAM_PARALLEL_H

// Includes
#include "TeamComposite.h"

// Forward declarations
class TeamAI;

//--------------------------------------------------------------------------------------
// Available policies to control the success or failure of the team parallel behaviour.
//--------------------------------------------------------------------------------------
enum TeamParallelPolicy
{
	TeamRequireOne, // It is sufficient if a single child behaviour evaluates to a certain status
	TeamRequireAll, // All child behaviours have to evaluate to a certain status
};

//--------------------------------------------------------------------------------------
// Contains all additional data required for the creation of a team parallel behaviour.
//--------------------------------------------------------------------------------------
struct TeamParallelInitData
{
	TeamParallelInitData(TeamParallelPolicy successPolicy, TeamParallelPolicy failurePolicy) : m_successPolicy(successPolicy),
																				               m_failurePolicy(failurePolicy)
	{}

	TeamParallelPolicy m_successPolicy;
	TeamParallelPolicy m_failurePolicy;
};

class TeamParallel : public TeamComposite
{
public:

	TeamParallel(const char* name, TeamAI* pTeamAI, TeamParallelPolicy successPolicy, TeamParallelPolicy failurePolicy);
	virtual ~TeamParallel(void);

	virtual void CalculateCharacteristicValues(void);

protected:
	virtual BehaviourStatus Update(float deltaTime);

	TeamParallelPolicy m_successPolicy; // The policy determining when the parallel behaviour succeeds
	TeamParallelPolicy m_failurePolicy; // The policy determining when the parallel behaviour fails
};

#endif // TEAM_PARALLEL_H