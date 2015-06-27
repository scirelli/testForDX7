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
#define THRUST 1591379.99f //kg*km/s^2
#define VEL_CAP 10
////GLOBALS////////////////////
extern LPDIRECTDRAWSURFACE7 lpddsPrimary;    // dd primary surface
extern LPDIRECTDRAWSURFACE7 lpddsSecondary;  // dd back surface
extern DDSURFACEDESC2       ddsd;            // a direct draw surface description struct
extern HRESULT              ddReturnVal;     // result back from dd calls
extern DDBLTFX              ddbltfx;
extern char                 cpErrorBuf[1024];
extern RECT                 wndRect; 
extern HWND					main_window_handle;
extern CTIMER               gameTimer, frameTimer2;
extern CTIMER2              frameTimer, gameTimer2;
extern float                fFrameTime;      //s
extern CSPRITE              Sprites[MAX_SPRITES];
extern PARTICLE             particles[MAX_PARTICLES];
extern bool                 bDebugMode;

////PROTOTYPES/////////////////
void DebugInfo(int);
void MoveBlocks();

////FUNCTIONS//////////////////
//-----------------------------------------------------------------------------
// Name: Main()
// Desc: Where all work is done
//-----------------------------------------------------------------------------
bool main()
{
	static int nFrames = 0;
	static CVECTOR starsVelVector(60.f,60.f,0.f);
	static CTIMER introTimer;
	
	if(GetKeyStatus(VK_ESCAPE) == true)
		return(0);
	
	//Clear the surface
	if(!ClrS( ))//can't be within a lock-Unlock block or u can't blt
		return(0);
	
	DynamicStarField(NULL,800,starsVelVector,nFrames);
	
	if(nFrames == 0)
		introTimer.StartTimer();
	if(introTimer.Elapsed(1000))
		Sprites[0].DrawSprite(lpddsSecondary);
	if(Sprites[0].GetAnimAttr(0) == SPRITE_ANIM_DONE)
	{
		MoveBlocks();
		Sprites[1].DrawSprite(lpddsSecondary);
		Sprites[2].DrawSprite(lpddsSecondary);
	}

	if(bDebugMode)
		DebugInfo(nFrames);
	
	//Flip primary and secondary surfaces
	Flip();
	//Sleep(1000);
	fFrameTime = frameTimer.EndTimerAndRestart();
	nFrames++;
	return(1);
}

//-----------------------------------------------------------------------------
// Name: MoveBlocks()
// Desc: Moves the two blocks of the logo
//-----------------------------------------------------------------------------
void MoveBlocks()
{
	Sprites[1].physProp.accel.y = 0.5f*fFrameTime;
	Sprites[1].physProp.VelVector.y += Sprites[1].physProp.accel.y;
	Sprites[1].physProp.fY = Sprites[1].GetYPos()+(Sprites[1].physProp.VelVector.y*fFrameTime);
	Sprites[1].SetYPos(Sprites[1].physProp.fY);
	if( Sprites[1].GetYPos() >= ( Sprites[0].GetYPos()+4 ) )
	{
		Sprites[1].SetYPos(Sprites[0].GetYPos()+4);
		static CTIMER temp;
		static DWORD temp2 = temp.StartTimer();
		if(temp.Elapsed(500))
		{
			Sprites[2].physProp.accel.y = 0.5f*fFrameTime;
			Sprites[2].physProp.VelVector.y += Sprites[1].physProp.accel.y;
			Sprites[2].physProp.fY = Sprites[2].GetYPos()+(Sprites[2].physProp.VelVector.y*fFrameTime);
			Sprites[2].SetYPos(Sprites[2].physProp.fY);
			if( Sprites[2].GetYPos() >= ( Sprites[0].GetYPos()+4 ) )
				Sprites[2].SetYPos(Sprites[0].GetYPos()+4);
		}
	}
}

//-----------------------------------------------------------------------------
// Name: DebugInfo()
// Desc: out puts debug info
//-----------------------------------------------------------------------------
void DebugInfo(int nFrames)
{
	static float fps=0.0f;
	static char cpBuffer[1024];

	sprintf(cpBuffer,"Render Time= %g",fFrameTime);
	DrawStringGDI(lpddsSecondary,0,0,cpBuffer);
	fps = 1.f/fFrameTime;
	sprintf(cpBuffer,"FPS= %g",fps);
	DrawStringGDI(lpddsSecondary,0,15,cpBuffer);
	sprintf(cpErrorBuf,"Anim. Index= %d",Sprites[0].GetCurrentAnimNum());
	DrawStringGDI(lpddsSecondary,100,15,cpErrorBuf);
	sprintf(cpBuffer,"Ship VelX= %g",(float)Sprites[0].physProp.VelVector.XComponent());
	DrawStringGDI(lpddsSecondary,100,30,cpBuffer);
	sprintf(cpBuffer,"Ship VelY= %g",(float)Sprites[0].physProp.VelVector.YComponent());
	DrawStringGDI(lpddsSecondary,100,45,cpBuffer);
	sprintf(cpBuffer,"Game Timer (sec)= %g",(float)gameTimer.GetStartElapsedTime()*0.001f);
	DrawStringGDI(lpddsSecondary,100,90,cpBuffer);
	sprintf(cpBuffer,"Ship Center of Mass X= %g",(float)Sprites[0].physProp.fCenterofMassX);
	DrawStringGDI(lpddsSecondary,0,165,cpBuffer);
	sprintf(cpBuffer,"Ship Center of Mass Y= %g",(float)Sprites[0].physProp.fCenterofMassY);
	DrawStringGDI(lpddsSecondary,0,180,cpBuffer);
	sprintf(cpBuffer,"Ship X= %d", Sprites[0].GetXPos());
	DrawStringGDI(lpddsSecondary,130,105,cpBuffer);
	sprintf(cpBuffer,"Ship Y= %d", Sprites[0].GetYPos());
	DrawStringGDI(lpddsSecondary,130,120,cpBuffer);
	sprintf(cpBuffer,"Logo frame# %d", Sprites[0].GetCurrentFrameNum());
	DrawStringGDI(lpddsSecondary,130,135,cpBuffer);

}