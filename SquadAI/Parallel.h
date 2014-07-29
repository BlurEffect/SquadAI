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

class Parallel : public Composite
{
public:

	//--------------------------------------------------------------------------------------
	// Available policies to control the success or failure of the parallel behaviour.
	//--------------------------------------------------------------------------------------
	enum Policy
	{
		RequireOne, // It is sufficient if a single child behaviour evaluates to a certain status
		RequireAll, // All child behaviours have to evaluate to a certain status
	};

	Parallel(Policy successPolicy, Policy failurePolicy);
	virtual ~Parallel(void);

protected:
	virtual BehaviourStatus Update(void);
	virtual void OnTerminate(void);

	Policy m_successPolicy; // The policy determining when the parallel behaviour succeeds
	Policy m_failurePolicy; // The policy determining when the parallel behaviour fails
};

#endif // PARALLEL_H