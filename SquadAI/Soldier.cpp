/* 
*  Kevin Meergans, SquadAI, 2014
*  Soldier.cpp
*  Represents a fighting entity in the test environment.
*/

// Includes
#include "Soldier.h"

Soldier::Soldier(void) : MovingEntity()
{
}

Soldier::Soldier(unsigned long id, EntityType type, const XMFLOAT2& position, float rotation, TestEnvironment* pEnvironment, float maxVelocity, float maxForce) : MovingEntity(id, type, position, rotation, pEnvironment, maxVelocity, maxForce)
{
}

Soldier::~Soldier(void)
{
}