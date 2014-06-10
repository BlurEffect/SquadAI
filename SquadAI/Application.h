/* 
*  Kevin Meergans, SquadAI, 2014
*  Application.h
*  Central class to the application. Initialises and updates components and coordinates
*  the communication between them.
*/

#ifndef APPLICATION_H
#define APPLICATION_H

class Application
{
public:
	bool Initialise();
	bool Update();
	void Cleanup();
};

#endif // APPLICATION_H