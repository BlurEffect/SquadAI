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

CoverPosition::CoverPosition(unsigned long id, EntityType type, const XMFLOAT2& position, float rotation, TestEnvironment* pEnvironment) : Entity(id, type, position, rotation, pEnvironment)
{
}

CoverPosition::~CoverPosition(void)
{
}