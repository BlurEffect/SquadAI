/* 
*  Kevin Meergans, SquadAI, 2014
*  Projectile.h
*  This class represents a projectile fired from one of the fighting
*  entities in order to kill hostile entities.
*/

#ifndef PROJECTILE_H
#define PROJECTILE_H

// Includes
#include <DirectXMath.h>
#include "Object.h"
#include "EntityData.h"
#include "CollidableObject.h"

using namespace DirectX;

class Projectile : public CollidableObject
{
public:
	Projectile(void);
	~Projectile(void);

	bool Initialise(const XMFLOAT2& position, float rotation, float uniformScale, ColliderType colliderType, void* pColliderData, const XMFLOAT2& direction, float speed, EntityTeam friendlyTeam);
	void Update(float deltaTime);

	// Data access functions

	const XMFLOAT2& GetDirection(void) const;
	float			GetSpeed(void) const;
	EntityTeam      GetFriendlyTeam(void) const;

	void SetDirection(const XMFLOAT2& direction);
	void SetSpeed(float speed);
    void SetFriendlyTeam(EntityTeam team);

private:
	XMFLOAT2   m_direction;			// The direction, in which the projectile is flying
	float      m_speed;				// The speed, at which the projectile is flying
	EntityTeam m_friendlyTeam;		// The team that fired this projectile
};

/*
class Projectile : public MovingEntity
{
public:
	Projectile(void);
	~Projectile(void);

	bool Initialise(ObjectType originType, const XMFLOAT2& target, const EntityInitData& initData, const EntityMovementInitData& movementInitData);
	void Update(float deltaTime);

	// Data access functions
	ObjectType GetOriginType(void) const;

private:
	ObjectType m_originType; // Entities of this type are immune to the projectiles.
};*/

#endif // PROJECTILE_H