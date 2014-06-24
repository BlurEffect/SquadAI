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
	return m_input.Initialise(hInst, hWnd);
}

//--------------------------------------------------------------------------------------
// Update the keyboard and mouse state.
//--------------------------------------------------------------------------------------
void InputManager::Update()
{
	m_input.Update();
	
	if(m_input.MiddleMouseButtonPressed())
	{
		m_cameraMovement = m_input.GetMouseMovement();
	}else
	{
		m_cameraMovement = XMFLOAT3(0.0f, 0.0f, m_input.GetMouseMovement().z);
	}
}

//--------------------------------------------------------------------------------------
// Cleanup the input component, free ressources.
//--------------------------------------------------------------------------------------
void InputManager::Cleanup(void)
{
	m_input.Cleanup();
}

// Data access functions

const XMFLOAT3& InputManager::GetCameraMovement(void) const
{
	return m_cameraMovement;
}