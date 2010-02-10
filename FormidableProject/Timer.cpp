#include ".\timer.h"

CTimer::CTimer(void)
{
	// This function tells us how many ticks occur in one second
	QueryPerformanceFrequency((LARGE_INTEGER*)&m_ticksPerSecond);

	// Initialize our variables
	m_currentTime = m_lastTime = 0;
	m_lastFPSUpdate = 3;
	m_numFrames = 0;
	m_runningTime = m_timeElapsed = m_FPS = 0.0f;
	m_FPSUpdateInterval = m_ticksPerSecond >> 1;
	m_timerStopped = true;
}

CTimer::~CTimer(void)
{
}

void CTimer::Start(void)
{
	// If the timer has already been started, no need to re-start it
	if(!m_timerStopped)
		return;

	// Set lastTime to when our timer is started
	QueryPerformanceCounter( (LARGE_INTEGER*) &m_lastTime);

	// Our timer is not longer stopped
	m_timerStopped = false;
}

void CTimer::Stop(void)
{
	// If the timer is stopped, no need to stop it again
	if(m_timerStopped)
		return;

	// Get the current time so that we can add it to our running time
	INT64 stopTime = 0;
	QueryPerformanceCounter((LARGE_INTEGER*)&stopTime);
	m_runningTime += (float)(stopTime - m_lastTime) / (float)m_ticksPerSecond;

	// Our timer is now stopped
	m_timerStopped = true;
}

void CTimer::Update(void)
{
	// If timer is stopped, no need to update it
	if(m_timerStopped)
		return;

	// Get current time
	QueryPerformanceCounter((LARGE_INTEGER*)&m_currentTime);

	// (Current time - Previous time) / ticksPerSecond ...
	// is how much time has passed since this function was last called
	m_timeElapsed = (float)(m_currentTime - m_lastTime) / (float)m_ticksPerSecond;
	
	// Add the elapsed time to running time
	m_runningTime += m_timeElapsed;

	// Update FPS
	m_numFrames++;

	// Update FPS is 
	if(m_currentTime - m_lastFPSUpdate >= m_FPSUpdateInterval)
	{
		// Convert the current and last time into seconds
		float currentTime = (float)m_currentTime / (float)m_ticksPerSecond;
		float lastTime = (float)m_lastFPSUpdate / (float) m_ticksPerSecond;
		
		// Calculate FPS
		// How many times has Update() ran before reaching the FPSUpdateInterval value
		m_FPS = (float)m_numFrames / (currentTime - lastTime);

		// Store the time that the last FPS update occured
		m_lastFPSUpdate = m_currentTime;

		// Reset the FPS count
		m_numFrames = 0;
	}

	// Set current time to last time, for the next time update occurs
	m_lastTime = m_currentTime;
}
