/* 
*  Kevin Meergans, SquadAI, 2014
*  Projectile.cpp
*  This class represents a projectile fired from one of the fighting
*  entities in order to kill hostile entities.
*/

// Includes
#include "Projectile.h"

Projectile::Projectile(void) : MovingEntity()
{
}

Projectile::~Projectile(void)
{
}

//--------------------------------------------------------------------------------------
// Initialises the projectile entity.
// Param1: The type of the entity that fired this projectile.
// Param2: The target position for the projectile.
// Param3: The basic initialisation data for the entity.
// Param4: The movement specific initialisation data for the entity.
// Returns true if the projectile entity was initialised successfully, false otherwise.
//--------------------------------------------------------------------------------------
bool Projectile::Initialise(ObjectType originType, const XMFLOAT2& target, const EntityInitData& initData, const EntityMovementInitData& movementInitData)
{
	if(!MovingEntity::Initialise(initData, movementInitData))
	{
		return false;
	}

	m_originType = originType;

	// Set the velocity for the projectile (will be constant throughout)
	XMFLOAT2 velocity;
	XMStoreFloat2(&velocity, XMVector2Normalize(XMLoadFloat2(&target) - XMLoadFloat2(&GetPosition())) * GetMaxVelocity());
	SetVelocity(velocity);

	return true;
}

//--------------------------------------------------------------------------------------
// Updates the projectile entity.
// Param1: The time in seconds passed since the last frame.
//--------------------------------------------------------------------------------------
void Projectile::Update(float deltaTime)
{
	// Calculate the new target position for the projectile. Ensure that it is never reached and
	// the projectile keeps on flying infinitely.Projectile will be destroyed upon hitting something 
	// (walls surrounding the environment will stop it)

	XMFLOAT2 target;
	XMStoreFloat2(&target, XMLoadFloat2(&GetPosition()) + XMLoadFloat2(&GetVelocity()) * GetMaxVelocity());

	m_movementManager.Seek(target, GetTargetReachedRadius());

	MovingEntity::Update(deltaTime);
}

// Data access functions

ObjectType Projectile::GetOriginType(void) const
{
	return m_originType;
}