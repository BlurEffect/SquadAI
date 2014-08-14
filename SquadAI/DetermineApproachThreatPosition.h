/* 
*  Kevin Meergans, SquadAI, 2014
*  DetermineApproachThreatPosition.h
*  An action behaviour that determines a target position for the entity to move
*  towards in order to investigate a threat.
*/

#ifndef DETERMINE_APPROACH_THREAT_POSITION_H
#define DETERMINE_APPROACH_THREAT_POSITION_H

// Includes
#include "UniversalIndividualBehaviour.h"

class DetermineApproachThreatPosition : public UniversalIndividualBehaviour
{
public:
	DetermineApproachThreatPosition(const char* name, Entity* pEntity);
	~DetermineApproachThreatPosition(void);

private:
	BehaviourStatus Update(float deltaTime);
};

#endif // DETERMINE_APPROACH_THREAT_POSITION_H