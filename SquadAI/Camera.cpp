/* 
*  Kevin Meergans, SquadAI, 2014
*  Camera.cpp
*  The camera used to navigate the application. It provides a top down
*  view of the scene and allows pan and zoom.
*/

// Includes
#include "Camera.h"

Camera::Camera() : m_position(0.0f, 0.0f, 0.0f), 
				   m_movementSpeed(0.1f, 0.1f, 0.1f),
				   m_right(1.0f, 0.0f, 0.0f), 
				   m_lookAt(0.0f, 0.0f, 1.0f), 
				   m_up(0.0f, 1.0f, 0.0f),
				   m_windowWidth(400),
		           m_windowHeight(400),
			       m_nearClippingPlane(1.0f),
				   m_farClippingPlane(1000.0f),
				   m_isOrthographic(false),
				   m_orthoZoomFactor(1.0f)
{
	XMStoreFloat4x4( &m_viewMatrix, XMMatrixIdentity() );
	XMStoreFloat4x4( &m_projectionMatrix, XMMatrixIdentity() );
}

Camera::~Camera()
{
	Cleanup();
}

//--------------------------------------------------------------------------------------
// Initialise the camera.
// Param1:  The initial position of the camera in world space.
// Param2:  Determines the direction, in which the camera is facing.
// Param3:  Determines the vector pointing upwards from the camera.
// Param4:  The field of view of the camera.
// Param5:  The width of the window the application is running in.
// Param6:  The height of the window the application is running in.
// Param7:  The near clipping plane of the camera.
// Param8:  The far clipping plane of the camera.
// Param9:  The initial movement speed of the camera along each axis.
// Param10: Determines whether the camera will use perspective or orthographic projection. 
// Returns true if the camera was initialised successfully, false otherwise.
//--------------------------------------------------------------------------------------
bool Camera::Initialise(const XMFLOAT3& position,const XMFLOAT3& lookAt,const XMFLOAT3& up, float fov, int windowWidth, int windowHeight, float clipNear, float clipFar, const XMFLOAT3& newMovementSpeed, bool isOrthographic)
{
	m_position          = position;
	m_lookAt            = lookAt;
	m_up	            = up;		
	m_windowWidth       = static_cast<float>(windowWidth);
	m_windowHeight      = static_cast<float>(windowHeight);
	m_nearClippingPlane = clipNear;
	m_farClippingPlane  = clipFar;
	m_isOrthographic    = isOrthographic;

	// Calculate the right vector
	XMStoreFloat3(&m_right, XMVector3Cross(XMLoadFloat3(&m_up), XMLoadFloat3(&m_lookAt)));
	
	// Calculate the view matrix
	XMStoreFloat4x4(&m_viewMatrix, XMMatrixLookAtLH(XMLoadFloat3(&m_position), XMLoadFloat3(&m_lookAt), XMLoadFloat3(&m_up)));

	if(m_isOrthographic)
	{
		XMStoreFloat4x4(&m_projectionMatrix, XMMatrixOrthographicLH(m_windowWidth * m_orthoZoomFactor, m_windowHeight * m_orthoZoomFactor, m_nearClippingPlane, m_farClippingPlane));
	}else
	{
		XMStoreFloat4x4(&m_projectionMatrix, XMMatrixPerspectiveFovLH(fov, static_cast<float>(windowWidth)/windowHeight, clipNear, clipFar ) );
	}

	return true;
}

//--------------------------------------------------------------------------------------
// Update the camera and compute the new view matrix according to the user input.
// Param1: A vector describing the camera movement.
//--------------------------------------------------------------------------------------
void Camera::Update(const XMFLOAT3& moveVector)
{
	if(moveVector.x == 0 && moveVector.y == 0 && moveVector.z == 0)
	{
		return;
	}

	// Update the camera position
	Move(moveVector);
	
	// Build the new view matrix

	m_viewMatrix._11 = m_right.x; 
	m_viewMatrix._12 = m_up.x; 
	m_viewMatrix._13 = m_lookAt.x;
	m_viewMatrix._14 = 0.0f;

	m_viewMatrix._21 = m_right.y; 
	m_viewMatrix._22 = m_up.y; 
	m_viewMatrix._23 = m_lookAt.y;
	m_viewMatrix._24 = 0.0f;

	m_viewMatrix._31 = m_right.z; 
	m_viewMatrix._32 = m_up.z; 
	m_viewMatrix._33 = m_lookAt.z;
	m_viewMatrix._34 = 0.0f;

	XMStoreFloat(&m_viewMatrix._41, - XMVector3Dot(XMLoadFloat3(&m_position), XMLoadFloat3(&m_right)));
	XMStoreFloat(&m_viewMatrix._42, - XMVector3Dot(XMLoadFloat3(&m_position), XMLoadFloat3(&m_up)));
	XMStoreFloat(&m_viewMatrix._43, - XMVector3Dot(XMLoadFloat3(&m_position), XMLoadFloat3(&m_lookAt)));
	m_viewMatrix._44 = 1.0f;
}

//--------------------------------------------------------------------------------------
// Updates the camera position according to the move vector passed to the function
// and taking the movement speed into account.
// Param1: A vector containing the movement speed factor for movement along each axis.
//--------------------------------------------------------------------------------------
void Camera::Move(const XMFLOAT3& moveVector)
{
	if(m_isOrthographic)
	{
		XMStoreFloat3(&m_position, XMLoadFloat3(&m_position) 
											- XMLoadFloat3(&m_right) * moveVector.x * m_movementSpeed.x		// add horizontal movement
											+ XMLoadFloat3(&m_up) * moveVector.y * m_movementSpeed.y		// add vertical movement
											);

		m_orthoZoomFactor -= moveVector.z * m_movementSpeed.z * g_cOrthoZoomWeight;
		if(m_orthoZoomFactor < g_cOrthoMinimalZoomFactor)
		{
			m_orthoZoomFactor = g_cOrthoMinimalZoomFactor;
		}else if(m_orthoZoomFactor > g_cOrthoMaximalZoomFactor)
		{
			m_orthoZoomFactor = g_cOrthoMaximalZoomFactor;
		}


		XMStoreFloat4x4(&m_projectionMatrix, XMMatrixOrthographicLH(m_windowWidth * m_orthoZoomFactor, m_windowHeight * m_orthoZoomFactor, m_nearClippingPlane, m_farClippingPlane));
	}else
	{
		XMStoreFloat3(&m_position, XMLoadFloat3(&m_position) 
											- XMLoadFloat3(&m_right) * moveVector.x * m_movementSpeed.x		// add horizontal movement
											+ XMLoadFloat3(&m_up) * moveVector.y * m_movementSpeed.y		// add vertical movement
											+ XMLoadFloat3(&m_lookAt) * moveVector.z * m_movementSpeed.z);	// add movement along z-axis
	}
}

//--------------------------------------------------------------------------------------
// Frees ressources associated to the camera.
//--------------------------------------------------------------------------------------
void Camera::Cleanup(void)
{
	// A placeholder at the moment.
}

// data access functions

const XMFLOAT4X4& Camera::GetViewMatrix(void) const
{
	return m_viewMatrix;
}

const XMFLOAT4X4& Camera::GetProjectionMatrix(void) const
{
	return m_projectionMatrix;
}

const XMFLOAT3& Camera::GetCameraPosition(void) const
{
	return m_position;
}

void Camera::SetMovementSpeed(const XMFLOAT3& newMovementSpeed)
{
	m_movementSpeed = newMovementSpeed;
}