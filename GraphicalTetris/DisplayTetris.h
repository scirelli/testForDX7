//-----------------------------------------------------------------------------
// File: DisplayTetris.h
//
// Desc: Definition of the CDispTetris class used to graphicaly display Textris
//-----------------------------------------------------------------------------

#ifndef DisplayTetris_h
#define DisplayTetris_h

////INCLUDES//////////////////
#include "CTetris.h"

////GLOBALS////////////////////
extern LPDIRECTDRAWSURFACE7 lpddsPrimary;    // dd primary surface
extern LPDIRECTDRAWSURFACE7 lpddsSecondary;  // dd back surface
extern DDSURFACEDESC2       ddsd;            // a direct draw surface description struct
extern HRESULT              ddReturnVal;     // result back from dd calls
extern DDBLTFX              ddbltfx;
extern HWND					main_window_handle;

////CONSTANTS/////////////////

////STRUCTS///////////////////

////CLASSES///////////////////

//----------------------------------
// Class: CDispTetris
//----------------------------------
class CDispTetris
{
private:
	//----Private Member Variables----
	CTetris *m_tetrisGame;
	CTIMER3 *m_timer;
	bool     m_bFullScreen;
	RECT     m_windowRect;
	RECT     m_clientRect;
	RECT     m_secondarySurfRect;
	
	//----Private Methods----
	void DebugInfo(int);

public:
	//----Public Member Variables----
	
	//----Public Methods----
	CDisplayTetris();
	~CDisplayTetris() {}
};

#endif