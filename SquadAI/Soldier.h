/* 
*  Kevin Meergans, SquadAI, 2014
*  Soldier.h
*  Represents a fighting entity in the test environment.
*/

#ifndef SOLDIER_H
#define SOLDIER_H

// Includes
#include <DirectXMath.h>
#include "FightingEntity.h"

using namespace DirectX;

class Soldier : public FightingEntity
{
public:
	Soldier(void);
	~Soldier(void);

	bool Initialise(EntityInitData initData, const EntityMovementInitData& movementInitData, const EntitySensorInitData& sensorInitData, const EntityCombatInitData& combatInitData);

private:
};

#endif // SOLDIER_H