/* 
*  Kevin Meergans, SquadAI, 2014
*  ObservationTargetSet.cpp
*  A condition behaviour that checks whether a observation target was set
*  for the entity or not.
*/

// Includes
#include "ObservationTargetSet.h"
#include "Entity.h"

ObservationTargetSet::ObservationTargetSet(const char* name, Entity* pEntity) : UniversalIndividualBehaviour(name, pEntity)
{
}

ObservationTargetSet::~ObservationTargetSet(void)
{
}

//--------------------------------------------------------------------------------------
// Updates the behaviour.
// Param1: The time in seconds passed since the last frame.
// Returns the state of the behaviour.
//--------------------------------------------------------------------------------------
BehaviourStatus ObservationTargetSet::Update(float deltaTime)
{
	if(GetEntity()->IsObservationTargetSet())
	{
		return StatusSuccess;
	}else
	{
		return StatusFailure;
	}
}
