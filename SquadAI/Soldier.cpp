/* 
*  Kevin Meergans, SquadAI, 2014
*  Soldier.cpp
*  Represents a fighting entity in the test environment.
*/

// Includes
#include "Soldier.h"

Soldier::Soldier(void) : Entity()
{
}

Soldier::Soldier(EntityType type, const XMFLOAT3& position, float rotation) : Entity(type, position, rotation)
{
}

Soldier::~Soldier(void)
{
}