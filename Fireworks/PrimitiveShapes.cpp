//-----------------------------------------------------------------------------
// File: BasicShapes.cpp
//
// Desc: Code for all basic shape drawing. Like lines circles etc.
//
//-----------------------------------------------------------------------------

////INCLUDES////
#include <ddraw.h>
#include "General.h"
#include "CreateRGB.h"
#include "ErrorCheck.h"
#include "PrimitiveShapes.h"

////GLOBALS////////////////////
extern DDSURFACEDESC2       ddsd;            // a direct draw surface description struct
extern HRESULT              ddReturnVal;     // result back from dd calls
extern DDBLTFX              ddbltfx;
extern char                 cpErrorBuf[1024];
extern RECT                 wndRect; 
extern HWND					main_window_handle;

////FUNCTIONS//////////////////

//-----------------------------------------------------------------------------
// Name: DrawPixel()
// Desc: draws a 16bit pixel to a surface
//-----------------------------------------------------------------------------
bool DrawPixel(DDSURFACEDESC2 ddsd, int x, int y, SCCOLOR col)
{
	unsigned short *video_buffer= (unsigned short*)ddsd.lpSurface;
	
	if(x >= WINDOW_WIDTH-2)
		x = WINDOW_WIDTH-2;
	if(x <= 2)
		x = 2;
	if(y >= WINDOW_HEIGHT-2)
		y = WINDOW_HEIGHT-2;
	if(y <= 2)
		y = 2;
	
	if(col.r > 255)	col.r=255;
	if(col.g > 255)	col.g=255;
	if(col.b > 255)	col.b=255;
	if(col.r < 0)	col.r=0;
	if(col.g < 0)	col.g=0;
	if(col.b < 0)	col.b=0;

	video_buffer[x + (y*ddsd.lPitch>>1)]= (unsigned short)_RGB16BIT565(col.r,col.g,col.b);

	return true;
}
//-----------------------------------------------------------------------------
// Name: DrawPixel(DDSURFACEDESC2 ddsd, int x, int y, int r, int g, int b)
// Desc: draws a 16bit pixel to a surface
//-----------------------------------------------------------------------------
bool DrawPixel(DDSURFACEDESC2 ddsd, int x, int y, int r, int g, int b)
{
	unsigned short *video_buffer= (unsigned short*)ddsd.lpSurface;
	
	
	if(x >= WINDOW_WIDTH-2)
	{ x = WINDOW_WIDTH-2; r=g=b=0;}
	if(x <= 2)
	{ x = 2; r=g=b=0;}
	if(y >= WINDOW_HEIGHT-2)
	{ y = WINDOW_HEIGHT-2; r=g=b=0;}
	if(y <= 2)
	{ y = 2; r=g=b=0;}
	/*
	if(r > 255)	r=255;
	if(g > 255)	g=255;
	if(b > 255)	b=255;
	if(r < 0)	r=0;
	if(g < 0)	g=0;
	if(b < 0)	b=0;
	*/

	video_buffer[x + (y*ddsd.lPitch>>1)]= (unsigned short)_RGB16BIT565((int)r,(int)g,(int)b);

	return true;
}

//-----------------------------------------------------------------------------
// Name: DrawParticle()
// Desc: draws an array of particles
//-----------------------------------------------------------------------------
bool DrawParticle(DDSURFACEDESC2 ddsd, PARTICLE *particle, int nCurrentMaxParticles)
{
	int i=0;

	unsigned short *video_buffer= (unsigned short*)ddsd.lpSurface;
	
	while(i <= nCurrentMaxParticles-1)
	{
		if(particle[i].x >= WINDOW_WIDTH-2)
			particle[i].x = WINDOW_WIDTH-2;
		if(particle[i].x <= 2)
			particle[i].x = 2;
		if(particle[i].y >= WINDOW_HEIGHT-2)
			particle[i].y = WINDOW_HEIGHT-2;
		if(particle[i].y <= 2)
			particle[i].y = 2;
		video_buffer[((int)particle[i].x) + (((int)particle[i].y)*ddsd.lPitch>>1)]= (unsigned short)_RGB16BIT565(particle[i].col.r,particle[i].col.g,particle[i].col.b);
		i++;
	}

	return true;
}

