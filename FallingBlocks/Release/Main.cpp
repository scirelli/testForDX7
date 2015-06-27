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
void MoveShip();
void DebugInfo(int);

////FUNCTIONS//////////////////
//-----------------------------------------------------------------------------
// Name: Main()
// Desc: Where all work is done
//-----------------------------------------------------------------------------
bool main()
{
	static int nFrames = 0;
	//static CVECTOR shipsVelVector(0.f,0.f,0.f);
	
	if(GetKeyStatus(VK_ESCAPE) == true)
		return(0);
	
	//Clear the surface
	if(!ClrS( ))//can't be within a lock-Unlock block or u can't blt
		return(0);
	Sprites[0].physProp.VelVector.XComponent();
	DynamicStarField(NULL,800,Sprites[0].physProp.VelVector,nFrames);
	Sprites[0].DrawSprite(lpddsSecondary);
	MoveShip();
	
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
// Name: MoveShip()
// Desc: 
//-----------------------------------------------------------------------------
void MoveShip()
{
	//static float angle = 90.f;
	static int nIndex = Sprites[0].GetCurrentAnimNum();
	static int array[]={0,0};
	static float fDegreeCount = 0;
	static int temp=0, nTimeBetweenFrames=130;
	
	//if(GetKeyStatus(VK_DOWN) == true)
	//{//decelerating
	//	return;
	//}
	if(GetKeyStatus(VK_RIGHT) == true) //increases angle
	{//turns the ship clockwise and changes the angle 10 degrees
	 	fDegreeCount += 7.5f;
		
		if(fabs(fDegreeCount) >= 22.5f)
		{
			Sprites[0].physProp.angle += 22.5f;
			if(Sprites[0].physProp.angle >= 360)
				Sprites[0].physProp.angle = Sprites[0].physProp.angle-360;
			nIndex++;
			if( nIndex > 15 )
				nIndex = 0;
			
			if(nIndex>7)
				temp = nIndex+8;
			else
				temp = nIndex;
			array[0] = temp; array[1] = temp;
			Sprites[0].SetAnimations(array,1,SPRITE_ANIM_ONCE,16);
			Sprites[0].SetCurrentAnim(16);
			
			fDegreeCount=0;
		}
	}
	else
		if(GetKeyStatus(VK_LEFT) == true) //decreases angle
		{//turns the ship counter-clockwise and changes the angle
			fDegreeCount -= 7.5f;
		
			if(fabs(fDegreeCount) >= 22.5f)
			{
				Sprites[0].physProp.angle -= 22.5f;
				if(Sprites[0].physProp.angle <= 0)
					Sprites[0].physProp.angle = (360+Sprites[0].physProp.angle);
	
				nIndex--;
				if(nIndex < 0)
					nIndex = 15;
			
				if(nIndex>7)
					temp = nIndex+8;
				else
					temp = nIndex;
				array[0] = temp; array[1] = temp;
				Sprites[0].SetAnimations(array,1,SPRITE_ANIM_ONCE,16);
				Sprites[0].SetCurrentAnim(16);

				fDegreeCount=0;
			}
	}
	
	if(GetKeyStatus(VK_UP) == true)
	{//accelerating
		Sprites[0].physProp.VelVector.x += ((THRUST*cos(Deg_Rad(Sprites[0].physProp.angle)))/Sprites[0].physProp.fMass)*fFrameTime;
		Sprites[0].physProp.VelVector.y += ((THRUST*sin(Deg_Rad(Sprites[0].physProp.angle)))/Sprites[0].physProp.fMass)*fFrameTime;
		if(Sprites[0].physProp.VelVector.x > VEL_CAP)
			Sprites[0].physProp.VelVector.x = VEL_CAP;
		if(Sprites[0].physProp.VelVector.x < -VEL_CAP)
			Sprites[0].physProp.VelVector.x = -VEL_CAP;
		if(Sprites[0].physProp.VelVector.y > VEL_CAP)
			Sprites[0].physProp.VelVector.y = VEL_CAP;
		if(Sprites[0].physProp.VelVector.y < -VEL_CAP)
			Sprites[0].physProp.VelVector.y = -VEL_CAP;
		nTimeBetweenFrames-=5;
		if(nTimeBetweenFrames < 30)
			nTimeBetweenFrames=30;
		Sprites[0].SetTimeBetweenFrames(nTimeBetweenFrames);
		Sprites[0].SetCurrentAnim(nIndex);
	}
	else
	{
		if(nIndex>7)
			temp = nIndex+8;
		else
			temp = nIndex;
		
		array[0] = temp; array[1] = temp;
		Sprites[0].SetAnimations(array,1,SPRITE_ANIM_ONCE,16);
		Sprites[0].SetCurrentAnim(16);
		nTimeBetweenFrames=130;
	}

	//----Debug info----
	if(bDebugMode)
	{
		sprintf(cpErrorBuf,"Angle= %g",Sprites[0].physProp.angle);
		DrawStringGDI(lpddsSecondary,100,60,cpErrorBuf);
		sprintf(cpErrorBuf,"Temp Index= %d",temp);
		DrawStringGDI(lpddsSecondary,100,75,cpErrorBuf);
		sprintf(cpErrorBuf,"Ship index= %d", nIndex);
		DrawStringGDI(lpddsSecondary,130,135,cpErrorBuf);
		sprintf(cpErrorBuf,"AnimationSeqMax()= %d", Sprites[0].GetAnimSeqMax(Sprites[0].GetCurrentAnimNum())+1);
		DrawStringGDI(lpddsSecondary,130,150,cpErrorBuf);
	}
	//----End Debug info----
}

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

}