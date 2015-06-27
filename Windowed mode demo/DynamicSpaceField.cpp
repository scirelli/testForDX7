//-----------------------------------------------------------------------------
// File: DynamicStarField.cpp
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
#define L1_FADE_AMOUNT 0
#define L2_FADE_AMOUNT 100
#define L3_FADE_AMOUNT 169
#define L1_PERCENT_OF_STARS 0.1f
#define L2_PERCENT_OF_STARS 0.4f
#define L3_PERCENT_OF_STARS 0.5f
#define L1_SPEED_REDUC_PERCENT 0.0f
#define L2_SPEED_REDUC_PERCENT 0.5f
#define L3_SPEED_REDUC_PERCENT 0.1f

////GLOBALS////////////////////
extern LPDIRECTDRAWSURFACE7 lpddsSecondary;  // dd back surface
extern DDSURFACEDESC2       ddsd;            // a direct draw surface description struct
extern HRESULT              ddReturnVal;     // result back from dd calls
extern DDBLTFX              ddbltfx;
extern char                 cpErrorBuf[1024];
extern RECT                 wndRect; 
extern HWND					main_window_handle;
extern CTIMER2              frameTimer,gameTimer2;
extern CTIMER               gameTimer, frameTimer2;
extern float                fFrameTime;
extern bool                 bDebugMode;
extern BOOL                 bActiveApp;
extern RECT                 wndRect;

////PROTOTYPES/////////////////
int  InitStars(PARTICLE*, RECT*, RECT&, int);
int  DrawStars(const PARTICLE*, int);
int  MoveStars(PARTICLE*, int, RECT, CVECTOR);
void DebugInfo(const PARTICLE*, int nFrame, int nNumofStars, RECT);

////FUNCTIONS//////////////////

//-----------------------------------------------------------------------------
// Name: ClearUpStarField()
// Desc: deletes any allocated space created for the starfield
//-----------------------------------------------------------------------------
void CleanUpStarField(PARTICLE *stars)
{
	if(stars != NULL)
	{
		delete [] stars;
		stars = NULL;
	}
}

//---------------------------------------------------------------
// Name: DynamicStarField()
// Desc:
//---------------------------------------------------------------
int DynamicStarField(RECT *starField, int nNumofStars, CVECTOR velVec, int nFrame)
{
	static PARTICLE *stars = new PARTICLE[nNumofStars];
	static RECT tempField = {0,0,1,1};
	
	//reverse the velocity vector so the stars move in the oppsoite direction as the ship
	velVec.Reverse();
	//velVec.y = -velVec.y;
	
	//init the stars
	if(!nFrame) //if first frame
		InitStars(stars, starField, tempField, nNumofStars);
	
	DrawStars(stars, nNumofStars);
	MoveStars(stars, nNumofStars, tempField, velVec);

	if(bDebugMode)
		DebugInfo(stars, nFrame, nNumofStars, tempField);

	return(1);
}

//---------------------------------------------------------------
// Name: InitStars()
// Desc: initiates the stars position and color
//---------------------------------------------------------------
inline int InitStars(PARTICLE *stars, RECT *starField, RECT &tempField, int nNumofStars)
{
	//create the field to draw the stars in
	if(starField == NULL) //if the user dosn't specify a size use the whole screen
	{
		tempField.top = 0; tempField.left = 0;
		tempField.bottom = wndRect.bottom; tempField.right = wndRect.right;
	}
	else //else use the size specified
	{
		tempField.top = starField->top; tempField.left = starField->left;
		tempField.bottom = starField->bottom; tempField.right = starField->right;
	}

	for(int i=0; i<nNumofStars; i++)
	{
		stars[i].Active = true;
		stars[i].angle = 0;
		stars[i].gravity = 0;
		stars[i].initvel = 0;
		stars[i].vx = stars[i].vy = stars[i].vx0 = stars[i].vy0 = 0;
		
		//----Fade the stars-----
		if( i<=(L1_PERCENT_OF_STARS*nNumofStars) )
		{	stars[i].r = stars[i].g = stars[i].b = 255-L1_FADE_AMOUNT; stars[i].fadefactor =L1_FADE_AMOUNT; }
		else
			if( (i>(L1_PERCENT_OF_STARS*nNumofStars)) && ( i<= L2_PERCENT_OF_STARS*nNumofStars) )
			{	stars[i].r = stars[i].g = stars[i].b = 255-L2_FADE_AMOUNT; stars[i].fadefactor =L2_FADE_AMOUNT;}
			else
				if( i>L2_PERCENT_OF_STARS*nNumofStars )
				{	stars[i].r = stars[i].g = stars[i].b = 255-L3_FADE_AMOUNT; stars[i].fadefactor =L3_FADE_AMOUNT;}
		//----End Fade the stars----
		
		//----Place the stars at random positions----
		stars[i].x = tempField.left + ( (rand()/(float)RAND_MAX)*(tempField.right-tempField.left) );
		if( stars[i].x > tempField.right ) //check to make sure that they are not off the screen
			stars[i].x = (float)tempField.right;
		else
			if( stars[i].x < tempField.left )
				stars[i].x = (float)tempField.left;
		
		stars[i].y = tempField.top + ( (rand()/(float)RAND_MAX)*(tempField.bottom-tempField.top) );
		if( stars[i].y > tempField.bottom ) //check to make sure that they are not off the screen
			stars[i].y = (float)tempField.bottom;
		else
			if( stars[i].y < tempField.top )
				stars[i].y = (float)tempField.top;
		//----End Place the stars at random positions----
	}
	return(1);
}

