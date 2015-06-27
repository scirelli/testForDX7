//-----------------------------------------------------------------------------
// File: Main.cpp
//
// Desc: Where everything is done
//
// Big problem is u have to switch everything to 24 or 32bit
//or use r0~31 g0~63 b0~31
//-----------------------------------------------------------------------------
////INCLUDES///////////////////
#include "General.h"
#include "CreateRGB.h"
#include "PrimitiveShapes.h"
#include "Bitmap.h"
#include "SCTimer.h"
#include "Sprite.h"
#include "SCMath.h"

////GLOBALS////////////////////
extern LPDIRECTDRAWSURFACE7 lpddsPrimary;    // dd primary surface
extern LPDIRECTDRAWSURFACE7 lpddsSecondary;  // dd back surface
extern LPDIRECTDRAWSURFACE7 lpddsTempSurface[];
extern DDSURFACEDESC2       ddsd;            // a direct draw surface description struct
extern HRESULT              ddReturnVal;     // result back from dd calls
extern DDBLTFX              ddbltfx;
extern char                 cpErrorBuf[1024];
extern RECT                 wndRect; 
extern HWND					main_window_handle;
extern CTIMER               gametimer;
extern CTIMER2              gametimer2,frametimer;
extern CSPRITE              Sprites[MAX_SPRITES];
extern PARTICLE             particles[MAX_PARTICLES];
tParticalExplotion          Explosion;
extern float                frametime;
extern int                  iMouse_x, iMouse_y, iWindow_x, iWindow_y,
                            nWindowWidth, nWindowHeight;

////PROTOTYPES/////////////////


////FUNCTIONS//////////////////
//-----------------------------------------------------------------------------
// Name: Main()
// Desc: Where all work is done
//-----------------------------------------------------------------------------
bool main()
{
	static int nCurrentMaxParticles,count=0;
	static long frames=0;
	static float fps=0.0f;
	static char buffer[1024];
	
	frames++;
	count++;

	if(GetKeyStatus(VK_ESCAPE) == true)
		return(0);
	
	//Clear the surface
	if(!ClrS(lpddsTempSurface[1],NULL,255,0,255))//can't be within a lock-Unlock block or u can't blt
		return(0);

	//Lock the surface for drawing
	if(!Lock(lpddsPrimary))
		return(0);

	
	//Unlock the surface
	if(!UnLock(lpddsPrimary))
		return(0);
	
	//RECT sorRect={300,300,332,332};
	//CopySurface(lpddsTempSurface[2],NULL,lpddsPrimary,&sorRect);	
	//CopySurface(lpddsSecondary,NULL,lpddsTempSurface[2],NULL);
	Sprites[2].DrawSprite(lpddsTempSurface[1]);
	
	//----Output Info----
	//fps = ((float)frames)/((float)(gametimer.GetStartElapsedTime())*0.001f);
	//sprintf(buffer,"FPS= %g", fps);
	//DrawStringGDI(lpddsSecondary,0,0,buffer);
	//sprintf(buffer,"Frame#= %d", frames);
	//DrawStringGDI(lpddsSecondary,0,15,buffer);
	//sprintf(buffer,"x= %d", iMouse_x);
	//DrawStringGDI(lpddsSecondary,0,30,buffer);
	//sprintf(buffer,"y= %d", iMouse_y);
	//DrawStringGDI(lpddsSecondary,0,45,buffer);
	//----End  Output Info----
		
	//Flip everything
	Flip(lpddsTempSurface[1],300,300,332,332);
	//ShowWindow(main_window_handle,SW_HIDE);
	//ShowWindow(main_window_handle,SW_SHOW);
	frametime = frametimer.EndTimerAndRestart();
	return(1);
}