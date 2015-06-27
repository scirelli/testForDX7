//-----------------------------------------------------------------------------
// File: StaticStarField.cpp
//
// Desc: 
//
//-----------------------------------------------------------------------------

////INCLUDES///////////////////
#include "General.h"
#include "CreateRGB.h"
#include "PrimitiveShapes.h"
#include "SCTimer.h"
#include "SCMath.h"
#include "SurfaceFunc.h"

////DEFINES////////////////////
#define PERCENT_OF_PIXELS 0.0011f
#define L1_FADE_AMOUNT 0
#define L2_FADE_AMOUNT 80
#define L3_FADE_AMOUNT 169

////GLOBALS////////////////////
extern LPDIRECTDRAWSURFACE7 lpddsSecondary;  // dd back surface
extern DDSURFACEDESC2       ddsd;            // a direct draw surface description struct
extern HRESULT              ddReturnVal;     // result back from dd calls
extern DDBLTFX              ddbltfx;
extern char                 cpErrorBuf[1024];
extern RECT                 wndRect; 
extern HWND					main_window_handle;
extern CTIMER2              frameTimer;
extern CTIMER               gameTimer;
extern bool                 bDebugMode;
PTR_PARTICLE                stars = NULL;

////PROTOTYPES/////////////////
int InitStars(PTR_PARTICLE &, RECT*, RECT&, float, double);
int DrawStars(PTR_PARTICLE, int);
int MoveStars(PTR_PARTICLE, int, RECT);

/////FUNCTIONS/////////////////

//-----------------------------------------------------------------------------
// Name: ClearUpStarField()
// Desc: deletes any allocated space created for the starfield
//-----------------------------------------------------------------------------
void CleanUpStarField()
{
	if(stars != NULL)
	{
		delete [] stars;
		stars = NULL;
	}
}

//-----------------------------------------------------------------------------
// Name: StaticStarField()
// Desc: draws and animates a constant star field. Meaning no scrolling
//-----------------------------------------------------------------------------
int StaticStarField(RECT *field, float fInitVel, double nFrame)
{
	static int nTotalAmountofStars = 0;
	static RECT tempField;
			
	if(!nFrame)
		nTotalAmountofStars = InitStars(stars, field, tempField, fInitVel, nFrame);
	if(bDebugMode)
	{
		sprintf(cpErrorBuf,"# of stars = %d",nTotalAmountofStars);
		DrawStringGDI(lpddsSecondary,0,80,cpErrorBuf);
		sprintf(cpErrorBuf,"time = %d", (unsigned int)timeGetTime()<<8);
		DrawStringGDI(lpddsSecondary,0,100,cpErrorBuf);
		float temp = 0;
		temp = fInitVel*cos(45);
		sprintf(cpErrorBuf,"Init velX= %g", temp);
		DrawStringGDI(lpddsSecondary,0,120,cpErrorBuf);
		temp = fInitVel*sin(45);
		sprintf(cpErrorBuf,"Init velY= %g", temp);
		DrawStringGDI(lpddsSecondary,0,140,cpErrorBuf);
		temp = stars[0].vx;
		sprintf(cpErrorBuf,"velX= %g", temp);
		DrawStringGDI(lpddsSecondary,0,160,cpErrorBuf);
	}
	DrawStars(stars,nTotalAmountofStars);
	//Sleep(500);
	MoveStars(stars,nTotalAmountofStars, tempField);
		
	return(1);
}

