/* 
*  Kevin Meergans, SquadAI, 2014
*  UpdateThreats.h
*  An action behaviour that updates the threats to the entity.
*/

#ifndef UPDATE_THREATS_H
#define UPDATE_THREATS_H

// Includes
#include "UniversalIndividualBehaviour.h"

class UpdateThreats : public UniversalIndividualBehaviour
{
public:
	UpdateThreats(const char* name, Entity* pEntity);
	~UpdateThreats(void);

private:
	BehaviourStatus Update(float deltaTime);
};

#endif // UPDATE_THREATS_H