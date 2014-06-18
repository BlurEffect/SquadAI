/* 
*  Kevin Meergans, SquadAI, 2014
*  InputManager.h
*  Functions as a middle layer between the actual input class 
*  and the application.
*/

#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H

// Includes
#include "Input.h"

class InputManager
{
public:
	InputManager(void);
	~InputManager(void);

	bool Initialise(HINSTANCE hInst, HWND hWnd);
	void Update(void);
	void Cleanup(void);
private:
	Input m_input; // The input object that is queried for key and mouse states
};

#endif // INPUT_MANAGER_H
