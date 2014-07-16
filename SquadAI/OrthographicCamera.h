/* 
*  Kevin Meergans, SquadAI, 2014
*  OrthographicCamera.h
*  The orthographic camera used to navigate the application. It provides a top down
*  view of the scene and allows pan and zoom.
*/

// Note: Parts of this class could be moved into an abstract camera class that this class should then inherit from.

#ifndef ORTHOGRAPHIC_CAMERA_H
#define ORTHOGRAPHIC_CAMERA_H

// Includes
#include <DirectXMath.h>
#include "ApplicationSettings.h"

using namespace DirectX;

class OrthographicCamera
{
public:
	OrthographicCamera(void);
	~OrthographicCamera(void);

	bool Initialise(const XMFLOAT3& position,const XMFLOAT3& lookAt,const XMFLOAT3& up, int windowWidth, int windowHeight, float clipNear, float clipFar, const XMFLOAT3& newMovementSpeed, float initialZoomFactor);
	void Update(const XMFLOAT3& moveVector);
	void Cleanup(void);

	const XMFLOAT4X4& GetViewMatrix(void) const;
	const XMFLOAT4X4& GetBaseViewMatrix(void) const;
	const XMFLOAT4X4& GetProjectionMatrix(void) const;
	const XMFLOAT4X4& GetBaseProjectionMatrix(void) const;

	const XMFLOAT3&   GetCameraPosition(void) const;
	float             GetZoomFactor(void) const;

	void SetMovementSpeed(const XMFLOAT3& newMovementSpeed);

	float       m_zoomFactor;        // The current zoom factor
private:
	void Move(const XMFLOAT3& moveVector);

	XMFLOAT4X4	m_viewMatrix;		    // The view matrix associated to this camera
	XMFLOAT4X4	m_baseViewMatrix;		// The original view matrix the camera was initialised with
	XMFLOAT4X4	m_projectionMatrix;	    // The orthographic projection matrix associated to this camera
	XMFLOAT4X4	m_baseProjectionMatrix;	// The original orthographic projection matrix, without applied zoom

	XMFLOAT3	m_position;		    // The current position of the camera
	XMFLOAT3    m_movementSpeed;    // Determines how fast the camera moves, that is how sensible it is to user input

	XMFLOAT3	m_right;			// The vector from the camera pointing to the right
	XMFLOAT3	m_lookAt;			// The vector from the camera in direction to its current target (z-axis)
	XMFLOAT3	m_up;				// The vector facing up from the camera (y-axis)

	float	    m_windowWidth;       // The width of the application window in pixels
	float       m_windowHeight;      // The height of the application window in pixels
	float       m_nearClippingPlane; // The near clipping plane of the camera
	float       m_farClippingPlane;  // The far clipping plane of the camera

	
};

#endif // ORTHOGRAPHIC_CAMERA_H