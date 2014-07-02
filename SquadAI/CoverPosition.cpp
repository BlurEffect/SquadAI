/* 
*  Kevin Meergans, SquadAI, 2014
*  CoverPosition.cpp
*  Represents a cover spot in the test environment.
*/

// Includes
#include "CoverPosition.h"

CoverPosition::CoverPosition(void) : Entity()
{
}

CoverPosition::CoverPosition(EntityType type, const XMFLOAT3& position, float rotation) : Entity(type, position, rotation)
{
}

CoverPosition::~CoverPosition(void)
{
}