//-----------------------------------------------------------------------------
// Name: DrawStringGDI()
// Desc: Draws a string to a surface using GDI
//-----------------------------------------------------------------------------
int DrawStringGDI(LPDIRECTDRAWSURFACE7 lpddsSurface, int x, int y, const char *string)
{
	HDC hDC;

	//Write text to the Secondary surface remember to Release the DC
	ddReturnVal = lpddsSurface->GetDC(&hDC);
	if (DDFailedCheck(ddReturnVal, "GetDC() failed", cpErrorBuf ))
	{	if(ddReturnVal == DDERR_SURFACELOST)
		{lpddsSurface->Restore();	lpddsSurface->GetDC(&hDC); }
		else
		{ MessageBox(main_window_handle, cpErrorBuf, "main()", MB_ICONEXCLAMATION); return(0); }
	}
	
		//SetBkColor(hDC,RGB(0,0,0));
		SetBkMode(hDC,TRANSPARENT);
		SetTextColor(hDC,RGB(255,255,255));
		//sprintf(buffer, "Frames/s = %d", "l");
		TextOut(hDC, x,y, string, strlen(string));
	
	ddReturnVal = lpddsSurface->ReleaseDC(hDC);
	if (DDFailedCheck(ddReturnVal, "ReleaseDC() failed", cpErrorBuf ))
	{	MessageBox(main_window_handle, cpErrorBuf, "main()", MB_ICONEXCLAMATION); return(0); }

	return(1);
}

//-----------------------------------------------------------------------------
// Name: ClrS()
// Desc: blts a black block to the destination surface clearing it
//-----------------------------------------------------------------------------
int ClrS(LPDIRECTDRAWSURFACE7 lpddsSurface, RECT destinationRec)
{
	//clear the struct
	memset(&ddbltfx,0,sizeof(DDBLTFX));
	//set the size field
	ddbltfx.dwSize = sizeof(DDBLTFX);
	//set the dwfillcolor field to the desired color; this is in 16bit mode
	ddbltfx.dwFillColor = 0;//or can use this _RGB16BIT565(0,0,0)

	//blt to the surface
	ddReturnVal  = lpddsSurface->Blt(&destinationRec,
									 NULL, //pointer to source surface
									 NULL, //pointer to the source rectangle
									 DDBLT_COLORFILL | DDBLT_WAIT,&ddbltfx);
	if (DDFailedCheck(ddReturnVal, "Blt failed", cpErrorBuf ))
	{	
		if(ddReturnVal==DDERR_SURFACELOST)
		{
			lpddsSurface->Restore();
			return(1);
		}
		else
		{
			MessageBox(main_window_handle, cpErrorBuf, "ClsS", MB_ICONEXCLAMATION);   
			return(0); 
		}
	}

	return(1);
}

//-----------------------------------------------------------------------------
// Name: DrawFillRect()
// Desc: blts a rect to a surface
//-----------------------------------------------------------------------------
int DrawFillRect(LPDIRECTDRAWSURFACE7 lpddsSurface, int x, int y, int x2, int y2, int r, int g, int b)
{
	RECT destRect={x,y,x2,y2};

	//clear the struct
	memset(&ddbltfx,0,sizeof(DDBLTFX));
	//set the size field
	ddbltfx.dwSize = sizeof(DDBLTFX);
	//set the dwfillcolor field to the desired color; this is in 16bit mode
	ddbltfx.dwFillColor = _RGB16BIT565(r,g,b);//or can use this _RGB16BIT565(0,0,0)

	//blt to the surface
	ddReturnVal  = lpddsSurface->Blt(&destRect,
									 NULL, //pointer to source surface
									 NULL, //pointer to the source rectangle
									 DDBLT_COLORFILL | DDBLT_WAIT,&ddbltfx);
	if (DDFailedCheck(ddReturnVal, "Blt failed", cpErrorBuf ))
	{	
		if(ddReturnVal==DDERR_SURFACELOST)
		{
			lpddsSurface->Restore();
			return(1);
		}
		else
		{
			MessageBox(main_window_handle, cpErrorBuf, "ClsS", MB_ICONEXCLAMATION);   
			return(0); 
		}
	}
}

