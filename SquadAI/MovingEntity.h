/* 
*  Kevin Meergans, SquadAI, 2014
*  MovingEntity.h
*  Abstract base class for all moving entities in the test environment.
*/

#ifndef MOVING_ENTITY_H
#define MOVING_ENTITY_H

// Includes
#include <DirectXMath.h>
#include "Entity.h"
#include "EntityMovementManager.h"

using namespace DirectX;

//--------------------------------------------------------------------------------------
// Bundles the data defining the properties of the entity related to movement.
//--------------------------------------------------------------------------------------
struct EntityMovementInitData
{
	EntityMovementInitData(void) : m_maxVelocity(0.0f),
							       m_maxTotalForce(0.0f),
							       m_maxSeeAhead(0.0f),
								   m_maxCollisionAvoidanceForce(0.0f),
								   m_maxSeparationForce(0.0f),       
								   m_targetReachedRadius(0.0f),		
								   m_slowArrivalRadius(0.0f),		 
								   m_separationRadius(0.0f)
	{}

	EntityMovementInitData(float maxVelocity, float maxTotalForce, float maxSeeAhead, float maxCollisionAvoidanceForce, 
		                   float maxSeparationForce, float targetReachedRadius, float slowArrivalRadius, float separationRadius) 
				: m_maxVelocity(maxVelocity),
				  m_maxTotalForce(maxTotalForce),
				  m_maxSeeAhead(maxSeeAhead),
				  m_maxCollisionAvoidanceForce(maxCollisionAvoidanceForce),
				  m_maxSeparationForce(maxSeparationForce),       
				  m_targetReachedRadius(targetReachedRadius),		
				  m_slowArrivalRadius(slowArrivalRadius),		 
				  m_separationRadius(separationRadius)
	{}

	float    m_maxVelocity;		// The maximal velocity allowed (limits the length of the velocity vector)
	float    m_maxTotalForce;	// The maximal force that can impact this entity's movement
	float    m_maxSeeAhead;		// The distance that the entity can see ahead and check for collisions with other entities

	float m_maxCollisionAvoidanceForce; // The maximal force that can result from avoiding collisions
	float m_maxSeparationForce;         // The maximal force that can result from separation from other entities
	float m_targetReachedRadius;		// When the distance between an entity and its target is lower than this, latter one counts as reached
	float m_slowArrivalRadius;		    // When this close to the final target, an entity will start to slow down
	float m_separationRadius;		    // When an entity registers other entities within this radius it will steer for separation from them
};


class MovingEntity : public Entity
{
public:
	MovingEntity(void);
	virtual ~MovingEntity(void) = 0;

	bool Initialise(const EntityInitData& initData, const EntityMovementInitData& movementInitData);
	virtual void Update(float deltaTime);
	virtual void Reset(void);

	// Data access functions

	const XMFLOAT2& GetVelocity(void) const;
	float           GetMaxVelocity(void) const;
	float           GetMaxTotalForce(void) const;
	float           GetMaxSeeAhead(void) const;
	float           GetMaxCollisionAvoidanceForce(void) const;
	float           GetMaxSeparationForce(void) const;
	float           GetTargetReachedRadius(void) const;
	float           GetSlowArrivalRadius(void) const;
	float           GetSeparationRadius(void) const;

	void SetVelocity(const XMFLOAT2& velocity);
	void SetMaxVelocity(float maxVelocity);
	void SetMaxTotalForce(float maxForce);
	void SetMaxSeeAhead(float maxSeeAhead);
	void SetMaxCollisionAvoidanceForce(float maxCollisionAvoidanceForce);
	void SetMaxSeparationForce(float maxSeparationForce);
	void SetTargetReachedRadius(float targetReachedRadius);
	void SetSlowArrivalRadius(float slowArrivalRadius);
	void SetSeparationRadius(float separationRadius);

private:
	EntityMovementManager    m_movementManager; // The manager responsible for updating the entities velocity and position
};

#endif // MOVING_ENTITY_H