//-----------------------------------------------------------------------------
// File: DisplayTetris.cpp
//
// Desc: Implementation of the CDispTetris class used to graphicaly display 
//       Textris
//-----------------------------------------------------------------------------


////INCLUDES//////////////////
#include "DisplayTetris.h"

////METHODS///////////////////
//--------------------------------------------------------------
// Name: DebugInfo()
// Desc: Used to debug the program
//--------------------------------------------------------------
void CDispTetris::DebugInfo(int nFrames)
{
	//static float fps=0.0f;
	static char cpBuffer[1024];
	static int startY = 10;

	sprintf(cpBuffer,"Render Time= %g", m_timer->GetTimeElapsed());
	DrawStringGDI(lpddsSecondary, 0, startY+0, cpBuffer);
	//fps = 1.f/fFrameTime;
	sprintf(cpBuffer,"FPS= %d", (int)m_timer->GetFrameRate());
	DrawStringGDI(lpddsSecondary, 0, startY+15, cpBuffer);
}