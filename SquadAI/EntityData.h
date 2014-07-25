/* 
*  Kevin Meergans, SquadAI, 2014
*  EntityData.h
*  Contains data structures defining an entity.
*/

#ifndef ENTITY_DATA_H
#define ENTITY_DATA_H

// Includes
#include <DirectXMath.h>

using namespace DirectX;



/*

//--------------------------------------------------------------------------------------
// Identifies an entity of a certain type within the test application.
//--------------------------------------------------------------------------------------
enum EntityType
{
	ASoldier,
	BSoldier,
	CoverSpot,
	ADeadSoldier,
	BDeadSoldier,
	ProjectileType,
	NumberOfEntityTypes
};

*/

//--------------------------------------------------------------------------------------
// Identifies a team that soldiers can belong to.
//--------------------------------------------------------------------------------------
enum EntityTeam
{
	TeamRed, // The red team  
	TeamBlue, // The blue team
	NumberOfTeams
};

//--------------------------------------------------------------------------------------
// Identifies the different types of objects that can be present in the test environment, 
// some of which are placable while others are created at runtime.
//--------------------------------------------------------------------------------------
enum ObjectType
{
	// Placable
	RedSoldierStartPositionType,	// Start position for a soldier of team red
	BlueSoldierStartPositionType,	// Start position for a soldier of team blue
	ObstacleType,					// An obstacle that blocks the way of soldiers
	RedFlagStartPositionType,		// Start position for the red team flag
	BlueFlagStartPositionType,		// Start position for the blue team flag
	RedBaseAreaType,				// A grid field that belongs to the red base
	BlueBaseAreaType,				// A grid field that belongs to the blue base
	// Non placable
	RedSoldierType,					// A soldier of team red
	BlueSoldierType,				// A soldier of team blue
	DeadRedSoldierType,				// A dead soldier of team red
	DeadBlueSoldierType,			// A dead soldier of team blue
	RedFlagType,					// The flag of team red
	BlueFlagType,					// The flag of team blue
	ProjectileType,					// A projectile shot by one of the soldiers
	NumberOfObjectTypes
};

//--------------------------------------------------------------------------------------
// The structure of the instance data for a renderable entity.
//--------------------------------------------------------------------------------------
struct Instance
{
	Instance(const XMFLOAT4X4& world) : m_world( world ){}

	XMFLOAT4X4 m_world;
};

#endif // ENTITY_DATA_H
