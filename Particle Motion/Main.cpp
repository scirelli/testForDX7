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

////GLOBALS////////////////////
extern LPDIRECTDRAWSURFACE7 lpddsPrimary;    // dd primary surface
extern LPDIRECTDRAWSURFACE7 lpddsSecondary;  // dd back surface
extern DDSURFACEDESC2       ddsd;            // a direct draw surface description struct
extern HRESULT              ddReturnVal;     // result back from dd calls
extern DDBLTFX              ddbltfx;
extern char                 cpErrorBuf[1024];
extern RECT                 wndRect; 
extern HWND					main_window_handle;
extern CTIMER               gametimer;
extern CTIMER2              gametimer2;
extern CSPRITE              Sprites[MAX_SPRITES];
extern PARTICLE             particles[MAX_PARTICLES];
extern int                  iMouse_x, iMouse_y;

////PROTOTYPES/////////////////


////FUNCTIONS//////////////////
//-----------------------------------------------------------------------------
// Name: Main()
// Desc: Where all work is done
//-----------------------------------------------------------------------------
bool main()
{
	static int frames=0;
	static float fps=0.0f;
	static char buffer[1024];
	frames++;
	

	if(GetKeyStatus(VK_ESCAPE) == true)
		return(0);
	
	//Clear the surface
	if(!ClrS(lpddsSecondary,wndRect))//can't be within a lock-Unlock block or u can't blt
		return(0);

	//Lock the surface for drawing
	if(!Lock(lpddsSecondary))
		return(0);
	

	//Unlock the surface
	if(!UnLock(lpddsSecondary))
		return(0);
	
	//----Output info----
	fps = ((float)frames)/((float)(gametimer.GetStartElapsedTime())*0.001f);
	sprintf(buffer,"FPS= %g", fps);
	DrawStringGDI(lpddsSecondary,0,0,buffer);
	sprintf(buffer,"x= %d", iMouse_x);
	DrawStringGDI(lpddsSecondary,0,40,buffer);
	sprintf(buffer,"y= %d", iMouse_y);
	DrawStringGDI(lpddsSecondary,0,60,buffer);
	//----End  Output Info----

	//Flip primary and secondary surfaces
	Flip();
	return(1);
}