/* 
*  Kevin Meergans, SquadAI, 2014
*  InputManager.h
*  Functions as a middle layer between the actual input class 
*  and the application.
*/

#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H

// Includes
#include <DirectXMath.h>
#include "Input.h"

using namespace DirectX;

class InputManager
{
public:
	InputManager(void);
	~InputManager(void);

	bool Initialise(HINSTANCE hInst, HWND hWnd);
	void Update(void);
	void Cleanup(void);

	const XMFLOAT3& GetCameraMovement(void) const;
	const POINT&    GetCursorPosition(void) const;
	bool            GetLeftClick(void) const;
	bool            GetBrowseLeft(void) const;
	bool            GetBrowseRight(void) const;
	bool            GetSave(void) const;
	bool            GetLoad(void) const;

private:
	HWND     m_windowHandle;   // A handle to the window the application is running in
	Input    m_input;          // The input object that is queried for key and mouse states
	XMFLOAT3 m_cameraMovement; // The camera movement for the current frame
	POINT    m_cursorPosition; // The position of the cursor within the client area in screen coordinates
};

#endif // INPUT_MANAGER_H
