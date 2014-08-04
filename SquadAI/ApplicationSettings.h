/* 
*  Kevin Meergans, SquadAI, 2014
*  ApplicationSettings.h
*  Contains some constants determining certain properties of the
*  application and how its components are initialised.
*/

// Note: In terms of encapsulation it would be better to split these up over several files
//       but in favour of convenience and easy tweaking they are all assembled here.

#ifndef APPLICATION_SETTINGS_H
#define APPLICATION_SETTINGS_H

// Includes
#include <unordered_map>
#include <DirectXMath.h>

using namespace DirectX;

// Camera initialisation settings
const XMFLOAT3 g_kInitialCameraPosition(0.0f, 0.0f, -500.0f);
const XMFLOAT3 g_kInitialCameraLookAt(0.0f, 0.0f, 1.0f);
const XMFLOAT3 g_kCameraUpVector(0.0f, 1.0f, 0.0f);
const XMFLOAT3 g_kInitialCameraSpeed(0.1f, 0.1f, 0.0001f);
const float    g_kCameraNearClippingPlane  = 1.0f;
const float    g_kCameraFarClippingPlane   = 1000.0f;
const float    g_kInitialCameraZoomFactor = 0.1f;

// Camera properties
const float g_kOrthoMinimalZoomFactor = 0.01f;  // Determines the minimal zoom factor for a camera using orthographic projection (should be greater than zero)
const float g_kOrthoMaximalZoomFactor = 100.0f; // Determines the maximal zoom factor for a camera using orthographic projection

// Renderer Settings
const float    g_kBackgroundColour[4] = {0.0f, 0.0f, 0.0f, 1.0f}; // The colour to which the backbuffer is cleared
const XMFLOAT4 g_kGridColour(1.0f, 1.0f, 1.0f, 1.0f);			  // The colour of the grid representing the test environment

// Test environment settings
const unsigned int g_kSoldiersPerTeam = 1; // The number of soldiers forming a team during the matches

// Game settings
const float g_kPickupFlagRadiusRelative = 0.25f; // An entity has to approach a flag this close (in relation to the grid spacing) in order to pick it up or return it
const float g_kFlagResetTimer		    = 10.0f; // A flag will be reset to its start position after it was dropped for this long
const float g_kRespawnTimer				= 5.0f;  // Killed entities have to wait this long before being able to respawn 

// Projectile settings
const float g_kProjectileSpeed(30.0f);   // Determines how fast projectiles can move within the test environment
const float g_kProjectileDamage(20.0f);  // The damage that an entity suffers when hit by a hostile projectile.


#endif // APPLICATION_SETTINGS_H