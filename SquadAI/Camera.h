/* 
*  Kevin Meergans, SquadAI, 2014
*  Camera.h
*  The camera used to navigate the application. It provides a top down
*  view of the scene and allows pan and zoom.
*/

#ifndef CAMERA_H
#define CAMERA_H

// Includes
#include <DirectXMath.h>

using namespace DirectX;

class Camera
{
public:
	Camera(void);
	~Camera(void);

	bool Initialise(const XMFLOAT3& position,const XMFLOAT3& lookAt,const XMFLOAT3& up, float fov, int windowWidth, int windowHeight, float clipNear, float clipFar, const XMFLOAT3& newMovementSpeed, bool isOrthographic);
	void Update(const XMFLOAT3& moveVector);
	void Cleanup(void);

	inline const XMFLOAT4X4* GetViewMatrix(void) const;
	inline const XMFLOAT4X4* GetProjectionMatrix(void) const;
	inline const XMFLOAT3*   GetCameraPosition(void) const;

	inline void SetMovementSpeed(const XMFLOAT3& newMovementSpeed);
private:
	void Move(const XMFLOAT3& moveVector);

	XMFLOAT4X4	m_viewMatrix;		// The view matrix associated to this camera
	XMFLOAT4X4	m_projectionMatrix;	// The projection matrix associated to this camera
	
	XMFLOAT3	m_position;		    // The current position of the camera
	XMFLOAT3    m_movementSpeed;    // Determines how fast the camera moves, that is how sensible it is to user input

	XMFLOAT3	m_right;			// The vector from the camera pointing to the right
	XMFLOAT3	m_lookAt;			// The vector from the camera in direction to its current target (z-axis)
	XMFLOAT3	m_up;				// The vector facing up from the camera (y-axis)
};

#endif // CAMERA_H