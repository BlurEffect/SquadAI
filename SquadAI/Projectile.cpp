/* 
*  Kevin Meergans, SquadAI, 2014
*  Projectile.cpp
*  This class represents a projectile fired from one of the fighting
*  entities in order to kill hostile entities.
*/

// Includes
#include "Projectile.h"

Projectile::Projectile(void) : CollidableObject(),
							   m_direction(0.0f, 0.0f),	
							   m_speed(0.0f),			
							   m_friendlyTeam(EntityTeam(0))
{
}

Projectile::~Projectile(void)
{
}

//--------------------------------------------------------------------------------------
// Initialises the projectile.
// Param1:  A unique identifier for the object.
// Param2:  The position of the object.
// Param3:  The rotation of the object along the world z-axis.
// Param4:  The uniform scale of the object.
// Param5:  The category the object belongs to.
// Param6:  The type of the collider that should be created.
// Param7:  A pointer to the initialisation data for the collider.
// Param8:  The position, from which the projectile was fired.
// Param9:  The direction, into which the projectile should fly.
// Param10:  The speed, at which the projectile will travel.
// Param11: The id of the entity that shot the projectile.
// Param12: The team that fired the projectile.
// Returns true if the projectile was initialised successfully, false otherwise.
//--------------------------------------------------------------------------------------
bool Projectile::Initialise(unsigned long id, const XMFLOAT2& position, float rotation, float uniformScale, ObjectCategory category, ColliderType colliderType, void* pColliderData, const XMFLOAT2& origin, const XMFLOAT2& direction, float speed, unsigned long shooterId, EntityTeam friendlyTeam)
{
	if(!CollidableObject::Initialise(id, position, rotation, uniformScale, category, colliderType, pColliderData))
	{
		return false;
	}

	m_origin       = origin;
	m_direction    = direction;
	m_speed        = speed;
	m_shooterId    = shooterId;
	m_friendlyTeam = friendlyTeam;

	return true;
}

//--------------------------------------------------------------------------------------
// Updates the projectile.
// Param1: The time in seconds passed since the last frame.
//--------------------------------------------------------------------------------------
void Projectile::Update(float deltaTime)
{
	// Calculate the new target position for the projectile.

	XMFLOAT2 newPosition;
	XMStoreFloat2(&newPosition, XMLoadFloat2(&GetPosition()) + XMVector2Normalize(XMLoadFloat2(&m_direction)) * m_speed * deltaTime);
	
	SetPosition(newPosition);
	UpdateColliderPosition(newPosition);
}

// Data access functions

const XMFLOAT2& Projectile::GetOrigin(void) const
{
	return m_origin;
}

const XMFLOAT2& Projectile::GetDirection(void) const
{
	return m_direction;
}

float Projectile::GetSpeed(void) const
{
	return m_speed;
}

unsigned long Projectile::GetShooterId(void) const
{
	return m_shooterId;
}

EntityTeam Projectile::GetFriendlyTeam(void) const
{
	return m_friendlyTeam;
}

void Projectile::SetOrigin(const XMFLOAT2& origin)
{
	m_origin = origin;
}

void Projectile::SetDirection(const XMFLOAT2& direction)
{
	m_direction = direction;
}

void Projectile::SetSpeed(float speed)
{
	m_speed = speed;
}

void Projectile::SetShooterId(unsigned long shooterId)
{
	m_shooterId = shooterId;
}

void Projectile::SetFriendlyTeam(EntityTeam team)
{
	m_friendlyTeam = team;
}
