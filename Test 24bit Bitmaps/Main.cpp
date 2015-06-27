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
extern CTIMER2              Frametimer;
extern CSPRITE              Sprites[MAX_SPRITES];
extern PARTICLE             particles[MAX_PARTICLES];

////PROTOTYPES/////////////////
void Init();

////FUNCTIONS//////////////////
//-----------------------------------------------------------------------------
// Name: Main()
// Desc: Where all work is done
//-----------------------------------------------------------------------------
bool main()
{
	static int frames=0;
	static float fps=0.0f, frametime=0.0f;
	static char buffer[1024];
	frames++;
	
	//if(frames == 1)
	//	bitmap.LoadBitmapFile24();
	if(GetKeyStatus(VK_ESCAPE) == true)
		return(0);
	
	//Clear the surface
	if(!ClrS(lpddsSecondary,&wndRect))//can't be within a lock-Unlock block or u can't blt
		return(0);

	//Lock the surface for drawing
	//if(!Lock(lpddsPrimary))
	//	return(0);
/*
	UCHAR *imagebuffer=NULL;
	imagebuffer = (UCHAR*)ddsd.lpSurface;
	for(int y=0; y<bitmap.GetHeight(); y++)
	{
		memcpy( &imagebuffer[y*ddsd.lPitch], 
		        &bitmap.GetImageData()[y*bitmap.GetWidth()*4],
				bitmap.GetWidth()*4 );
		
	}
*/
	//Unlock the surface
	//if(!UnLock(lpddsPrimary))
	//	return(0);
	
	Sprites[0].DrawSprite(lpddsSecondary);
	Sprites[1].DrawSprite(lpddsSecondary);
	Sprites[2].DrawSprite(lpddsSecondary);
	frametime = Frametimer.EndTimerAndRestart();
	//----Output FPS----
	sprintf(buffer,"Render Time= %g",frametime);
	DrawStringGDI(lpddsSecondary,0,0,buffer);
	fps = ((float)frames)/((float)(gametimer.GetStartElapsedTime())*0.001f);
	sprintf(buffer,"FPS= %g",fps);
	DrawStringGDI(lpddsSecondary,0,20,buffer);
	//----End  FPS----

	//Flip primary and secondary surfaces
	Flip();
	return(1);
}