/* 
*  Kevin Meergans, SquadAI, 2014
*  ProjectileProperties.h
*  Contains some constants determining certain properties of the
*  projectile entity, mostly associated to the movement behaviour.
*/

#ifndef PROJECTILE_PROPERTIES_H
#define PROJECTILE_PROPERTIES_H

// Includes
#include <DirectXMath.h>

using namespace DirectX;

// Movement
const float g_kProjectileMaxVelocity(30.0f);				// Determines how fast projectiles can move within the test environment
const float g_kProjectileMaxForce(20.0f);				    // Determines the maximum amount of forces that can impact the projectile

// Other
const float g_kProjectileScale(0.2f);   // Determines how big projectiles are in relation to a grid field
const float g_kProjectileDamage(20.0f); // The damage that an entity suffers when hit by a hostile projectile.


#endif // PROJECTILE_PROPERTIES_H