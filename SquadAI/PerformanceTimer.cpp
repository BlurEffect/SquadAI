/* 
*  Kevin Meergans, SquadAI, 2014
*  PerformanceTimer.cpp
*  Provides functions for measurement of the delta time and framerate.
*/

// Includes
#include "PerformanceTimer.h"

PerformanceTimer::PerformanceTimer(void) : m_frameCount(0),
										   m_framesPerSecond(0),
										   m_frameStartTime(0),
										   m_startTimeFPSCount(0),
										   m_deltaTime(0.0f)
{
}

PerformanceTimer::~PerformanceTimer(void)
{
}

//--------------------------------------------------------------------------------------
// Initialises the performance timer.
//--------------------------------------------------------------------------------------
void PerformanceTimer::Initialise(void)
{
	// Set the current time as the start time
	m_frameStartTime    = timeGetTime();
	m_startTimeFPSCount = timeGetTime();
}

//--------------------------------------------------------------------------------------
// Updates the performance timer.
//--------------------------------------------------------------------------------------
void PerformanceTimer::Update(void)
{
	++m_frameCount;

	if(timeGetTime() >= (m_startTimeFPSCount + 1000))
	{
		// When one second has passed, set the framerate and reset the timer and count
		m_framesPerSecond = m_frameCount;
		
		m_frameCount = 0;
		m_startTimeFPSCount = timeGetTime();
	}

	// Get the time passed since the last frame in seconds
	m_deltaTime = (timeGetTime() - m_frameStartTime) * 0.001f;

	m_frameStartTime = timeGetTime();
}

// Data access functions

unsigned int PerformanceTimer::GetFPS(void) const
{
	return m_framesPerSecond;
}

float PerformanceTimer::GetDeltaTime(void) const
{
	return m_deltaTime;
}