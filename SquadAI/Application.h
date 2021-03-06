/* 
*  Kevin Meergans, SquadAI, 2014
*  Application.h
*  Central class to the application. Initialises and updates components and coordinates
*  the communication between them.
*/

#ifndef APPLICATION_H
#define APPLICATION_H

// Includes
#include <Windows.h>
#include <DirectXMath.h>
#include <string>
#include <iostream>

#include "OrthographicCamera.h"
#include "InputManager.h"
#include "Renderer.h"
#include "TestEnvironment.h"
#include "ApplicationSettings.h"
#include "AppData.h"
#include "ObjectTypes.h"
#include "PerformanceTimer.h"

using namespace DirectX;

class Application
{
public:
	Application(void);
	~Application(void);

	bool Initialise(HINSTANCE hInst, PHANDLER_ROUTINE HandlerRoutine, HWND hWnd, unsigned int windowWidth, unsigned int windowHeight);
	void Update(void);
	void Cleanup(void);

private:

	void ProcessInput(void);
	bool SaveTestEnvironment(void);
	bool LoadTestEnvironment(void);
	bool CreateNewTestEnvironment(void);

	PHANDLER_ROUTINE   m_pHandlerRoutine;  // This handler is set as console handler for all allocated consoles

	AppData            m_appData;          // The current state and properties of the app
	OrthographicCamera m_camera;		   // The camera component of the application
	InputManager	   m_inputManager;	   // The input component of the application
	Renderer		   m_renderer;		   // The renderer component of the application
	TestEnvironment    m_testEnvironment;  // The test environment used ->could be array later on
	PerformanceTimer   m_performanceTimer; // The timer object used to get the current FPS and delta time
};

#endif // APPLICATION_H