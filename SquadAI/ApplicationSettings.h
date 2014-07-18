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

// Movement Manager
const float g_kSoldierMaxCollisionAvoidanceForce(10.0f); // The maximal force that can result from avoiding collisions
const float g_kSoldierMaxSeparationForce(4.0f);          // The maximal force that can result from separation from other entities
const float g_kSoldierTargetReachedRadius(2.0f);		  // When the distance between an entity and its target is lower than this, latter one counts as reached
const float g_kSoldierSlowArrivalRadius(2.0f);			  // When this close to the final target, an entity will start to slow down
const float g_kSoldierSeparationRadius(3.0f);			  // When an entity registers other entities within this radius it will steer for separation from them

// Entity properties
const float g_kSoldierMaxVelocity(10.0f); // Determines how fast soldiers can move within the test environment
const float g_kSoldierMaxForce(0.5f);     // Determines the maximum amount of forces that can impact the character
const float g_kSoldierMaxSeeAhead(3.0f);  // Determines how far the soldier can look ahead in order to check for collisions

const float g_kSoldierFieldOfView(XM_PI/6.0f);
const float g_kSoldierViewingDistance(10.0f);

// 0.1
// 0.05f
// 0.001f
// 3.0f
#endif // APPLICATION_SETTINGS_H