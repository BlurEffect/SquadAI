/* 
*  Kevin Meergans, SquadAI, 2014
*  GreatestSuspectedThreatSet.cpp
*  A condition behaviour that checks whether one of the entity's suspected
*  threats was selected and set as greatest suspected threat.
*/

// Includes
#include "GreatestSuspectedThreatSet.h"
#include "Entity.h"

GreatestSuspectedThreatSet::GreatestSuspectedThreatSet(const char* name, Entity* pEntity) : UniversalIndividualBehaviour(name, pEntity)
{
}

GreatestSuspectedThreatSet::~GreatestSuspectedThreatSet(void)
{
}

//--------------------------------------------------------------------------------------
// Updates the behaviour.
// Param1: The time in seconds passed since the last frame.
// Returns the state of the behaviour.
//--------------------------------------------------------------------------------------
BehaviourStatus GreatestSuspectedThreatSet::Update(float deltaTime)
{
	if(GetEntity()->GetGreatestSuspectedThreat())
	{
		return StatusSuccess;
	}else
	{
		return StatusFailure;
	}
}
