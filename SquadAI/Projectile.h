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
#include "MovingEntity.h"

using namespace DirectX;

class Projectile : public MovingEntity
{
public:
	Projectile(void);
	~Projectile(void);

	bool Initialise(const XMFLOAT2& target, const EntityInitData& initData, const EntityMovementInitData& movementInitData);
	void Update(float deltaTime);
};

#endif // PROJECTILE_H