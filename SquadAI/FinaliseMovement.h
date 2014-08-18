/* 
*  Kevin Meergans, SquadAI, 2014
*  FinaliseMovement.h
*  An action behaviour that can be used to set the behaviour of an entity when it reaches
*  its movement target. Can also be used to reset movement data etc.
*/

#ifndef FINALISE_MOVEMENT_H
#define FINALISE_MOVEMENT_H

// Includes
#include "UniversalIndividualBehaviour.h"

class FinaliseMovement : public UniversalIndividualBehaviour
{
public:
	FinaliseMovement(const char* name, Entity* pEntity);
	~FinaliseMovement(void);

private:
	BehaviourStatus Update(float deltaTime);
};

#endif // FINALISE_MOVEMENT_H