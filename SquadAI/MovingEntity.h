/* 
*  Kevin Meergans, SquadAI, 2014
*  MovingEntity.h
*  Base class for all moving entities in the test environment.
*/

#ifndef MOVING_ENTITY_H
#define MOVING_ENTITY_H

// Includes
#include <DirectXMath.h>
#include "Entity.h"
#include "MovementManager.h"

using namespace DirectX;

//--------------------------------------------------------------------------------------
// Bundles the data defining the properties for entity movement.
//--------------------------------------------------------------------------------------
struct EntityMovementData
{
	EntityMovementData(void) : m_velocity(0.0f, 0.0f),
							   m_maxVelocity(0.0f),
							   m_maxForce(0.0f),
							   m_maxSeeAhead(0.0f)
	{}

	EntityMovementData(XMFLOAT2 velocity, float maxVelocity, float maxForce, float maxSeeAhead) : m_velocity(velocity),
																							      m_maxVelocity(maxVelocity),
																			                      m_maxForce(maxForce),
																			                      m_maxSeeAhead(maxSeeAhead)
	{}

	XMFLOAT2 m_velocity;    // The current velocity of the entity
	float    m_maxVelocity; // The maximal velocity allowed (limits the length of the velocity vector)
	float    m_maxForce;    // The maximal force that can impact this entity's movement
	float    m_maxSeeAhead; // The distance that the entity can see ahead and check for collisions with other entities
};


class MovingEntity : public Entity
{
public:
	MovingEntity(void);
	MovingEntity(unsigned long id, EntityType type, const XMFLOAT2& position, float rotation, float scale, float radius, TestEnvironment* pEnvironment, const EntityMovementData& movementData);
	~MovingEntity(void);

	bool Initialise(void);
	void Update(void);

	// Data access functions
	const XMFLOAT2& GetVelocity(void) const;
	float           GetMaxVelocity(void) const;
	float           GetMaxForce(void) const;
	float           GetMaxSeeAhead(void) const;
	void SetVelocity(const XMFLOAT2& velocity);
	void SetMaxVelocity(float maxVelocity);
	void SetMaxForce(float maxForce);
	void SetMaxSeeAhead(float maxSeeAhead);
private:
	EntityMovementData m_movementData;    // The movement properties of the entity
	MovementManager    m_movementManager; // The manager responsible for updating the entities velocity and position
};

#endif // MOVING_ENTITY_H