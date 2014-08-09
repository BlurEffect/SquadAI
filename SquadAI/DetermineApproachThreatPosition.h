/* 
*  Kevin Meergans, SquadAI, 2014
*  DetermineApproachThreatPosition.h
*  An action behaviour that determines a target position for the entity to move
*  towards in order to investigate a threat.
*/

#ifndef DETERMINE_APPROACH_THREAT_POSITION_H
#define DETERMINE_APPROACH_THREAT_POSITION_H

// Includes
#include "Behaviour.h"

class DetermineApproachThreatPosition : public Behaviour
{
public:
	DetermineApproachThreatPosition(Entity* pEntity, const char* name);
	~DetermineApproachThreatPosition(void);

private:
	BehaviourStatus Update(float deltaTime);
};

#endif // DETERMINE_APPROACH_THREAT_POSITION_H