//-----------------------------------------------------------------------------
// Name: DrawFillRect()
// Desc: blts a rect to a surface
//-----------------------------------------------------------------------------
int DrawFillRect(LPDIRECTDRAWSURFACE7 lpddsSurface, RECT destinationRect, SCCOLOR col)
{
	//clear the struct
	memset(&ddbltfx,0,sizeof(DDBLTFX));
	//set the size field
	ddbltfx.dwSize = sizeof(DDBLTFX);
	//set the dwfillcolor field to the desired color; this is in 16bit mode
	ddbltfx.dwFillColor = _RGB16BIT565(col.r,col.g,col.b);//or can use this _RGB16BIT565(0,0,0)

	//blt to the surface
	ddReturnVal  = lpddsSurface->Blt(&destinationRect,
									 NULL, //pointer to source surface
									 NULL, //pointer to the source rectangle
									 DDBLT_COLORFILL | DDBLT_WAIT,&ddbltfx);
	if (DDFailedCheck(ddReturnVal, "Blt failed", cpErrorBuf ))
	{	
		if(ddReturnVal==DDERR_SURFACELOST)
		{
			lpddsSurface->Restore();
			return(1);
		}
		else
		{
			MessageBox(main_window_handle, cpErrorBuf, "ClsS", MB_ICONEXCLAMATION);   
			return(0); 
		}
	}
}

//-----------------------------------------------------------------------------
// Name: DrawLine()
// Desc: draws an unclipped line to a surface
//-----------------------------------------------------------------------------
int DrawLine(DDSURFACEDESC2 ddsd, int xo, int yo, int x1,int y1, SCCOLOR col)
{
	// this function draws a line from xo,yo to x1,y1 using differential error
	// terms (based on Bresenahams work)

	int dx,             // difference in x's
		dy,             // difference in y's
		x_inc,          // amount in pixel space to move during drawing
		y_inc,          // amount in pixel space to move during drawing
		error=0,        // the discriminant i.e. error i.e. decision variable
		index;          // used for looping
	USHORT color = _RGB16BIT565(col.r,col.g,col.b);
	UCHAR *vb_start = (UCHAR*)ddsd.lpSurface;
	int lpitch = ddsd.lPitch;
	// pre-compute first pixel address in video buffer
	USHORT *vb_start16 = (USHORT *)vb_start + xo + yo*(lpitch >> 1);

	// compute horizontal and vertical deltas
	dx = x1-xo;
	dy = y1-yo;

	// test which direction the line is going in i.e. slope angle
	if (dx>=0)
	{
		x_inc = 1;

	} // end if line is moving right
	else
	{
		x_inc = -1;
		dx    = -dx;  // need absolute value

	} // end else moving left

	// test y component of slope

	if (dy>=0)
	{
		y_inc = (lpitch >> 1);
	} // end if line is moving down
	else
	{
		y_inc = -(lpitch >> 1);
		dy    = -dy;  // need absolute value

	} // end else moving up

	// now based on which delta is greater we can draw the line
	if (dx>dy)
	{
		// draw the line
		for (index=0; index<=dx; index++)
		{
			// set the pixel
			*vb_start16 = color;

	        // adjust the error term
		    error+=dy;

			// test if error has overflowed
			if (error>dx)
			{
				error-=dx;

				// move to next line
				vb_start16+=y_inc;


			} // end if error overflowed

			 // move to the next pixel
			vb_start16+=x_inc;

		} // end for

	} // end if |slope| <= 1
	else
	{
		// draw the line
		for (index=0; index<=dy; index++)
		{
			// set the pixel
			*vb_start16 = color;

			// adjust the error term
			error+=dx;

			// test if error overflowed
			if (error>0)
			{
				error-=dy;

				// move to next line
				vb_start16+=x_inc;

			} // end if error overflowed

			// move to the next pixel
			vb_start16+=y_inc;

		} // end for

	} // end else |slope| > 1

	// return success
	return(1);

} // end Draw_Line16