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
#include "StaticStarField.h"

////GLOBALS////////////////////
extern LPDIRECTDRAWSURFACE7 lpddsPrimary;    // dd primary surface
extern LPDIRECTDRAWSURFACE7 lpddsSecondary;  // dd back surface
extern DDSURFACEDESC2       ddsd;            // a direct draw surface description struct
extern HRESULT              ddReturnVal;     // result back from dd calls
extern DDBLTFX              ddbltfx;
extern char                 cpErrorBuf[1024];
extern RECT                 wndRect; 
extern HWND					main_window_handle;
extern CTIMER               gameTimer;
extern CTIMER2              frameTimer;
extern CSPRITE              Sprites[MAX_SPRITES];
extern PARTICLE             particles[MAX_PARTICLES];
extern bool                 bDebugMode;

////PROTOTYPES/////////////////
void Init();

////FUNCTIONS//////////////////
//-----------------------------------------------------------------------------
// Name: Main()
// Desc: Where all work is done
//-----------------------------------------------------------------------------
bool main()
{
	static double nFrames=0;
	static float fps=0.0f, frameTime=0.0f;
	static char cpBuffer[1024];
	static RECT rect = {150,150,250,250};

	if(GetKeyStatus(VK_ESCAPE) == true)
		return(0);
	
	//Clear the surface
	if(!ClrS(lpddsSecondary,&wndRect))//can't be within a lock-Unlock block or u can't blt
		return(0);
	
	StaticStarField(NULL,1, nFrames);
	Sprites[0].DrawSprite(lpddsSecondary);
	//OutputDebugString("Outside");
	if(bDebugMode)
	{//OutputDebugString("Inside");
		//----Output FPS----
		frameTime = frameTimer.EndTimerAndRestart();
		sprintf(cpBuffer,"Render Time= %g",frameTime);
		DrawStringGDI(lpddsSecondary,0,0,cpBuffer);
		fps = ((float)nFrames)/((float)(gameTimer.GetStartElapsedTime())*0.001f);
		sprintf(cpBuffer,"FPS= %g",fps);
		DrawStringGDI(lpddsSecondary,0,20,cpBuffer);
		sprintf(cpBuffer,"Frame# %g", nFrames);
		DrawStringGDI(lpddsSecondary,0,40,cpBuffer);
		sprintf(cpBuffer,"gameTimer= %d", gameTimer.GetStartElapsedTime());
		DrawStringGDI(lpddsSecondary,0,60,cpBuffer);
		//----End  FPS----
	}
	//Flip primary and secondary surfaces
	Flip();
	
	nFrames++;

	return(1);
}

/*
	//----Do pixel animation here----
	//Lock the surface for drawing
	if(!Lock(lpddsPrimary))
		return(0);


	//Unlock the surface
	if(!UnLock(lpddsPrimary))
		return(0);
	//----End do pixel animation here----
*/