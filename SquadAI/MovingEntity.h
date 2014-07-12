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

class MovingEntity : public Entity
{
public:
	MovingEntity(void);
	MovingEntity(unsigned long id, EntityType type, const XMFLOAT2& position, float rotation, TestEnvironment* pEnvironment, float maxVelocity, float maxForce);
	~MovingEntity(void);

	// Data access functions
	const XMFLOAT2& GetVelocity(void) const;
	float           GetMaxVelocity(void) const;
	float           GetMaxForce(void) const;
	void SetVelocity(const XMFLOAT2& velocity);
	void SetMaxVelocity(const float maxVelocity);
	void SetMaxForce(const float maxForce);

private:
	XMFLOAT2        m_velocity;			// The current velocity of the entity
	float           m_maxVelocity;		// The maximal velocity allowed (limits the length of the velocity vector)
	float           m_maxForce;         // The maximal force that can impact this entity's movement
	MovementManager m_movementManager;  // The manager responsible for updating the entities velocity and position
};

#endif // MOVING_ENTITY_H