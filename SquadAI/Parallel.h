/* 
*  Kevin Meergans, SquadAI, 2014
*  Parallel.h
*  Composite behaviour that executes all of its child behaviours in parallel. Success and failure
*  of the parallel composite depend on policies that have to be set by the user.
*  Based on Alex Champandard's presentation "Understanding the Second-Generation of Behavior Trees"
*  and the associated "Behavior Tree Starter Kit" (BTSK).
*/

#ifndef PARALLEL_H
#define PARALLEL_H

// Includes
#include "Composite.h"


//--------------------------------------------------------------------------------------
// Available policies to control the success or failure of the parallel behaviour.
//--------------------------------------------------------------------------------------
enum ParallelPolicy
{
	RequireOne, // It is sufficient if a single child behaviour evaluates to a certain status
	RequireAll, // All child behaviours have to evaluate to a certain status
};

//--------------------------------------------------------------------------------------
// Contains all additional data required for the creation of a parallel behaviour.
//--------------------------------------------------------------------------------------
struct ParallelInitData
{
	ParallelInitData(ParallelPolicy successPolicy, ParallelPolicy failurePolicy) : m_successPolicy(successPolicy),
																				   m_failurePolicy(failurePolicy)
	{}

	ParallelPolicy m_successPolicy;
	ParallelPolicy m_failurePolicy;
};

class Parallel : public Composite
{
public:

	Parallel(Entity* pEntity, const char* name, ParallelPolicy successPolicy, ParallelPolicy failurePolicy);
	virtual ~Parallel(void);

protected:
	virtual BehaviourStatus Update(float deltaTime);
	virtual void OnTerminate(BehaviourStatus status);

	ParallelPolicy m_successPolicy; // The policy determining when the parallel behaviour succeeds
	ParallelPolicy m_failurePolicy; // The policy determining when the parallel behaviour fails
};

#endif // PARALLEL_H