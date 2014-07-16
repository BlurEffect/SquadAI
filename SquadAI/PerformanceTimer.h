/* 
*  Kevin Meergans, SquadAI, 2014
*  PerformanceTimer.h
*  Provides functions for measurement of the delta time and framerate.
*/

#ifndef PERFORMANCE_TIMER_H
#define PERFORMANCE_TIMER_H

// Includes
#include <windows.h>
#include <mmsystem.h>

class PerformanceTimer
{
public:
	PerformanceTimer(void);
	~PerformanceTimer(void);

	void Initialise(void);
	void Update(void);

	// Data access functions

	unsigned int GetFPS(void) const;
	float        GetDeltaTime(void) const;

private:
	unsigned int  m_frameCount;        // The number of frames for the current second
	unsigned int  m_framesPerSecond;   // The current framerate
	unsigned long m_frameStartTime;	   // The time since the last frame, in seconds
	unsigned long m_startTimeFPSCount; // The time since the beginning of the last second
	float         m_deltaTime;         // The time since the last frame/update
};

#endif // PERFORMANCE_TIMER_H