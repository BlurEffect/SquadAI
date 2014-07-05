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
// Returns true if the user issued input to select the next entity type available in the list.
//--------------------------------------------------------------------------------------
bool InputManager::GetNextEntityType(void) const
{
	return m_input.KeyDownDown();
}

//--------------------------------------------------------------------------------------
// Returns true if the user issued input to select the previous entity type available in the list.
//--------------------------------------------------------------------------------------
bool InputManager::GetPreviousEntityType(void) const
{
	return m_input.KeyUpDown();
}

//--------------------------------------------------------------------------------------
// Returns true if the user issued input to rotate the entities to be placed clockwise.
//--------------------------------------------------------------------------------------
bool InputManager::GetRotateRight(void) const
{
	return m_input.KeyRightDown();
}

//--------------------------------------------------------------------------------------
// Returns true if the user issued input to rotate the entities to be placed counter-clockwise.
//--------------------------------------------------------------------------------------
bool InputManager::GetRotateLeft(void) const
{
	return m_input.KeyLeftDown();
}

//--------------------------------------------------------------------------------------
// Returns true if the user issued input to save a test environment.
//--------------------------------------------------------------------------------------
bool InputManager::GetSave(void) const
{
	return m_input.KeyLeftCtrlPressed() && m_input.KeySDown();
}

//--------------------------------------------------------------------------------------
// Returns true if the user issued input to load a test environment.
//--------------------------------------------------------------------------------------
bool InputManager::GetLoad(void) const
{
	return m_input.KeyLeftCtrlPressed() && m_input.KeyLDown();
}

//--------------------------------------------------------------------------------------
// Returns true if the user issued input to delete an entity from the test environment.
//--------------------------------------------------------------------------------------
bool InputManager::GetDelete(void) const
{
	return m_input.KeyDeleteDown();
}