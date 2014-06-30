/* 
*  Kevin Meergans, SquadAI, 2014
*  Input.cpp
*  Detects user input relying on Direct Input.
*/

// Includes
#include "Input.h"

// Defines
#define KEYDOWN(name, key) (name[key] & 0x80) 

Input::Input(void) : m_pDI(nullptr),
					 m_pDIMouseDevice(nullptr),
					 m_pDIKeyboardDevice(nullptr)							
{
	ZeroMemory(&m_oldMouseState, sizeof(m_oldMouseState));
	ZeroMemory(&m_mouseState, sizeof(m_mouseState));
	ZeroMemory(&m_oldKeyboardState, sizeof(m_oldKeyboardState));
	ZeroMemory(&m_keyboardState, sizeof(m_keyboardState));
}

Input::~Input(void)
{
	Cleanup();
}

//--------------------------------------------------------------------------------------
// Initialise the input component.
// Param1: A handle to the instance.
// Param2: A handle to the window.
// Returns true if input was set up successfully, false otherwise.
//--------------------------------------------------------------------------------------
bool Input::Initialise(HINSTANCE hInst, HWND hWnd)
{
	return SetupDirectInput(hInst, hWnd);
}

//--------------------------------------------------------------------------------------
// Update the keyboard and mouse state.
//--------------------------------------------------------------------------------------
void Input::Update()
{
	UpdateKeyboardInput();
	UpdateMouseInput();
}

//--------------------------------------------------------------------------------------
// Cleanup the input component, free ressources.
//--------------------------------------------------------------------------------------
void Input::Cleanup(void)
{
	CleanupDirectInput();
}

//--------------------------------------------------------------------------------------
// Initialise and set up direct input
// Param1: A handle to the instance.
// Param2: A handle to the window.
// Returns true if direct input was set up successfully, false otherwise.
//--------------------------------------------------------------------------------------
bool Input::SetupDirectInput(HINSTANCE hInst, HWND hWnd)
{ 
    // Create the DirectInput object. 
	if(FAILED(DirectInput8Create(hInst, DIRECTINPUT_VERSION, IID_IDirectInput8, reinterpret_cast<void**>(&m_pDI), NULL)))
	{
		return false;
	}

	// Initialise the mouse device

	// Retrieve a pointer to an IDirectInputDevice8 interface for the mouse.
	if(FAILED(m_pDI->CreateDevice(GUID_SysMouse, &m_pDIMouseDevice, NULL)))
	{
		return false;
	}

    // Set the data format using the predefined mouse data format.
	if(FAILED(m_pDIMouseDevice->SetDataFormat(&c_dfDIMouse2)))
	{
		return false;
	}

    // Set the cooperative level 
	if(FAILED(m_pDIMouseDevice->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND)))
	{
		return false;
	}

	// Get the input's device state and store it in 'MouseState'
	ZeroMemory(&m_mouseState, sizeof(m_mouseState));

	if(FAILED(m_pDIMouseDevice->GetDeviceState(sizeof(m_mouseState), &m_mouseState)))
	{
		// If input is lost, try to reacquire it
		HRESULT hr = m_pDIMouseDevice->Acquire();

		while(hr == DIERR_INPUTLOST) 
		{
			hr = m_pDIMouseDevice->Acquire();
		}

		if(FAILED(m_pDIMouseDevice->GetDeviceState(sizeof(m_mouseState), &m_mouseState)))
		{
			return false;
		}
	}

	// Initialise the keyboard device

    // Retrieve a pointer to an IDirectInputDevice8 interface
	if(FAILED(m_pDI->CreateDevice(GUID_SysKeyboard, &m_pDIKeyboardDevice, NULL)))
	{
		return false;
	}

    // Set the data format using the predefined mouse data format.
    if(FAILED(m_pDIKeyboardDevice->SetDataFormat(&c_dfDIKeyboard)))
	{
		return false;
	}

    // Set the cooperative level 
	if(FAILED(m_pDIKeyboardDevice->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE)))
	{
		return false;
	}

	// Initialise state of keyboard
	ZeroMemory(m_keyboardState, 256);
	if(FAILED(m_pDIKeyboardDevice -> GetDeviceState(sizeof(m_keyboardState), reinterpret_cast<LPVOID>(&m_keyboardState))))
	{
		// Device might have been lost, try to reacquire it
		HRESULT hr = m_pDIKeyboardDevice->Acquire();

        while (hr == DIERR_INPUTLOST)
		{
			hr = m_pDIKeyboardDevice->Acquire();
		}

		if(FAILED(m_pDIKeyboardDevice->GetDeviceState(sizeof(m_keyboardState), reinterpret_cast<LPVOID>(&m_keyboardState))))
		{
			return false;
		}
	}

    return true; 
}

