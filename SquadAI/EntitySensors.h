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

// Forward declarations
class Entity;
class TestEnvironment;

using namespace DirectX;

class EntitySensors
{
public:
	EntitySensors(void);
	~EntitySensors(void);

	bool Initialise(Entity* pEntity, TestEnvironment* pTestEnvironment);
	
	void CheckForThreats(const XMFLOAT2& viewDirection, float viewingRange, float fieldOfView);

private:
	Entity*          m_pEntity;      // The entity object associated to this sensors component
	TestEnvironment* m_pEnvironment; // The test environment, to which the entity belongs
};

#endif // ENTITY_SENSORS_H