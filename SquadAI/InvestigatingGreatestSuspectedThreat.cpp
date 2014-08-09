/* 
*  Kevin Meergans, SquadAI, 2014
*  InvestigatingGreatestSuspectedThreat.h
*  A condition behaviour that checks whether the entity is still investigating the
*  currently greatest suspected threat or if there is a new greatest suspected threat
*  that the entity should check.
*/
// Includes
#include "InvestigatingGreatestSuspectedThreat.h"
#include "Entity.h"

InvestigatingGreatestSuspectedThreat::InvestigatingGreatestSuspectedThreat(Entity* pEntity, const char* name) : Behaviour(pEntity, name)
{
}

InvestigatingGreatestSuspectedThreat::~InvestigatingGreatestSuspectedThreat(void)
{
}

//--------------------------------------------------------------------------------------
// Updates the behaviour.
// Param1: The time in seconds passed since the last frame.
// Returns the state of the behaviour.
//--------------------------------------------------------------------------------------
BehaviourStatus InvestigatingGreatestSuspectedThreat::Update(float deltaTime)
{
	if(GetEntity()->IsInvestigatingGreatestSuspectedThreat())
	{
		return StatusSuccess;
	}else
	{
		return StatusFailure;
	}
}