//--------------------------------------------------------------------------------------
// Detects mouse button presses and movements.
//--------------------------------------------------------------------------------------
void Input::UpdateMouseInput()
{
    if (m_pDIMouseDevice)
	{
		// Remember the old mouse state 
		memcpy(&m_oldMouseState, &m_mouseState, sizeof(m_oldMouseState));

		// Get the new mouse state
	    ZeroMemory(&m_mouseState, sizeof(m_mouseState));
		if(FAILED(m_pDIMouseDevice->GetDeviceState(sizeof(m_mouseState), &m_mouseState)))
		{
			// If input is lost, try to reacquire it
			HRESULT hr = m_pDIMouseDevice->Acquire();

			while(hr == DIERR_INPUTLOST) 
			{
				hr = m_pDIMouseDevice->Acquire();
			}

			m_pDIMouseDevice->GetDeviceState(sizeof(m_mouseState), &m_mouseState);
		}
	}

	// Update the mouse movement vector
	m_mouseMovement.x = static_cast<float>(m_mouseState.lX);
	m_mouseMovement.y = static_cast<float>(m_mouseState.lY);
	m_mouseMovement.z = static_cast<float>(m_mouseState.lZ);
}

//--------------------------------------------------------------------------------------
// Detects key presses on the keyboard.
//--------------------------------------------------------------------------------------
void Input::UpdateKeyboardInput() 
{ 
	if (m_pDIKeyboardDevice)
	{
		// Remember the old keyboard state 
		memcpy(m_oldKeyboardState, m_keyboardState, sizeof(m_oldKeyboardState));

		// Get the new keyboard state
		ZeroMemory(m_keyboardState, 256);
		if(FAILED(m_pDIKeyboardDevice -> GetDeviceState(sizeof(m_keyboardState), reinterpret_cast<LPVOID>(&m_keyboardState))))
		{
			// Device might have been lost, try to reacquire it
			HRESULT hr = m_pDIKeyboardDevice->Acquire();

			while (hr == DIERR_INPUTLOST)
			{
				hr = m_pDIKeyboardDevice->Acquire();
			}

			m_pDIKeyboardDevice->GetDeviceState(sizeof(m_keyboardState), reinterpret_cast<LPVOID>(&m_keyboardState));
		}
	}
}

//--------------------------------------------------------------------------------------
// Cleanup direct input and free all associated ressources.
//--------------------------------------------------------------------------------------
void Input::CleanupDirectInput() 
{ 
    if (m_pDI) 
    { 
        // Release the mouse
		if (m_pDIMouseDevice) 
        { 
			// Unacquire device before releasing it
            m_pDIMouseDevice->Unacquire(); 
            m_pDIMouseDevice->Release();
            m_pDIMouseDevice = nullptr; 
        } 
        
		// Release the keyboard
		if (m_pDIKeyboardDevice) 
        { 
			// Unacquire device before releasing it 
            m_pDIKeyboardDevice->Unacquire(); 
            m_pDIKeyboardDevice->Release();
            m_pDIKeyboardDevice = nullptr; 
        }
		
		// Release the Direct Input Object
		m_pDI->Release();
        m_pDI = nullptr;
    } 
} 

// Access mouse input

//--------------------------------------------------------------------------------------
// Returns the current mouse movement consisting of the movement of the mouse detected 
// along x and y axis as well as the movement of the mouse wheel as the z-component of the
// returned vector.
//--------------------------------------------------------------------------------------
const XMFLOAT3& Input::GetMouseMovement(void) const
{
	return m_mouseMovement;
}

//--------------------------------------------------------------------------------------
// Returns true if the left mouse button was pushed down in this frame.
//--------------------------------------------------------------------------------------
bool Input::LeftMouseButtonDown(void) const
{
	return (m_mouseState.rgbButtons[0] & 0x80) && (!(m_oldMouseState.rgbButtons[0] & 0x80)); 
}

//--------------------------------------------------------------------------------------
// Returns true if the right mouse button was pushed down in this frame.
//--------------------------------------------------------------------------------------
bool Input::RightMouseButtonDown(void) const
{
	return (m_mouseState.rgbButtons[1] & 0x80) && (!(m_oldMouseState.rgbButtons[1] & 0x80));
}

//--------------------------------------------------------------------------------------
// Returns true if the middle mouse button was pushed down in this frame.
//--------------------------------------------------------------------------------------
bool Input::MiddleMouseButtonDown(void) const
{
	return (m_mouseState.rgbButtons[2] & 0x80) && (!(m_oldMouseState.rgbButtons[2] & 0x80));
}

//--------------------------------------------------------------------------------------
// Returns true if the left mouse button is pushed down.
//--------------------------------------------------------------------------------------
bool Input::LeftMouseButtonPressed(void) const
{
	return (m_mouseState.rgbButtons[0] & 0x80);
}

//--------------------------------------------------------------------------------------
// Returns true if the right mouse button is pushed down.
//--------------------------------------------------------------------------------------
bool Input::RightMouseButtonPressed(void) const
{
	return (m_mouseState.rgbButtons[1] & 0x80);
}

//--------------------------------------------------------------------------------------
// Returns true if the middle mouse button is pushed down.
//--------------------------------------------------------------------------------------
bool Input::MiddleMouseButtonPressed(void) const
{
	return (m_mouseState.rgbButtons[2] & 0x80);
}

