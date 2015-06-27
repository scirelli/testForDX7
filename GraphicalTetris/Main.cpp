//-----------------------------------------------------------------------------
// File: Main.cpp
//
// Desc: Where everything is done
//
//-----------------------------------------------------------------------------
////INCLUDES///////////////////
#include "General.h"
#include "CreateRGB.h"
#include "PrimitiveShapes.h"
#include "Bitmap.h"
#include "SCTimer.h"
#include "Sprite.h"
#include "SCMath.h"
#include "SurfaceFunc.h"
#include "DynamicSpaceField.h"

////DEFINES////////////////////

////GLOBALS////////////////////
extern LPDIRECTDRAWSURFACE7 lpddsPrimary;    // dd primary surface
extern LPDIRECTDRAWSURFACE7 lpddsSecondary;  // dd back surface
extern DDSURFACEDESC2       ddsd;            // a direct draw surface description struct
extern HRESULT              ddReturnVal;     // result back from dd calls
extern DDBLTFX              ddbltfx;
extern char                 cpErrorBuf[1024];
extern RECT                 wndRect; 
extern HWND					main_window_handle;
extern CTIMER3              frameTimer,gameTimer;
extern float                fFrameTime;      //s

////PROTOTYPES/////////////////
int  Display( CTetris& );
void AnimRowCompl( CTetris&, int );
int  DisplayWin( CTetris & );
int  DisplayLose( CTetris & );

////FUNCTIONS//////////////////
//-----------------------------------------------------------------------------
// Name: Main()
// Desc: Where all work is done
// 
// Note: Use GetWindowRect(main_window_handle, &temp); if bounds are needed
//-----------------------------------------------------------------------------
bool main()
{
	static int nFrames = 0;
	frameTimer.Tick();
	
	if(GetKeyStatus(VK_ESCAPE) == true)
		return(0);
	
	//Clear the surface
	if(!ClrS( ))//can't be within a lock-Unlock block or u can't blt
		return(0);
	
	//Sprites[0].physProp.VelVector.XComponent();
	//DynamicStarField(NULL,800,Sprites[0].physProp.VelVector,nFrames);
	//Sprites[0].DrawSprite(lpddsSecondary);
	//MoveShip();
	
	if(bDebugMode)
		DebugInfo(nFrames);
	
	//Flip primary and secondary surfaces
	FlipToClient(lpddsSecondary);
	//Sleep(1000);
	fFrameTime = frameTimer.GetTimeElapsed();
	nFrames++;
	return(1);
}