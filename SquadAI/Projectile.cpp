/* 
*  Kevin Meergans, SquadAI, 2014
*  Projectile.cpp
*  This class represents a projectile fired from one of the fighting
*  entities in order to kill hostile entities.
*/

// Includes
#include "Projectile.h"

Projectile::Projectile(const XMFLOAT2& position, float rotation, float uniformScale) : Object(position, rotation, uniformScale),
																					   m_direction(0.0f, 0.0f),	
																		               m_speed(0.0f),			
																					   m_friendlyTeam(EntityTeam(0)),
																					   m_pCollider(nullptr)	
{
}

Projectile::~Projectile(void)
{
	if(m_pCollider)
	{
		delete m_pCollider;
	}
}

//--------------------------------------------------------------------------------------
// Initialises the projectile.
// Param1: The direction, into which the projectile should fly.
// Param2: The speed, at which the projectile will travel.
// Param3: The team that fired the projectile.
// Param4: A pointer to the collider that should be used as blueprint for the projectile's collider
// Returns true if the projectile was initialised successfully, false otherwise.
//--------------------------------------------------------------------------------------
bool Projectile::Initialise(const XMFLOAT2& direction, float speed, EntityTeam friendlyTeam, Collider* pCollider)
{
	m_direction    = direction;
	m_speed        = speed;
	m_friendlyTeam = friendlyTeam;

	m_pCollider = ColliderFactory::CreateCollider(pCollider);

	if(!m_pCollider)
	{
		return false;
	}

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
}

// Data access functions

const XMFLOAT2& Projectile::GetDirection(void) const
{
	return m_direction;
}

float Projectile::GetSpeed(void) const
{
	return m_speed;
}

EntityTeam Projectile::GetFriendlyTeam(void) const
{
	return m_friendlyTeam;
}

const Collider* Projectile::GetCollider(void) const
{
	return m_pCollider;
}

void Projectile::SetDirection(const XMFLOAT2& direction)
{
	m_direction = direction;
}

void Projectile::SetSpeed(float speed)
{
	m_speed = speed;
}

void Projectile::SetFriendlyTeam(EntityTeam team)
{
	m_friendlyTeam = team;
}
