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
extern int                  iMouse_x, iMouse_y;

////PROTOTYPES/////////////////
void CreateParticleExplosion(int x, int y, float Velinit, int life, float gravity, float angle);
BOOL DrawParticleExplosion(DDSURFACEDESC2);
BOOL MoveParticleExplosion(int);

////FUNCTIONS//////////////////
//-----------------------------------------------------------------------------
// Name: Main()
// Desc: Where all work is done
//-----------------------------------------------------------------------------
bool main()
{
	static int frames=0, nCurrentMaxParticles,count=0;
	static float fps=0.0f,fps2;
	static char buffer[1024],buffer2[100];
	
	frames++;
	count++;

	if(GetKeyStatus(VK_ESCAPE) == true)
		return(0);
	
	if(count == 1)
		CreateParticleExplosion(400,300,0.08,10,0.09f,999);//0.28 1.3
	if(count >= 300)
		count =0;
	//Clear the surface
	if(!ClrS(lpddsSecondary,wndRect))//can't be within a lock-Unlock block or u can't blt
		return(0);

	//Lock the surface for drawing
	if(!Lock(lpddsSecondary))
		return(0);
	
	DrawParticleExplosion(ddsd);
		
	//Unlock the surface
	if(!UnLock(lpddsSecondary))
		return(0);
	
	//----Output Info----
	fps = ((float)frames)/((float)(gametimer.GetStartElapsedTime())*0.001f);
	sprintf(buffer,"FPS= %g", fps);
	DrawStringGDI(lpddsSecondary,0,0,buffer);
	sprintf(buffer,"Frame#= %d", frames);
	DrawStringGDI(lpddsSecondary,0,15,buffer);
	sprintf(buffer,"Time(ms)= %g",(GetTickCount()-Explosion.fStartTime)*0.001f);
	DrawStringGDI(lpddsSecondary,0,30,buffer);
	sprintf(buffer,"x= %d", iMouse_x);
	DrawStringGDI(lpddsSecondary,0,45,buffer);
	sprintf(buffer,"y= %d", iMouse_y);
	DrawStringGDI(lpddsSecondary,0,60,buffer);
	sprintf(buffer,"[10].vy= %g", Explosion.particle[10].vy);
	DrawStringGDI(lpddsSecondary,0,75,buffer);
	//----End  Output Info----

	//Flip primary and secondary surfaces
	Flip();

	frametime = frametimer.EndTimerAndRestart();
	return(1);
}

void CreateParticleExplosion(int x, int y, float Velinit, int life, float gravity, float angle)
{
	int m;
	
	Explosion.Active = true;
	Explosion.initVel = Velinit;
	Explosion.x = x;
	Explosion.y = y;
	Explosion.fStartTime = GetTickCount();

	for(int i=0; i<= MAX_PARTICLES-1; i++)
	{
		Explosion.particle[i].x = 0;
		Explosion.particle[i].y = 0;
		
		Explosion.particle[i].initvel =  (rand()/32768.0f)*Velinit+(rand()/32768.0f)*Velinit+(rand()/32768.0f)*Velinit;
		/*@@@@@@@@@@Diffrent effects@@@@@@@@@@@@@@@@@@@@@
		Explosion.particle[i].initvel =((rand()/32768.0f)*Velinit+(rand()/32768.0f)*Velinit+(rand()/32768.0f)*Velinit)*sqrt((rand()/32768.0f)*Velinit);
		Explosion.particle[i].initvel =(rand()/32768.0f)*Velinit+(rand()/32768.0f)*Velinit+((rand()/32768.0f)*Velinit)*sqrt((rand()/32768.0f)*Velinit);
		*/
		if(angle < 999)
		{
			if(Rand(0,1) == 1)
				m = -1;
			else
				m=1;
			Explosion.particle[i].angle = -angle + m*((rand()/32768.0f)*10);//go ten degrees up or down of the angle
		}
		else
			Explosion.particle[i].angle = (rand()/32768.0f)*360;
		
		//Explosion.particle[i].fadefactor = Rand(0.0f,3.0f); //fade the particles by a random amount
		Explosion.particle[i].r = 0.0f;//Rand(200,255);
		Explosion.particle[i].g = 0.0f;//Rand(200,255);
		Explosion.particle[i].b = 0.f;//Rand(200,255);
		Explosion.particle[i].Active = true;
		Explosion.particle[i].gravity = gravity;
		Explosion.particle[i].vx0 = Explosion.particle[i].initvel*cos( (Explosion.particle[i].angle*PI)/180.0f);
		Explosion.particle[i].vy0 = Explosion.particle[i].initvel * sin( (Explosion.particle[i].angle*PI)/180.0f );
	}
}

BOOL DrawParticleExplosion(DDSURFACEDESC2 ddsdtoaSurface)
{
	//BOOL finished = TRUE;
		
	if(Explosion.Active)
	{
		for(int i=0; i<= MAX_PARTICLES-1; i++)
		{
			if(Explosion.particle[i].Active)
			{
				//finished = FALSE;
				Explosion.particle[i].fadefactor = 1.0f;//Rand(0.0f,3.0f); 
				if( (Explosion.particle[i].r -= Explosion.particle[i].fadefactor) < 0.0f)
					Explosion.particle[i].r = 0.0f;
				if( (Explosion.particle[i].g -= Explosion.particle[i].fadefactor) < 0.0f)
					Explosion.particle[i].g = 0.0f;
				if( (Explosion.particle[i].b -= Explosion.particle[i].fadefactor) < 0.0f)
					Explosion.particle[i].b = 0.0f;
				DrawPixel(ddsdtoaSurface,Explosion.x+Explosion.particle[i].x,Explosion.y+Explosion.particle[i].y,
						  (int)Explosion.particle[i].r,
						  (int)Explosion.particle[i].g,
						  (int)Explosion.particle[i].b );
				MoveParticleExplosion(i);

				if( (Explosion.particle[i].r <= 0.0) && (Explosion.particle[i].g <= 0.0) && (Explosion.particle[i].b <= 0.0) )
					Explosion.particle[i].Active = FALSE;
				else
					Explosion.particle[i].Active = TRUE;
			}
		}
	}
	//if(finished)
		//Explosion.Active = FALSE;

	return TRUE;
}

BOOL MoveParticleExplosion(int i)
{//move the particles
	float time = (((float)(GetTickCount()-Explosion.fStartTime))*0.001f),
		  frictionX=1,
		  frictionY=1;
	
	Explosion.particle[i].vx = Explosion.particle[i].vx0*(frametime*1000);
	Explosion.particle[i].vy = ( Explosion.particle[i].vy0  )*(frametime*1000);
	Explosion.particle[i].vy += (Explosion.particle[i].gravity*time*0.5)*(frametime*1000);
	
	Explosion.particle[i].x += Explosion.particle[i].vx*frictionX;
	Explosion.particle[i].y += Explosion.particle[i].vy*frictionY;
	
	return true;
}