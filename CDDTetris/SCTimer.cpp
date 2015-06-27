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
// Name: Elapsed()
// Desc: returns the elapsed time since the m_StartTime time sent in elapsed
//-----------------------------------------------------------------------------
float CTIMER2::Elapsed()
{
	LARGE_INTEGER endtime;
	QueryPerformanceCounter(&endtime);
	return ( ( (float)endtime.QuadPart - (float)m_StartTime.QuadPart ) / m_PerfFrequency.QuadPart );
}

//-----------------------------------------------------------------------------
// Name: GetStartTime()
// Desc: 
//-----------------------------------------------------------------------------
LARGE_INTEGER CTIMER2::GetStartTime()
{
	return m_StartTime;
}

//==================================================== CTIMER3 ==============================================================================
//-----------------------------------------------------------------------------
// Name : CTimer () (Constructor)
// Desc : CTimer Class Constructor
//-----------------------------------------------------------------------------
CTIMER3::CTIMER3()
{
	// Query performance hardware and setup time scaling values
	if (QueryPerformanceFrequency((LARGE_INTEGER *)&m_PerfFreq)) 
    { 
		m_PerfHardware		= TRUE;
		QueryPerformanceCounter((LARGE_INTEGER *) &m_LastTime); 
		m_TimeScale			= 1.0f / m_PerfFreq;
	} 
    else 
    { 
		// no performance counter, read in using timeGetTime 
		m_PerfHardware		= FALSE;
		m_LastTime			= timeGetTime(); 
		m_TimeScale			= 0.001f;
	
    } // End If No Hardware

	// Clear any needed values
    m_SampleCount       = 0;
	m_FrameRate			= 0;
	m_FPSFrameCount		= 0;
	m_FPSTimeElapsed	= 0.0f;
}

//-----------------------------------------------------------------------------
// Name : CTIMER3 () (Destructor)
// Desc : CTIMER3 Class Destructor
//-----------------------------------------------------------------------------
CTIMER3::~CTIMER3()
{
}

//-----------------------------------------------------------------------------
// Name : Tick () 
// Desc : Function which signals that frame has advanced
// Note : You can specify a number of frames per second to lock the frame rate
//        to. This will simply soak up the remaining time to hit that target.
//-----------------------------------------------------------------------------
void CTIMER3::Tick( float fLockFPS )
{
    float fTimeElapsed; 

    // Is performance hardware available?
	if ( m_PerfHardware ) 
    {
        // Query high-resolution performance hardware
		QueryPerformanceCounter((LARGE_INTEGER *)&m_CurrentTime);
	} 
    else 
    {
        // Fall back to less accurate timer
		m_CurrentTime = timeGetTime();

	} // End If no hardware available

	// Calculate elapsed time in seconds
	fTimeElapsed = (m_CurrentTime - m_LastTime) * m_TimeScale;

    // Smoothly ramp up frame rate to prevent jittering
    //if ( fLockFPS == 0.0f ) fLockFPS = (1.0f / GetTimeElapsed()) + 20.0f;
    
    // Should we lock the frame rate ?
    if ( fLockFPS > 0.0f )
    {
        while ( fTimeElapsed < (1.0f / fLockFPS))
        {
            // Is performance hardware available?
	        if ( m_PerfHardware ) 
            {
                // Query high-resolution performance hardware
		        QueryPerformanceCounter((LARGE_INTEGER*)&m_CurrentTime);
	        } 
            else 
            {
                // Fall back to less accurate timer
		        m_CurrentTime = timeGetTime();

	        } // End If no hardware available

	        // Calculate elapsed time in seconds
	        fTimeElapsed = (m_CurrentTime - m_LastTime) * m_TimeScale;

        } // End While
    } // End If

	// Save current frame time
	m_LastTime = m_CurrentTime;

    // Filter out values wildly different from current average
    if ( fabsf(fTimeElapsed - m_TimeElapsed) < 1.0f  )
    {
        // Wrap FIFO frame time buffer.
        memmove( &m_FrameTime[1], m_FrameTime, (MAX_SAMPLE_COUNT - 1) * sizeof(float) );
        m_FrameTime[ 0 ] = fTimeElapsed;
        if ( m_SampleCount < MAX_SAMPLE_COUNT ) m_SampleCount++;

    } // End if
    

	// Calculate Frame Rate
	m_FPSFrameCount++;
	m_FPSTimeElapsed += m_TimeElapsed;
	if ( m_FPSTimeElapsed > 1.0f) 
    {
		m_FrameRate			= m_FPSFrameCount;
		m_FPSFrameCount		= 0;
		m_FPSTimeElapsed	= 0.0f;
	} // End If Second Elapsed

    // Count up the new average elapsed time
    m_TimeElapsed = 0.0f;
    for ( ULONG i = 0; i < m_SampleCount; i++ ) m_TimeElapsed += m_FrameTime[ i ];
    if ( m_SampleCount > 0 ) m_TimeElapsed /= m_SampleCount;
}

//-----------------------------------------------------------------------------
// Name : GetFrameRate () 
// Desc : Returns the frame rate, sampled over the last second or so.
//-----------------------------------------------------------------------------
unsigned long CTIMER3::GetFrameRate( LPTSTR lpszString ) const
{
    // Fill string buffer ?
    if ( lpszString )
    {
        // Copy frame rate value into string
        _itot( m_FrameRate, lpszString, 10 );

        // Append with FPS
        strcat( lpszString, _T(" FPS") );

    } // End if build FPS string

    return m_FrameRate;
}

//-----------------------------------------------------------------------------
// Name : GetTimeElapsed () 
// Desc : Returns the amount of time elapsed since the last frame (Seconds)
//-----------------------------------------------------------------------------
float CTIMER3::GetTimeElapsed() const
{
    return m_TimeElapsed;
}
