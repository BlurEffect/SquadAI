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

//--------------------------------------------------------------------------------------
// The structure of the instance data for a renderable entity.
//--------------------------------------------------------------------------------------
struct Instance
{
	Instance(const XMFLOAT4X4& world) : m_world( world ){}

	XMFLOAT4X4 m_world;
};

#endif // ENTITY_DATA_H
