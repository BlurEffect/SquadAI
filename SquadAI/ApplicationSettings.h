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


#endif // APPLICATION_SETTINGS_H