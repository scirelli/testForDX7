//-----------------------------------------------------------------------------
// File: SCTimer.h
//
// Desc: Timer class, for animations
//
//-----------------------------------------------------------------------------

////INCLUDES///////////////////////
#include <windows.h>
//#include <windowsx.h>

#ifndef SCTimer_h
#define SCTimer_h

//------SCTIMER CLASS---------
class CTIMER
{
private:
	DWORD m_dwTimeStartedTimer;

public:
	CTIMER();
	DWORD GetCurrentTime();
	DWORD StartTimer();
	DWORD GetStartTime();
	int   GetStartElapsedTime();
	bool  Elapsed(DWORD &start, int interval); //has time elapsed since start
	bool  Elapsed(int interval); //has time elapsed since start
};

class CTIMER2
{
private:
	LARGE_INTEGER m_StartTime;
	LARGE_INTEGER m_EndTime;
	LARGE_INTEGER m_PerfFrequency;
public:
	CTIMER2();
	int StartTimer();
	float EndTimerAndRestart();
	float Elapsed(LARGE_INTEGER);

	LARGE_INTEGER GetStartTime();
};
#endif