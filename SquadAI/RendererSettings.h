/* 
*  Kevin Meergans, SquadAI, 2014
*  RendererSettings.h
*  Contains some constants defining the properties of the renderer.
*/

#ifndef RENDERER_SETTINGS_H
#define RENDERER_SETTINGS_H

// Includes
#include <DirectXMath.h>

using namespace DirectX;

const float    g_cBackgroundColour[4] = {0.0f, 0.0f, 0.0f, 1.0f}; // The colour to which the backbuffer is cleared
const XMFLOAT4 g_cGridColour(1.0f, 1.0f, 1.0f, 1.0f); // The colour of the grid representing the test environment

#endif // RENDERER_SETTINGS_H