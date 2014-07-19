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
const float g_kProjectileMaxVelocity(20.0f);				// Determines how fast soldiers can move within the test environment
const float g_kProjectileMaxForce(20.0f);				    // Determines the maximum amount of forces that can impact the character
const float g_kProjectileMaxSeeAhead(0.0f);				    // Determines how far the soldier can look ahead in order to check for collisions
const float g_kProjectileMaxCollisionAvoidanceForce(0.0f);  // The maximal force that can result from avoiding collisions
const float g_kProjectileMaxSeparationForce(0.0f);          // The maximal force that can result from separation from other entities
const float g_kProjectileTargetReachedRadius(0.0f);		    // When the distance between an entity and its target is lower than this, latter one counts as reached
const float g_kProjectileSlowArrivalRadius(0.0f);			// When this close to the final target, an entity will start to slow down
const float g_kProjectileSeparationRadius(0.0f);			// When an entity registers other entities within this radius it will steer for separation from them

// Other
const float g_kProjectileScale(0.2f); // Determines how big projectiles are in relation to a grid field

#endif // PROJECTILE_PROPERTIES_H