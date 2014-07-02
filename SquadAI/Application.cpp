/* 
*  Kevin Meergans, SquadAI, 2014
*  Application.cpp
*  Central class to the application. Initialises and updates components and coordinates
*  the communication between them.
*/

// Includes
#include "Application.h"

//--------------------------------------------------------------------------------------
// Initialise the components of the application.
// Returns true if all components were successfully initialised, false otherwise.
//--------------------------------------------------------------------------------------
bool Application::Initialise(HINSTANCE hInst, HWND hWnd, unsigned int windowWidth, unsigned int windowHeight)
{
	m_appData.m_windowWidth  = windowWidth;
	m_appData.m_windowHeight = windowHeight;

	TestEnvironmentData initData;
	initData.m_gridWidth			    = 50.0f;
	initData.m_gridHeight			    = 50.0f;
	initData.m_gridHorizontalPartitions = 50;
	initData.m_gridVerticalPartitions   = 50;

	return m_camera.Initialise(g_cInitialCameraPosition, g_cInitialCameraLookAt, g_cCameraUpVector, 
							   windowWidth, windowHeight, g_cCameraNearClippingPlane, g_cCameraFarClippingPlane, 
		                       g_cInitialCameraSpeed) &&
		   m_inputManager.Initialise(hInst, hWnd) &&
		   m_renderer.Initialise(hWnd, windowWidth, windowHeight, m_camera.GetViewMatrix(), m_camera.GetProjectionMatrix(), initData) &&
		   m_testEnvironment.Initialise(initData);

	return true;
}

//--------------------------------------------------------------------------------------
// Updates the components of the application.
//--------------------------------------------------------------------------------------
void Application::Update()
{
	m_inputManager.Update();
	ProcessInput();
	m_camera.Update(m_inputManager.GetCameraMovement());
	m_testEnvironment.Update(m_renderer.GetRenderContext());
	m_renderer.RenderScene(m_camera.GetViewMatrix(), m_camera.GetProjectionMatrix());
}

//--------------------------------------------------------------------------------------
// Releases resources used by the application and terminates the components.
//--------------------------------------------------------------------------------------
void Application::Cleanup()
{
	m_camera.Cleanup();
	m_inputManager.Cleanup();
	m_renderer.Cleanup();
	m_testEnvironment.Cleanup();
}

//--------------------------------------------------------------------------------------
// Processes the user input for the current frame and updates the application data
// accordingly.
//--------------------------------------------------------------------------------------
void Application::ProcessInput(void)
{
	if(m_inputManager.GetBrowseRight())
	{
		m_appData.m_selectedEntityType = EntityType((m_appData.m_selectedEntityType + 1) % NumberOfEntityTypes);
	}else if(m_inputManager.GetBrowseRight())
	{
		m_appData.m_selectedEntityType = EntityType((m_appData.m_selectedEntityType - 1) % NumberOfEntityTypes);
	}

	if(m_inputManager.GetLeftClick())
	{
		// Translate the cursor position to world space
		XMFLOAT2 cursorPos(m_inputManager.GetCursorPosition().x, m_inputManager.GetCursorPosition().y);

		cursorPos.x -= m_appData.m_windowWidth/2;
		cursorPos.y  = -cursorPos.y + m_appData.m_windowHeight/2;

		XMStoreFloat2(&cursorPos, XMLoadFloat2(&cursorPos) * m_camera.m_zoomFactor);

		XMFLOAT4X4 inverseViewMatrix;
		XMVECTOR determinant;
		XMStoreFloat4x4(&inverseViewMatrix, XMMatrixInverse(&determinant, XMLoadFloat4x4(&m_camera.GetViewMatrix())));

		XMFLOAT3 cameraTranslation;
		cameraTranslation.x = m_camera.GetViewMatrix()._41;
		cameraTranslation.y = m_camera.GetViewMatrix()._42;
		cameraTranslation.z = m_camera.GetViewMatrix()._43;
		
		XMStoreFloat2(&cursorPos, XMLoadFloat2(&cursorPos) - XMLoadFloat3(&cameraTranslation));
	}
}