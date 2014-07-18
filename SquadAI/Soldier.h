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
	Soldier(unsigned long id, EntityType type, const XMFLOAT2& position, float rotation, float scale, float radius, TestEnvironment* pEnvironment, const EntityMovementData& movementData, const EntitySensorData& sensorData, const EntityCombatData& combatData);
	~Soldier(void);
private:
};

#endif // SOLDIER_H