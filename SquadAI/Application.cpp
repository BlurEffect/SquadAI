/* 
*  Kevin Meergans, SquadAI, 2014
*  Application.cpp
*  Central class to the application. Initialises and updates components and coordinates
*  the communication between them.
*/

// Includes
#include "Application.h"

Application::Application(void) : m_pHandlerRoutine(nullptr)
{
}

Application::~Application(void)
{
}

//--------------------------------------------------------------------------------------
// Initialise the components of the application.
// Param1: The instance to use.
// Param2: The handler routine that should be set as console handler to allow for proper cleanup.
// Param3: The width of the application window in pixels.
// Param3: The height of the application window in pixels.
// Returns true if all components were successfully initialised, false otherwise.
//--------------------------------------------------------------------------------------
bool Application::Initialise(HINSTANCE hInst, PHANDLER_ROUTINE pHandlerRoutine, HWND hWnd, unsigned int windowWidth, unsigned int windowHeight)
{
	m_pHandlerRoutine = pHandlerRoutine;

	m_appData.m_windowWidth  = windowWidth;
	m_appData.m_windowHeight = windowHeight;

	float        gridSize	            = 50.0f;
	unsigned int numberOfGridPartitions = 20;

	m_performanceTimer.Initialise();

	return m_camera.Initialise(g_kInitialCameraPosition, g_kInitialCameraLookAt, g_kCameraUpVector, 
							   windowWidth, windowHeight, g_kCameraNearClippingPlane, g_kCameraFarClippingPlane, 
		                       g_kInitialCameraSpeed, g_kInitialCameraZoomFactor) &&
		   m_inputManager.Initialise(hInst, hWnd) &&
		   m_renderer.Initialise(hWnd, windowWidth, windowHeight, m_camera.GetBaseViewMatrix(), m_camera.GetBaseProjectionMatrix()) &&
		   m_renderer.SetupGrid(gridSize, numberOfGridPartitions) &&
		   m_testEnvironment.Initialise(gridSize, numberOfGridPartitions);

	return true;
}

