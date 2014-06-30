/* 
*  Kevin Meergans, SquadAI, 2014
*  InputManager.h
*  Functions as a middle layer between the actual input class 
*  and the application.
*/

// Includes
#include "InputManager.h"

InputManager::InputManager(void) : m_cameraMovement(0.0f, 0.0f, 0.0f)					
{
}

InputManager::~InputManager(void)						
{
	Cleanup();
}

//--------------------------------------------------------------------------------------
// Initialise the input component.
// Param1: A handle to the instance.
// Param2: A handle to the window.
// Returns true if input was set up successfully, false otherwise.
//--------------------------------------------------------------------------------------
bool InputManager::Initialise(HINSTANCE hInst, HWND hWnd)
{
	m_windowHandle = hWnd;
	return m_input.Initialise(hInst, hWnd);
}

//--------------------------------------------------------------------------------------
// Update the keyboard and mouse state.
//--------------------------------------------------------------------------------------
void InputManager::Update()
{
	m_input.Update();
	
	// Update camera movement
	if(m_input.MiddleMouseButtonPressed())
	{
		m_cameraMovement = m_input.GetMouseMovement();
	}else
	{
		m_cameraMovement = XMFLOAT3(0.0f, 0.0f, m_input.GetMouseMovement().z);
	}

	// Update cursor position
	LPPOINT pCursorPosition = &m_cursorPosition;
	GetCursorPos(pCursorPosition);
	
	// Convert screen coordinates to client area coordinates
	ScreenToClient(m_windowHandle, pCursorPosition);
}

//--------------------------------------------------------------------------------------
// Cleanup the input component, free ressources.
//--------------------------------------------------------------------------------------
void InputManager::Cleanup(void)
{
	m_input.Cleanup();
}

// Data access functions

//--------------------------------------------------------------------------------------
// Returns a vector containing the camera movement along each axis for the current frame.
//--------------------------------------------------------------------------------------
const XMFLOAT3& InputManager::GetCameraMovement(void) const
{
	return m_cameraMovement;
}

//--------------------------------------------------------------------------------------
// Returns the cursor position within the client area.
//--------------------------------------------------------------------------------------
const POINT& InputManager::GetCursorPosition(void) const
{
	return m_cursorPosition;
}

//--------------------------------------------------------------------------------------
// Returns true if the user performed a left mouse click during the current frame.
//--------------------------------------------------------------------------------------
bool InputManager::GetLeftClick(void) const
{
	return m_input.LeftMouseButtonReleased();
}

//--------------------------------------------------------------------------------------
// Returns true if the user issued input to browse left in a list of options.
//--------------------------------------------------------------------------------------
bool InputManager::GetBrowseLeft(void) const
{
	return m_input.KeyLeftDown();
}

//--------------------------------------------------------------------------------------
// Returns true if the user issued input to browse right in a list of options.
//--------------------------------------------------------------------------------------
bool InputManager::GetBrowseRight(void) const
{
	return m_input.KeyRightDown();
}