//-----------------------------------------------------------------------------
// Name: InitStars()
// Desc: Initiates all the stars and returns the total amount of stars to draw
//-----------------------------------------------------------------------------
int InitStars(PTR_PARTICLE &stars, 
			   RECT* field, RECT &tempField, 
			   float fInitVel, double nFrame)
{
	int nTotalAmountofStars=0;
	int nCount=0;
	
	//create the rect in which to draw the star field
	if(!field)
	{
		nTotalAmountofStars = (int)(WINDOW_WIDTH*WINDOW_HEIGHT*PERCENT_OF_PIXELS);
		tempField.top = 0; tempField.left = 0; tempField.bottom = WINDOW_HEIGHT; tempField.right = WINDOW_WIDTH;
	}
	else
	{
		nTotalAmountofStars = (int)((field->right - field->left)*(field->bottom - field->top)*PERCENT_OF_PIXELS);
		tempField.top = field->top; tempField.left = field->left; 
		tempField.bottom = field->bottom; tempField.right = field->right;
	}
	//allocate space for the amount of stars to draw
	stars = new PARTICLE[nTotalAmountofStars];
	
	//init the stars
	for(int i=0; i<nTotalAmountofStars; i++)
	{
		stars[i].Active = true;
		stars[i].angle = 45;
		stars[i].gravity = 0;
		stars[i].initvel = fInitVel;

		//you want the least amount of stars in the foreground so take a percent
		//of the total amount of stars and set their color to the brightest
		if(i < (nTotalAmountofStars*0.10) )
		{
			stars[i].r = 255-L1_FADE_AMOUNT; 
			stars[i].g = 255-L1_FADE_AMOUNT; 
			stars[i].b = 255-L1_FADE_AMOUNT;
			stars[i].fadefactor = L1_FADE_AMOUNT;
			stars[i].vx = stars[i].initvel*cos(stars[i].angle);
			stars[i].vy = stars[i].initvel*sin(stars[i].angle);
			stars[i].vx0 = stars[i].initvel*cos(stars[i].angle);
			stars[i].vy0 = stars[i].initvel*sin(stars[i].angle);
		}
		//same for the mid level stars except make them a brightness less then the lvl 1 stars
		if( (i >= (nTotalAmountofStars*0.10)) && (i<(nTotalAmountofStars*(0.30))) )
		{
			stars[i].r = 255-L2_FADE_AMOUNT; 
			stars[i].g = 255-L2_FADE_AMOUNT; 
			stars[i].b = 255-L2_FADE_AMOUNT;
			stars[i].fadefactor = L2_FADE_AMOUNT;
			stars[i].vx = stars[i].initvel*cos(stars[i].angle)*0.5f;
			stars[i].vy = stars[i].initvel*sin(stars[i].angle)*0.5f;
			stars[i].vx0 = stars[i].initvel*cos(stars[i].angle)*0.5f;
			stars[i].vy0 = stars[i].initvel*sin(stars[i].angle)*0.5f;
		}
		//same for the lvl 3 stars make them the dimest
		if( (i>=(nTotalAmountofStars*(0.30)))&& (i<=nTotalAmountofStars) )
		{
			stars[i].r = 255-L3_FADE_AMOUNT; 
			stars[i].g = 255-L3_FADE_AMOUNT; 
			stars[i].b = 255-L3_FADE_AMOUNT;
			stars[i].fadefactor = L3_FADE_AMOUNT;
			stars[i].vx = stars[i].initvel*cos(stars[i].angle)*(0.33f);
			stars[i].vy = stars[i].initvel*sin(stars[i].angle)*(0.33f);
			stars[i].vx0 = stars[i].initvel*cos(stars[i].angle)*(0.33f);
			stars[i].vy0 = stars[i].initvel*sin(stars[i].angle)*(0.33f);
		}
		
		if( (++nCount) == 100)
		{
			//srand(rand());
			nCount=0;
		}
		stars[i].x = tempField.left + ( (rand()/(float)RAND_MAX)*(tempField.right-tempField.left) );
		if( stars[i].x >= tempField.right )
			stars[i].x = (float)tempField.right;
		else
			if( stars[i].x <= tempField.left )
				stars[i].x = (float)tempField.left;
		stars[i].y = tempField.top + ( (rand()/(float)RAND_MAX)*(tempField.bottom-tempField.top) );
		if( stars[i].y >= tempField.bottom )
			stars[i].y = (float)tempField.bottom;
		else
			if( stars[i].y <= tempField.top )
				stars[i].y = (float)tempField.top;
	}
	
	return nTotalAmountofStars;
}

//-----------------------------------------------------------------------------
// Name: MoveStars()
// Desc: moves the stars
//-----------------------------------------------------------------------------
int MoveStars(PTR_PARTICLE stars, int nTotalAmountofStars, RECT tempField)
{
	for(int i=0; i<nTotalAmountofStars; i++)
	{
		stars[i].x += stars[i].vx;//*((frameTimer.Elapsed()));
		stars[i].y += stars[i].vy;//*((frameTimer.Elapsed()));
		//stars[i].vy = 0;
		//stars[i].vx0 = stars[i].initvel*cos(stars[i].angle)*(1/3);
		//stars[i].vy0 = stars[i].initvel*sin(stars[i].angle)*(1/3);
		if( stars[i].x >= tempField.right )
		{
			stars[i].x = (float)tempField.left;
			stars[i].vx = stars[i].vx0;
			stars[i].vy = stars[i].vy0;
		}
		else
			if( stars[i].x <= tempField.left )
			{
				stars[i].x = (float)tempField.right;
				stars[i].vx = stars[i].vx0;
				stars[i].vy = stars[i].vy0;
			}
		if( stars[i].y >= tempField.bottom )
		{
			stars[i].y = (float)tempField.top;
			stars[i].vx = stars[i].vx0;
			stars[i].vy = stars[i].vy0;
		}
		else
			if( stars[i].y <= tempField.top )
			{
				stars[i].y = (float)tempField.bottom;
				stars[i].vx = stars[i].vx0;
				stars[i].vy = stars[i].vy0;
			}

	}
	
	return 1;
}

//-----------------------------------------------------------------------------
// Name: DrawStars()
// Desc: draws the stars
//-----------------------------------------------------------------------------
int DrawStars(PTR_PARTICLE stars, int nTotalAmountofStars)
{
	
	//----Do pixel animation here----
	//Lock the surface for drawing
	if(!Lock(lpddsSecondary))
		return(0);
	
	for(int i=0; i<nTotalAmountofStars; i++)
		DrawPixel32(ddsd,(int)stars[i].x,(int)stars[i].y, stars[i].r,stars[i].g,stars[i].b);
/*	
	DrawPixel32(ddsd,100,100, 255,255,255);
	DrawPixel32(ddsd,110,100, 170,170,170);
	DrawPixel32(ddsd,120,100, 86,86,86);
*/
	//Unlock the surface
	if(!UnLock(lpddsSecondary))
		return(0);
	//----End do pixel animation here----
	return 1;
}