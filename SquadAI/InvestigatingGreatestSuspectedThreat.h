/* 
*  Kevin Meergans, SquadAI, 2014
*  InvestigatingGreatestSuspectedThreat.h
*  A condition behaviour that checks whether the entity is still investigating the
*  currently greatest suspected threat or if there is a new greatest suspected threat
*  that the entity should check.
*/

#ifndef INVESTIGATING_GREATEST_SUSPECTED_THREAT_H
#define INVESTIGATING_GREATEST_SUSPECTED_THREAT_H

// Includes
#include "UniversalIndividualBehaviour.h"

class InvestigatingGreatestSuspectedThreat : public UniversalIndividualBehaviour
{
public:
	InvestigatingGreatestSuspectedThreat(const char* name, Entity* pEntity);
	~InvestigatingGreatestSuspectedThreat(void);

private:
	BehaviourStatus Update(float deltaTime);
};

#endif // INVESTIGATING_GREATEST_SUSPECTED_THREAT_H