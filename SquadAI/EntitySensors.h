/* 
*  Kevin Meergans, SquadAI, 2014
*  EntitySensors.h
*  This class simulates the senses of the entity and updates the entity's
*  memory according to observations made and events perceived.
*/

#ifndef ENTITY_SENSORS_H
#define ENTITY_SENSORS_H

// Includes
#include <DirectXMath.h>
#include <list>
#include "EntityMemory.h"

// Forward declarations
class FightingEntity;
struct EntitySensorInitData;

using namespace DirectX;

class EntitySensors
{
public:
	EntitySensors(void);
	~EntitySensors(void);

	bool Initialise(FightingEntity* pEntity, const EntitySensorInitData& initData);
	void CheckForThreats(void);

	// Data access functions

	float GetFieldOfView(void) const;
	float GetViewingDistance(void) const;

	void SetFieldOfView(float fieldOfView);
	void SetViewingDistance(float viewingDistance);

private:
	float		    m_fieldOfView;     // The angle determining the field of view in radians
	float		    m_viewingDistance; // The entity can look this far ahead
	FightingEntity* m_pEntity;         // The memory object associated to the entity using these sensors
};

#endif // ENTITY_SENSORS_H