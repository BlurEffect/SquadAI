/* 
*  Kevin Meergans, SquadAI, 2014
*  CameraSettings.h
*  Contains constants determining some properties of the
*  camera used by the application
*/

#ifndef CAMERA_SETTINGS
#define CAMERA_SETTINGS

const float g_cOrthoZoomWeight        = 0.01f; // This factor is multiplied with the originally computed delta zoom factor to achieve 
										       // a similar zoom behaviour than when using perspective projections
const float g_cOrthoMinimalZoomFactor = 0.01f; // Determines the minimal zoom factor for a camera using orthographic projection (should be greater than zero)
const float g_cOrthoMaximalZoomFactor = 100.0f; // Determines the maximal zoom factor for a camera using orthographic projection

#endif // CAMERA_SETTINGS