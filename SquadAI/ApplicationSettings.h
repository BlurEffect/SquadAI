/* 
*  Kevin Meergans, SquadAI, 2014
*  ApplicationSettings.h
*  Contains some constants determining certain properties of the
*  application and how its components are initialised.
*/

#ifndef APPLICATION_SETTINGS_H
#define APPLICATION_SETTINGS_H

// Includes
#include <DirectXMath.h>

using namespace DirectX;

// Camera settings
// Determine how the camera of the application is set up
const XMFLOAT3 g_cInitialCameraPosition(0.0f, 0.0f, -20.0f);
const XMFLOAT3 g_cInitialCameraLookAt(0.0f, 0.0f, 0.0f);
const XMFLOAT3 g_cCameraUpVector(0.0f, 1.0f, 0.0f);
const XMFLOAT3 g_cInitialCameraSpeed(0.1f, 0.1f, 0.1f);
const float    g_cCameraFieldOfView        = XM_PI/4;
const float    g_cCameraNearClippingPlane  = 1.0f;
const float    g_cCameraFarClippingPlane   = 1000.0f;
const bool     g_cCreateOrthographicCamera = true;

#endif // APPLICATION_SETTINGS_H