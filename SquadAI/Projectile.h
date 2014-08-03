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
#include "ObjectTypes.h"
#include "CollidableObject.h"

using namespace DirectX;

class Projectile : public CollidableObject
{
public:
	Projectile(void);
	~Projectile(void);

	bool Initialise(unsigned long id, const XMFLOAT2& position, float rotation, float uniformScale, ObjectCategory category, ColliderType colliderType, void* pColliderData, const XMFLOAT2& origin, const XMFLOAT2& direction, float speed, unsigned long shooterId, EntityTeam friendlyTeam);
	void Update(float deltaTime);

	// Data access functions

	const XMFLOAT2& GetOrigin(void) const;
	const XMFLOAT2& GetDirection(void) const;
	float			GetSpeed(void) const;
	unsigned long   GetShooterId(void) const;
	EntityTeam      GetFriendlyTeam(void) const;

	void SetOrigin(const XMFLOAT2& origin);
	void SetDirection(const XMFLOAT2& direction);
	void SetSpeed(float speed);
	void SetShooterId(unsigned long shooterId);
    void SetFriendlyTeam(EntityTeam team);

private:
	XMFLOAT2      m_origin;       // The position, from which the projectile was fired
	XMFLOAT2      m_direction;	  // The direction, in which the projectile is flying
	float         m_speed;		  // The speed, at which the projectile is flying
	unsigned long m_shooterId;    // The id of the entity that shot this projectile
	EntityTeam    m_friendlyTeam; // The team that fired this projectile
};

#endif // PROJECTILE_H