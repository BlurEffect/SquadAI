/* 
*  Kevin Meergans, SquadAI, 2014
*  SoldierProperties.h
*  Contains some constants determining certain properties of the
*  soldier entity, mostly associated to the movement and combat
*  behaviour.
*/

#ifndef SOLDIER_PROPERTIES_H
#define SOLDIER_PROPERTIES_H

// Includes
#include <DirectXMath.h>

using namespace DirectX;

// Movement
const float g_kSoldierMaxVelocity(15.0f);				 // Determines how fast soldiers can move within the test environment
const float g_kSoldierMaxForce(0.5f);					 // Determines the maximum amount of forces that can impact the character
const float g_kSoldierMaxSeeAhead(5.0f);				 // Determines how far the soldier can look ahead in order to check for collisions
const float g_kSoldierMaxCollisionAvoidanceForce(10.0f); // The maximal force that can result from avoiding collisions
const float g_kSoldierMaxSeparationForce(4.0f);          // The maximal force that can result from separation from other entities
const float g_kSoldierTargetReachedRadius(2.0f);		 // When the distance between an entity and its target is lower than this, latter one counts as reached
const float g_kSoldierSlowArrivalRadius(2.0f);			 // When this close to the final target, an entity will start to slow down
const float g_kSoldierSeparationRadius(3.0f);			 // When an entity registers other entities within this radius it will steer for separation from them

// Sensors
const float g_kSoldierFieldOfView(XM_PI/6.0f); // Determines the field of view of soldiers
const float g_kSoldierViewingDistance(10.0f);  // Determines how far soldiers can see in order to spot enemies

// Combat
const float g_kSoldierMaxHealth(100.0f); // The maximal health of a soldier entity.

#endif // SOLDIER_PROPERTIES_H