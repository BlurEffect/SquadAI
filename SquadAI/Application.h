/* 
*  Kevin Meergans, SquadAI, 2014
*  Application.h
*  Central class to the application. Initialises and updates components and coordinates
*  the communication between them.
*/

#ifndef APPLICATION_H
#define APPLICATION_H

// Includes
#include "Camera.h"
#include "InputManager.h"
#include "Renderer.h"
#include "TestEnvironment.h"
#include "ApplicationSettings.h"

class Application
{
public:
	bool Initialise(HINSTANCE hInst, HWND hWnd, unsigned int windowWidth, unsigned int windowHeight);
	void Update(void);
	void Cleanup(void);
private:
	Camera			m_camera;			// The camera component of the application
	InputManager	m_inputManager;		// The input component of the application
	Renderer		m_renderer;			// The renderer component of the application
	TestEnvironment m_testEnvironment; // The test environment used ->could be array later on
};

#endif // APPLICATION_H