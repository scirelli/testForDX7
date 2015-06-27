//-----------------------------------------------------------------------------
// File: SCTimer.cpp
//
// Desc: Implemtation for the timer class all time is in milliseconds
//
//-----------------------------------------------------------------------------

////INCLUDES/////////////////
#include "SCTimer.h"


////FUNCTIONS///////////////

//-----------------------------------------------------------------------------
// Name: CTIMER
// Desc: Constructor: inits obj.
//-----------------------------------------------------------------------------
CTIMER::CTIMER()
{
	m_dwTimeStartedTimer = 0;
}

//-----------------------------------------------------------------------------
// Name: GetCurrentTime()
// Desc: Gets the current time in ms by calling GetTickCount()
//-----------------------------------------------------------------------------
DWORD CTIMER::GetCurrentTime()
{
	return timeGetTime();
}

//-----------------------------------------------------------------------------
// Name: StartTimer()
// Desc: Starts the timer. And stores the time timer was started
//-----------------------------------------------------------------------------
DWORD CTIMER::StartTimer()
{
	m_dwTimeStartedTimer = GetTickCount();
	return( m_dwTimeStartedTimer );
}

//-----------------------------------------------------------------------------
// Name: GetStartTimer()
// Desc: Gets the time when the timer was started
//-----------------------------------------------------------------------------
DWORD CTIMER::GetStartTime()
{
	return m_dwTimeStartedTimer;
}

//-----------------------------------------------------------------------------
// Name: GetStartElapsedTime()
// Desc: Gets the time that has elapsed since timer was started
//-----------------------------------------------------------------------------
int CTIMER::GetStartElapsedTime()
{
	return ( (int)(GetTickCount() - m_dwTimeStartedTimer) );
}

//-----------------------------------------------------------------------------
// Name: Elapsed()
// Desc: Has the time (interval) elapsed since start?
//-----------------------------------------------------------------------------
bool CTIMER::Elapsed(DWORD &start, int interval) //has time elapsed since start
{
	if( ( (int)(GetTickCount()) ) >= ( ((int)start)+interval) )
		return true;
	return false;
}

//-----------------------------------------------------------------------------
// Name: Elapsed()
// Desc: Has the time (interval) elapsed since m_dwTimeStartedTimer?
//-----------------------------------------------------------------------------
bool CTIMER::Elapsed(int interval) //has time elapsed since start
{
	if( ( (int)(GetTickCount()) ) >= ( ((int)m_dwTimeStartedTimer)+interval) )
		return true;
	return false;
}

//=====================================================CTIMER2 CLASS===========================================================================
//-----------------------------------------------------------------------------
// Name: 
// Desc: 
//-----------------------------------------------------------------------------
CTIMER2::CTIMER2()
{
	QueryPerformanceFrequency(&m_PerfFrequency);
}

//-----------------------------------------------------------------------------
// Name: StartTimer()
// Desc: Start the timer
//-----------------------------------------------------------------------------
int CTIMER2::StartTimer()
{
	QueryPerformanceFrequency(&m_PerfFrequency);
	QueryPerformanceCounter(&m_StartTime);
	return (1);
}

//-----------------------------------------------------------------------------
// Name: EndTimerAndRestart()
// Desc: Stops the timer and returns the diffrence end-start, and restarts the timer
//-----------------------------------------------------------------------------
float CTIMER2::EndTimerAndRestart()
{
	static float temp;
	QueryPerformanceCounter(&m_EndTime);
	temp = ( (float)m_EndTime.QuadPart - (float)m_StartTime.QuadPart ) / m_PerfFrequency.QuadPart;
	m_StartTime.QuadPart = m_EndTime.QuadPart;
	return (temp);
}

//-----------------------------------------------------------------------------
// Name: Elapsed()
// Desc: returns the elapsed time since the start time sent in elapsed
//-----------------------------------------------------------------------------
float CTIMER2::Elapsed(LARGE_INTEGER start)
{
	LARGE_INTEGER endtime;
	QueryPerformanceCounter(&endtime);
	return ( ( (float)endtime.QuadPart - (float)start.QuadPart ) / m_PerfFrequency.QuadPart );
}

//-----------------------------------------------------------------------------
// Name: GetStartTime()
// Desc: 
//-----------------------------------------------------------------------------
LARGE_INTEGER CTIMER2::GetStartTime()
{
	return m_StartTime;
}