//--------------------------------------------------------------------------------------
// Updates the components of the application.
//--------------------------------------------------------------------------------------
void Application::Update()
{
	m_performanceTimer.Update();

	m_appData.m_framesPerSecond = m_performanceTimer.GetFPS();

	m_inputManager.Update();
	ProcessInput();
	m_camera.Update(m_inputManager.GetCameraMovement());
	m_testEnvironment.Update(m_renderer.GetRenderContext(), m_performanceTimer.GetDeltaTime());
	m_renderer.RenderScene(m_camera.GetViewMatrix(), m_camera.GetProjectionMatrix(), m_appData, m_testEnvironment.GetGameContext());
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
	// If desired by the user, quit the application.
	if(m_inputManager.GetExitApplication())
	{
		PostQuitMessage(0);
		return;
	}

	// Check if the user wants to create a new test application
	if(m_appData.m_applicationState == EditMode && m_inputManager.GetNewEnvironment())
	{
		CreateNewTestEnvironment();
		return;
	}

	// Update the state of the application.

	if(m_inputManager.GetToggleMode())
	{
		if(m_appData.m_applicationState == EditMode)
		{
			if(m_testEnvironment.StartSimulation())
			{
				m_appData.m_applicationState = SimulationRunning;
			}
		}else
		{
			m_appData.m_applicationState = EditMode;
			m_testEnvironment.EndSimulation();
		}
	}

	if((m_appData.m_applicationState != EditMode) && (m_inputManager.GetTogglePaused()))
	{
		if(m_appData.m_applicationState == SimulationRunning)
		{
			m_appData.m_applicationState = SimulationPaused;
			m_testEnvironment.PauseSimulation();
		}else
		{
			m_appData.m_applicationState = SimulationRunning;
			m_testEnvironment.ResumeSimulation();
		}
	}

	// Translate the cursor position from screen space to world space

	XMFLOAT2 cursorPos(static_cast<float>(m_inputManager.GetCursorPosition().x), static_cast<float>(m_inputManager.GetCursorPosition().y));

	cursorPos.x -= m_appData.m_windowWidth/2;
	cursorPos.y  = -cursorPos.y + m_appData.m_windowHeight/2;

	XMStoreFloat2(&cursorPos, XMLoadFloat2(&cursorPos) * m_camera.m_zoomFactor);

	XMFLOAT4X4 inverseViewMatrix;
	XMVECTOR determinant;
	XMStoreFloat4x4(&inverseViewMatrix, XMMatrixInverse(&determinant, XMLoadFloat4x4(&m_camera.GetViewMatrix())));

	XMFLOAT2 cameraTranslation;
	cameraTranslation.x = m_camera.GetViewMatrix()._41;
	cameraTranslation.y = m_camera.GetViewMatrix()._42;
		
	XMStoreFloat2(&cursorPos, XMLoadFloat2(&cursorPos) - XMLoadFloat2(&cameraTranslation));

	// Update the cursor position in grid fields within the app data structure
	XMFLOAT2 gridPosition;
	m_testEnvironment.WorldToGridPosition(cursorPos, gridPosition);
	m_appData.m_cursorGridPosX = static_cast<int>(gridPosition.x);
	m_appData.m_cursorGridPosY = static_cast<int>(gridPosition.y);

	if(m_appData.m_applicationState == EditMode)
	{
		if(m_inputManager.GetLeftClick())
		{
			// Add a new entity to the test environment
			m_testEnvironment.AddObject(m_appData.m_selectedObjectType, cursorPos, m_appData.m_currentRotation);
		}

		if(m_inputManager.GetNextEntityType())
		{
			m_appData.m_selectedObjectType = ObjectType(m_appData.m_selectedObjectType + 1);
			if(m_appData.m_selectedObjectType > BlueAttackPositionType)
			{
				m_appData.m_selectedObjectType = ObjectType(0);
			}
		}else if(m_inputManager.GetPreviousEntityType())
		{
			m_appData.m_selectedObjectType = ObjectType(m_appData.m_selectedObjectType - 1);
			if(m_appData.m_selectedObjectType < 0)
			{
				m_appData.m_selectedObjectType = ObjectType(BlueAttackPositionType);
			}
		}

		if(m_inputManager.GetRotateLeft())
		{
			m_appData.m_currentRotation -= 90.0f;
			if(m_appData.m_currentRotation < 0.0f)
			{
				m_appData.m_currentRotation = 270.0f;
			}
		}else if(m_inputManager.GetRotateRight())
		{
			m_appData.m_currentRotation += 90.0f;
			if(m_appData.m_currentRotation >= 360.0f)
			{
				m_appData.m_currentRotation = 0.0f;
			}
		}

		if(m_inputManager.GetSave())
		{
			SaveTestEnvironment();
		}

		if(m_inputManager.GetLoad())
		{
			LoadTestEnvironment();
		}

		if(m_inputManager.GetDelete())
		{
			m_testEnvironment.RemoveObjects(cursorPos);
		}
	}
}

//--------------------------------------------------------------------------------------
// Save the current test environment to a file specified by the user.
// Returns true if the saving process was successful.
//--------------------------------------------------------------------------------------
bool Application::SaveTestEnvironment(void)
{
	// Note: The whole application will close when the allocated console is closed manually by the user.

	// Allocate a console to allow user input
	FILE *stream;
	AllocConsole();
	freopen_s(&stream, "CONIN$", "r+t", stdin);
	freopen_s(&stream, "CONOUT$", "w+t", stdout);
	freopen_s(&stream, "CONOUT$", "w+t", stderr);

	if(m_pHandlerRoutine)
	{
		SetConsoleCtrlHandler(m_pHandlerRoutine, TRUE);
	}

	std::cout << "Please enter the name of the file, to which the current test environemnt should be saved."
			  << "\nEnter 'cancel' to cancel the saving process."
			  << "\nDon't forget to append the ending '.txt' to the filename." 
			  << "\nPress enter after providing a name to proceed.\n";

	// Note: Validation of the filename and error handling should be added.

	// Get a filename from the user or cancel the operation
	std::string filename;
	std::cin >> filename;
	if(filename != "cancel")
	{
		std::cout << "\nSaving...";
		m_testEnvironment.Save(filename);
		std::cout << "\nSaving completed.";
	}

	if(m_pHandlerRoutine)
	{
		SetConsoleCtrlHandler(m_pHandlerRoutine, FALSE);
	}

	// Saving operation finished, get rid of console
	FreeConsole();

	return true;
}


