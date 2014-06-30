/* 
*  Kevin Meergans, SquadAI, 2014
*  Input.h
*  Detects user input relying on Direct Input.
*/

// Note: Might be a candidate for implementation as a Singleton.

#ifndef INPUT_H
#define INPUT_H

#define DIRECTINPUT_VERSION 0x0800 // Define direct input version

// Includes
#include <windows.h>	
#include <dinput.h>	  
#include <DirectXMath.h>

using namespace DirectX;

class Input
{
public:
	Input(void);
	~Input(void);

	bool Initialise(HINSTANCE hInst, HWND hWnd);
	void Update(void);
	void Cleanup(void);

	const XMFLOAT3& GetMouseMovement(void) const;

	bool LeftMouseButtonDown(void) const;
	bool RightMouseButtonDown(void) const;
	bool MiddleMouseButtonDown(void) const;

	bool LeftMouseButtonPressed(void) const;
	bool RightMouseButtonPressed(void) const;
	bool MiddleMouseButtonPressed(void) const;

	bool LeftMouseButtonReleased(void) const;
	bool RightMouseButtonReleased(void) const;
	bool MiddleMouseButtonReleased(void) const;

	bool KeyLeftDown(void) const;
	bool KeyRightDown(void) const;
	bool KeyUpDown(void) const;
	bool KeyDownDown(void) const;

	bool KeyLeftPressed(void) const;
	bool KeyRightPressed(void) const;
	bool KeyUpPressed(void) const;
	bool KeyDownPressed(void) const;

	bool KeyLeftReleased(void) const;
	bool KeyRightReleased(void) const;
	bool KeyUpReleased(void) const;
	bool KeyDownReleased(void) const;

private:
	bool SetupDirectInput(HINSTANCE hInst, HWND hWnd);
	void UpdateMouseInput(void);
	void UpdateKeyboardInput(void);
	void CleanupDirectInput(void);

	LPDIRECTINPUT8		 m_pDI;				  // Pointer to the Direct Input object
	LPDIRECTINPUTDEVICE8 m_pDIMouseDevice;	  // The Direct Input mouse device
	LPDIRECTINPUTDEVICE8 m_pDIKeyboardDevice; // The Direct Input keyboard device

	DIMOUSESTATE2 m_oldMouseState;			// The mouse state during the previous update
	DIMOUSESTATE2 m_mouseState;				// The current mouse state
	char		  m_oldKeyboardState[256];  // The keyboard state of the previous update
	char		  m_keyboardState[256];		// The current keyboard state

	XMFLOAT3	  m_mouseMovement; // Describes the mouse movement along x and y axis as well as mouse wheel movement
};

#endif // INPUT_H