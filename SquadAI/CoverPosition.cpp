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

CoverPosition::~CoverPosition(void)
{
}

//--------------------------------------------------------------------------------------
// Initialises the cover position.
// Param1: The base initialisation data for the entity.
// Returns true if the cover position was initialised successfully, false otherwise.
//--------------------------------------------------------------------------------------
bool CoverPosition::Initialise(EntityInitData initData)
{
	return Entity::Initialise(initData);
}