//-----------------------------------------------------------------------------
// File: SCTimer.h
//
// Desc: Timer class, for animations
//
//-----------------------------------------------------------------------------

////INCLUDES///////////////////////
#include <windows.h>
#include <math.h>
#include <stdlib.h>
#include <tchar.h>
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
	float Elapsed();

	LARGE_INTEGER GetStartTime();
};

//-----------------------------------------------------------------------------
// Definitions, Macros & Constants
//-----------------------------------------------------------------------------
const ULONG MAX_SAMPLE_COUNT = 50; // Maximum frame time sample count

//-----------------------------------------------------------------------------
// Name : CTIMER3 (Class)
// Desc : Game Timer class, queries performance hardware if available, and 
//        calculates all the various values required for frame rate based
//        vector / value scaling.
//-----------------------------------------------------------------------------
class CTIMER3
{
public:
    //-------------------------------------------------------------------------
    // Constructors & Destructors for This Class
    //-------------------------------------------------------------------------
	         CTIMER3();
	virtual ~CTIMER3();

	//------------------------------------------------------------
	// Public Functions For This Class
	//------------------------------------------------------------
	void	        Tick( float fLockFPS = 0.0f );
    unsigned long   GetFrameRate( LPTSTR lpszString = NULL ) const;
    float           GetTimeElapsed() const;

private:
	//------------------------------------------------------------
	// Private Variables For This Class
	//------------------------------------------------------------
    bool            m_PerfHardware;             // Has Performance Counter
	double          m_TimeScale;                // Amount to scale counter
	float           m_TimeElapsed;              // Time elapsed since previous frame
    __int64         m_CurrentTime;              // Current Performance Counter
    __int64         m_LastTime;                 // Performance Counter last frame
	__int64         m_PerfFreq;                 // Performance Frequency

    float           m_FrameTime[MAX_SAMPLE_COUNT];
    ULONG           m_SampleCount;

    unsigned long   m_FrameRate;                // Stores current framerate
	unsigned long   m_FPSFrameCount;            // Elapsed frames in any given second
	float           m_FPSTimeElapsed;           // How much time has passed during FPS sample
	
	//------------------------------------------------------------
	// Private Functions For This Class
	//------------------------------------------------------------
};

#endif