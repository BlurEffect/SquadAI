/* 
*  Kevin Meergans, SquadAI, 2014
*  GreatestKnownThreatSet.cpp
*  A condition behaviour that checks whether one of the entity's known
*  threats was selected and set as greatest known threat.
*/

// Includes
#include "GreatestKnownThreatSet.h"
#include "Entity.h"

GreatestKnownThreatSet::GreatestKnownThreatSet(Entity* pEntity, const char* name) : Behaviour(pEntity, name)
{
}

GreatestKnownThreatSet::~GreatestKnownThreatSet(void)
{
}

//--------------------------------------------------------------------------------------
// Updates the behaviour.
// Param1: The time in seconds passed since the last frame.
// Returns the state of the behaviour.
//--------------------------------------------------------------------------------------
BehaviourStatus GreatestKnownThreatSet::Update(float deltaTime)
{
	if(GetEntity()->GetGreatestKnownThreat())
	{
		return StatusSuccess;
	}else
	{
		return StatusFailure;
	}
}
