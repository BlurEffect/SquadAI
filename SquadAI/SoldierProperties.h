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
const float g_kSoldierMaxSpeed(10.0f);				     // Determines how fast soldiers can move within the test environment
const float g_kSoldierMaxForce(1.0f);					 // Determines the maximum amount of forces that can impact the character
const float g_kSoldierMaxSeeAhead(6.0f);				 // Determines how far the soldier can look ahead in order to check for collisions
const float g_kSoldierMaxCollisionAvoidanceForce(0.6f);  // The maximal force that can result from avoiding collisions
const float g_kSoldierMaxAvoidWallsForce(2.0f); //1.2         // The maximal force that can result from being pushed away from walls
const float g_kSoldierMaxSeparationForce(1.2f);          // The maximal force that can result from separation from other entities
const float g_kSoldierTargetReachedRadius(2.0f);		 // When the distance between an entity and its target is lower than this, latter one counts as reached

// Combat
const float g_kFireWeaponInterval(0.5f);					 // After every shot, the soldier has to wait this many seconds before he can fire his weapon again

// Sensors
const float g_kSoldierFieldOfView(XM_PI/6.0f);	// /6.0f	     // Determines the field of view of soldiers
const float g_kSoldierViewingDistance(20.0f);	 // 10.0f	     // Determines how far soldiers can see in order to spot enemies

// Other
const float g_kSoldierMaxHealth(100.0f);				 // The maximal health of a soldier entity.

#endif // SOLDIER_PROPERTIES_H