//--------------------------------------------------------------------------------------
// Returns true if the left mouse button was released in this frame.
//--------------------------------------------------------------------------------------
bool Input::LeftMouseButtonReleased(void) const
{
	return (!(m_mouseState.rgbButtons[0] & 0x80)) && (m_oldMouseState.rgbButtons[0] & 0x80);
}

//--------------------------------------------------------------------------------------
// Returns true if the right mouse button was released in this frame.
//--------------------------------------------------------------------------------------
bool Input::RightMouseButtonReleased(void) const
{
	return (!(m_mouseState.rgbButtons[1] & 0x80)) && (m_oldMouseState.rgbButtons[1] & 0x80);
}

//--------------------------------------------------------------------------------------
// Returns true if the middle mouse button was released in this frame.
//--------------------------------------------------------------------------------------
bool Input::MiddleMouseButtonReleased(void) const
{
	return (!(m_mouseState.rgbButtons[2] & 0x80)) && (m_oldMouseState.rgbButtons[2] & 0x80);
}

//--------------------------------------------------------------------------------------
// Returns true if the left arrow key was pushed down in this frame.
//--------------------------------------------------------------------------------------
bool Input::KeyLeftDown(void) const
{
	return (KEYDOWN(m_keyboardState, DIK_LEFT)) && (!KEYDOWN(m_oldKeyboardState, DIK_LEFT));
}

//--------------------------------------------------------------------------------------
// Returns true if the right arrow key was pushed down in this frame.
//--------------------------------------------------------------------------------------
bool Input::KeyRightDown(void) const
{
	return (KEYDOWN(m_keyboardState, DIK_RIGHT)) && (!KEYDOWN(m_oldKeyboardState, DIK_RIGHT));
}

//--------------------------------------------------------------------------------------
// Returns true if the up arrow key was pushed down in this frame.
//--------------------------------------------------------------------------------------
bool Input::KeyUpDown(void) const
{
	return (KEYDOWN(m_keyboardState, DIK_UP)) && (!KEYDOWN(m_oldKeyboardState, DIK_UP));
}

//--------------------------------------------------------------------------------------
// Returns true if the down arrow key was pushed down in this frame.
//--------------------------------------------------------------------------------------
bool Input::KeyDownDown(void) const
{
	return (KEYDOWN(m_keyboardState, DIK_DOWN)) && (!KEYDOWN(m_oldKeyboardState, DIK_DOWN));
}

//--------------------------------------------------------------------------------------
// Returns true if the left arrow key is pushed down in this frame.
//--------------------------------------------------------------------------------------
bool Input::KeyLeftPressed(void) const
{
	return (KEYDOWN(m_keyboardState, DIK_LEFT));
}

//--------------------------------------------------------------------------------------
// Returns true if the right arrow key is pushed down in this frame.
//--------------------------------------------------------------------------------------
bool Input::KeyRightPressed(void) const
{
	return (KEYDOWN(m_keyboardState, DIK_RIGHT));
}

//--------------------------------------------------------------------------------------
// Returns true if the up arrow key is pushed down in this frame.
//--------------------------------------------------------------------------------------
bool Input::KeyUpPressed(void) const
{
	return (KEYDOWN(m_keyboardState, DIK_UP));
}

//--------------------------------------------------------------------------------------
// Returns true if the down arrow key is pushed down in this frame.
//--------------------------------------------------------------------------------------
bool Input::KeyDownPressed(void) const
{
	return (KEYDOWN(m_keyboardState, DIK_DOWN));
}

//--------------------------------------------------------------------------------------
// Returns true if the left arrow key was released in this frame.
//--------------------------------------------------------------------------------------
bool Input::KeyLeftReleased(void) const
{
	return (!KEYDOWN(m_keyboardState, DIK_LEFT)) && (KEYDOWN(m_oldKeyboardState, DIK_LEFT));
}

//--------------------------------------------------------------------------------------
// Returns true if the right arrow key was released in this frame.
//--------------------------------------------------------------------------------------
bool Input::KeyRightReleased(void) const
{
	return (!KEYDOWN(m_keyboardState, DIK_RIGHT)) && (KEYDOWN(m_oldKeyboardState, DIK_RIGHT));
}

//--------------------------------------------------------------------------------------
// Returns true if the up arrow key was released in this frame.
//--------------------------------------------------------------------------------------
bool Input::KeyUpReleased(void) const
{
	return (!KEYDOWN(m_keyboardState, DIK_UP)) && (KEYDOWN(m_oldKeyboardState, DIK_UP));
}

//--------------------------------------------------------------------------------------
// Returns true if the down arrow key was released in this frame.
//--------------------------------------------------------------------------------------
bool Input::KeyDownReleased(void) const
{
	return (!KEYDOWN(m_keyboardState, DIK_DOWN)) && (KEYDOWN(m_oldKeyboardState, DIK_DOWN));
}