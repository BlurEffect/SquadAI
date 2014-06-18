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

class Input
{
public:
	Input(void);
	~Input(void);

	bool Initialise(HINSTANCE hInst, HWND hWnd);
	void Update(void);
	void Cleanup(void);

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
};

#endif // INPUT_H