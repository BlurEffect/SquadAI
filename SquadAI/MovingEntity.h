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

using namespace DirectX;

class MovingEntity : public Entity
{
public:
	MovingEntity(void);
	MovingEntity(unsigned long id, EntityType type, const XMFLOAT2& position, float rotation);
	~MovingEntity(void);

	// Data access functions
	const XMFLOAT2& GetVelocity(void) const;
	void SetVelocity(const XMFLOAT2& velocity);

private:
	XMFLOAT2 m_velocity;
};

#endif // MOVING_ENTITY_H