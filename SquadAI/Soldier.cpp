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

Soldier::Soldier(unsigned long id, EntityType type, const XMFLOAT3& position, float rotation) : Entity(id, type, position, rotation)
{
}

Soldier::~Soldier(void)
{
}