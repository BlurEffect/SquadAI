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
	initData.m_gridHorizontalPartitions = 20;
	initData.m_gridVerticalPartitions   = 20;

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
	m_renderer.RenderScene(m_camera.GetViewMatrix(), m_camera.GetProjectionMatrix(), m_appData);
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
	if(m_inputManager.GetNextEntityType())
	{
		m_appData.m_selectedEntityType = EntityType(m_appData.m_selectedEntityType + 1);
		if(m_appData.m_selectedEntityType > CoverSpot)
		{
			m_appData.m_selectedEntityType = EntityType(0);
		}
	}else if(m_inputManager.GetPreviousEntityType())
	{
		m_appData.m_selectedEntityType = EntityType(m_appData.m_selectedEntityType - 1);
		if(m_appData.m_selectedEntityType < 0)
		{
			m_appData.m_selectedEntityType = EntityType(CoverSpot);
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

	// Translate the cursor position from screen space to world space

	XMFLOAT3 cursorPos(static_cast<float>(m_inputManager.GetCursorPosition().x), static_cast<float>(m_inputManager.GetCursorPosition().y), 0.0f);

	cursorPos.x -= m_appData.m_windowWidth/2;
	cursorPos.y  = -cursorPos.y + m_appData.m_windowHeight/2;
	cursorPos.z  = 0.0f;

	XMStoreFloat3(&cursorPos, XMLoadFloat3(&cursorPos) * m_camera.m_zoomFactor);

	XMFLOAT4X4 inverseViewMatrix;
	XMVECTOR determinant;
	XMStoreFloat4x4(&inverseViewMatrix, XMMatrixInverse(&determinant, XMLoadFloat4x4(&m_camera.GetViewMatrix())));

	XMFLOAT3 cameraTranslation;
	cameraTranslation.x = m_camera.GetViewMatrix()._41;
	cameraTranslation.y = m_camera.GetViewMatrix()._42;
	cameraTranslation.z = 0.0f;
		
	XMStoreFloat3(&cursorPos, XMLoadFloat3(&cursorPos) - XMLoadFloat3(&cameraTranslation));

	if(m_inputManager.GetLeftClick())
	{
		// Add a new entity to the test environment
		m_testEnvironment.AddEntity(m_appData.m_selectedEntityType, cursorPos, m_appData.m_currentRotation);
	}

	// Update the cursor position in grid fields within the app data structure
	XMFLOAT3 gridPosition;
	m_testEnvironment.GetGridPosition(cursorPos, gridPosition);
	m_appData.m_cursorGridPosX = static_cast<int>(gridPosition.x);
	m_appData.m_cursorGridPosY = static_cast<int>(gridPosition.y);

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
		m_testEnvironment.RemoveEntity(cursorPos);
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
		if(!m_renderer.SetupGrid(m_testEnvironment.GetData()))
		{
			return false;
		}
	}

	// Loading operation finished, get rid of console
	FreeConsole();

	return true;
}