//--------------------------------------------------------------------------------------
// Load a test environment from a file specified by the user.
// Returns true if the loading process was successful.
//--------------------------------------------------------------------------------------
bool Application::LoadTestEnvironment(void)
{
	// Note: The whole application will close when the allocated console is closed manually by the user.

	// Allocate a console to allow user input 
	FILE *stream;
	AllocConsole();
	freopen_s(&stream, "CONIN$", "r+t", stdin);
	freopen_s(&stream, "CONOUT$", "w+t", stdout);
	freopen_s(&stream, "CONOUT$", "w+t", stderr);

	if(m_pHandlerRoutine)
	{
		SetConsoleCtrlHandler(m_pHandlerRoutine, TRUE);
	}

	std::cout << "Please enter the name of a file containing a test environment."
			  << "\nDon't forget to append the ending '.txt' to the filename."
			  << "\nEnter 'cancel' to cancel the loading process."
			  << "\nPress enter after providing a name to proceed.\n";

	// Note: Validation of the filename and error handling should be added.

	// Get a filename from the user or cancel the operation
	std::string filename;
	std::cin >> filename;
	if(filename != "cancel")
	{
		std::cout << "\nLoading...";
		m_testEnvironment.Load(filename);		
		std::cout << "\nLoading completed.";

		// Prepare the renderer for the loaded environment
		if(!m_renderer.SetupGrid(m_testEnvironment.GetGridSize(), m_testEnvironment.GetNumberOfGridPartitions()))
		{
			return false;
		}
	}

	if(m_pHandlerRoutine)
	{
		SetConsoleCtrlHandler(m_pHandlerRoutine, FALSE);
	}

	// Loading operation finished, get rid of console
	FreeConsole();

	return true;
}

//--------------------------------------------------------------------------------------
// Create a test environment based on user input.
// Returns true if the new test environment could be created successfully.
//--------------------------------------------------------------------------------------
bool Application::CreateNewTestEnvironment(void)
{
	// Note: The whole application will close when the allocated console is closed manually by the user.

	// Allocate a console to allow user input
	FILE *stream;
	AllocConsole();
	freopen_s(&stream, "CONIN$", "r+t", stdin);
	freopen_s(&stream, "CONOUT$", "w+t", stdout);
	freopen_s(&stream, "CONOUT$", "w+t", stderr);

	if(m_pHandlerRoutine)
	{
		SetConsoleCtrlHandler(m_pHandlerRoutine, TRUE);
	}

	std::cout << "Please enter the size and number of partitions for the new grid separated by a space."
			  << "\nEnter 'cancel' to cancel the process."
			  << "\nPress enter after providing a name to proceed.\n";

	// Note: Validation of the filename and error handling should be added.

	// Get the input from the user or cancel the operation

	std::string input;
	getline(std::cin, input);

	if(input != "cancel")
	{
		float        gridSize = 0.0f;
		unsigned int partitions = 0;

		std::istringstream iss(input);
		iss >> gridSize >> partitions;
		
		std::cout << "\nCreating new environment...";

		if((m_testEnvironment.Initialise(gridSize, partitions)) && m_renderer.SetupGrid(m_testEnvironment.GetGridSize(), m_testEnvironment.GetNumberOfGridPartitions()))
		{
			std::cout << "\nNew environment completed.";
		}else
		{
			std::cout << "\nAn error occurred.";
			return false;
		}
	}

	if(m_pHandlerRoutine)
	{
		SetConsoleCtrlHandler(m_pHandlerRoutine, FALSE);
	}

	// Saving operation finished, get rid of console
	FreeConsole();

	return true;
}
