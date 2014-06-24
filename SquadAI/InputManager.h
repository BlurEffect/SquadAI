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

private:
	Input    m_input;          // The input object that is queried for key and mouse states
	XMFLOAT3 m_cameraMovement; // The camera movement for the current frame
};

#endif // INPUT_MANAGER_H