//---------------------------------------------------------------
// Name: DrawStars()
// Desc: Draws the stars to the screen
//---------------------------------------------------------------
inline int DrawStars(const PARTICLE *stars, int nTotalAmountofStars)
{
	//----Do pixel animation here----
	//Lock the surface for drawing
	if(!Lock(lpddsSecondary))
		return(0);
	
	for(int i=0; i<nTotalAmountofStars; i++)
		DrawPixel32(ddsd,(int)stars[i].x,(int)stars[i].y, stars[i].r,stars[i].g,stars[i].b);

	//Unlock the surface
	if(!UnLock(lpddsSecondary))
		return(0);
	//----End do pixel animation here----
	return 1;
}

//---------------------------------------------------------------
// Name: MoveStars()
// Desc: changes the stars direction and velocity if needed
//---------------------------------------------------------------
inline int MoveStars(PARTICLE* stars, int nNumofStars, RECT field, CVECTOR velVec)
{
	for(int i=0; i<nNumofStars; i++)
	{
		if(stars[i].fadefactor == L3_FADE_AMOUNT)
		{
			stars[i].x += velVec.x*L3_SPEED_REDUC_PERCENT*fFrameTime;
			stars[i].y += velVec.y*L3_SPEED_REDUC_PERCENT*fFrameTime;
		}
		else
			if(stars[i].fadefactor == L2_FADE_AMOUNT)
			{
				stars[i].x += velVec.x*L2_SPEED_REDUC_PERCENT*fFrameTime;
				stars[i].y += velVec.y*L2_SPEED_REDUC_PERCENT*fFrameTime;
			}
			else
			{
				stars[i].x += velVec.x*fFrameTime;
				stars[i].y += velVec.y*fFrameTime;
			}
		if( stars[i].x >= field.right-1 ) //check to make sure that they are not off the screen
			stars[i].x = (float)field.left; //if so then move them to other side of the screen
		else
			if( stars[i].x <= field.left )
				stars[i].x = (float)field.right-1;
		if( stars[i].y >= field.bottom ) //check to make sure that they are not off the screen
			stars[i].y = (float)field.top;//if so then move them to other side of the screen
		else
			if( stars[i].y <= field.top )
				stars[i].y = (float)field.bottom-1;
	}
	
	return(1);
}

//---------------------------------------------------------------
// Name: DebugInfo()
// Desc: output any info i want to keep track of
//---------------------------------------------------------------
inline void DebugInfo(const PARTICLE *stars, int nFrame, int nNumofStars, RECT field)
{
	sprintf(cpErrorBuf,"Frame# %d", nFrame);
	DrawStringGDI(lpddsSecondary,0,30,cpErrorBuf);
	sprintf(cpErrorBuf,"Init x[0]= %d", (int)stars[0].x);
	DrawStringGDI(lpddsSecondary,0,45,cpErrorBuf);
	sprintf(cpErrorBuf,"Init y[0]= %d", (int)stars[0].y);
	DrawStringGDI(lpddsSecondary,0,60,cpErrorBuf);
	sprintf(cpErrorBuf,"Init velX0= %g", stars[0].vx0);
	DrawStringGDI(lpddsSecondary,0,75,cpErrorBuf);
	sprintf(cpErrorBuf,"Init velY0= %g", stars[0].vy0);
	DrawStringGDI(lpddsSecondary,0,90,cpErrorBuf);
	sprintf(cpErrorBuf,"# of L1 stars= %g", nNumofStars*L1_PERCENT_OF_STARS);
	DrawStringGDI(lpddsSecondary,0,105,cpErrorBuf);
	sprintf(cpErrorBuf,"# of L2 stars= %g", nNumofStars*L2_PERCENT_OF_STARS);
	DrawStringGDI(lpddsSecondary,0,120,cpErrorBuf);
	sprintf(cpErrorBuf,"# of L3 stars= %g", nNumofStars*L3_PERCENT_OF_STARS);
	DrawStringGDI(lpddsSecondary,0,135,cpErrorBuf);
	sprintf(cpErrorBuf,"Rect= %d,%d,%d,%d", field.left,field.top,field.right,field.bottom);
	DrawStringGDI(lpddsSecondary,0,150,cpErrorBuf);
}