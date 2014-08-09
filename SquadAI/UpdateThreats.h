/* 
*  Kevin Meergans, SquadAI, 2014
*  UpdateThreats.h
*  An action behaviour that updates the threats to the entity.
*/

#ifndef UPDATE_THREATS_H
#define UPDATE_THREATS_H

// Includes
#include "Behaviour.h"

class UpdateThreats : public Behaviour
{
public:
	UpdateThreats(Entity* pEntity, const char* name);
	~UpdateThreats(void);

private:
	BehaviourStatus Update(float deltaTime);
};

#endif // UPDATE_THREATS_H