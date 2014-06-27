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
	/*
	return m_camera.Initialise(g_cInitialCameraPosition, g_cInitialCameraLookAt, g_cCameraUpVector, g_cCameraFieldOfView, 
							   windowWidth, windowHeight, g_cCameraNearClippingPlane, g_cCameraFarClippingPlane, 
		                       g_cInitialCameraSpeed, g_cCreateOrthographicCamera) &&
		   m_inputManager.Initialise(hInst, hWnd) &&
		   m_renderer.Initialise(hWnd, windowWidth, windowHeight) &&
		   m_testEnvironment.Initialise();
	*/
	if(!m_camera.Initialise(g_cInitialCameraPosition, g_cInitialCameraLookAt, g_cCameraUpVector, windowWidth, windowHeight, 
						    g_cCameraNearClippingPlane, g_cCameraFarClippingPlane, g_cInitialCameraSpeed))
	{
		return false;
	}

	if(! m_inputManager.Initialise(hInst, hWnd))
	{
		return false;
	}

	if(!m_renderer.Initialise(hWnd, windowWidth, windowHeight, m_camera.GetViewMatrix(), m_camera.GetProjectionMatrix()))
	{
		return false;
	}

	if(!m_testEnvironment.Initialise())
	{
		return false;
	}

	return true;
}

//--------------------------------------------------------------------------------------
// Updates the components of the application.
//--------------------------------------------------------------------------------------
void Application::Update()
{
	m_inputManager.Update();
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