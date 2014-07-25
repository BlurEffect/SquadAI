/* 
*  Kevin Meergans, SquadAI, 2014
*  TestEnvironmentData.h
*  Contains data structures used by the test environment class
*/

#ifndef TEST_ENVIRONMENT_DATA_H
#define TEST_ENVIRONMENT_DATA_H

// Includes
#include "Entity.h"

//--------------------------------------------------------------------------------------
// Holds a value for every direction in the test environment.
//--------------------------------------------------------------------------------------
enum Direction
{
	North,
	NorthEast,
	East,
	SouthEast,
	South,
	SouthWest,
	West,
	NorthWest,
	NumberOfDirections
};

//--------------------------------------------------------------------------------------
// Used to distinguish between different groups of entities.
//--------------------------------------------------------------------------------------
enum EntityGroup
{
	GroupTeamRed,						// The group of all living soldiers of team A
	GroupTeamBlue,						// The group of all living soldiers of team B
	GroupAllSoldiers,				// The group of all living soldiers
	GroupObstacles,					// The group of all obstacles/cover positions
	GroupTeamRedAndObstacles,			// The group of all living soldiers of team A and all obstacles
	GroupTeamBlueAndObstacles,			// The group of all living soldiers of team B and all obstacles
	GroupAllSoldiersAndObstacles	// The group of all living soldiers and all obstacles
};

//--------------------------------------------------------------------------------------
// Holds the information associated to a grid field in edit mode.
//--------------------------------------------------------------------------------------
struct GridField
{
	GridField(void) : m_pEntity(nullptr),
					  m_isEmpty(true),
					  m_id(0),
					  m_type(ObjectType(0)),
					  m_rotation(0.0f)
	{}

	Entity*       m_pEntity;  // A pointer to the entity originally placed on this field
	bool          m_isEmpty;  // Tells whether the grid field was originally occupied by an entity.
	unsigned long m_id;       // The ID of the entity placed on this field.
	ObjectType    m_type;     // The type of the entity originally placed on this field.
	float         m_rotation; // The original rotation of the entity.
};

/*
//--------------------------------------------------------------------------------------
// Holds the information associated to a basic objects present in the test environment.
//--------------------------------------------------------------------------------------
struct BasicObject
{
	BasicObject(unsigned long gridId, ObjectType type, const XMFLOAT2& position, float rotation, float uniformScale)
		: m_gridId(gridId),
		  m_type(type),
		  m_position(position),
		  m_rotation(rotation),
		  m_uniformScale(uniformScale)
	{}

	unsigned long		  m_gridId;			// An id that identifies the grid field that this object is placed on (multiple objects per field are allowed)
	ObjectType			  m_type;			// The type of this object
	XMFLOAT2              m_position;		// The world position of this object
	float                 m_rotation;		// The rotation of this object along the world z-axis
	float                 m_uniformScale;	// The uniform scale to apply to this object
};
*/
#endif // TEST_ENVIRONMENT_DATA_H