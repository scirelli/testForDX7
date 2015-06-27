//-----------------------------------------------------------------------------
// File: BasicShapes.cpp
//
// Desc: Code for all basic shape drawing. Like lines circles etc.
//
//-----------------------------------------------------------------------------

////INCLUDES////
#include "CUtilities.h"

////GLOBALS////////////////////

////FUNCTIONS//////////////////

/*
//-----------------------------------------------------------------------------
// Name: DrawPixel()
// Desc: draws a 16bit pixel to a surface
//-----------------------------------------------------------------------------
bool CUtilities::DrawPixel(DDSURFACEDESC2 ddsd, int x, int y, SCCOLOR col)
{
	unsigned short *video_buffer= (unsigned short*)ddsd.lpSurface;
		
	video_buffer[x + (y*ddsd.lPitch>>1)]= (unsigned short)BIULDCOLOR(col.r,col.g,col.b);

	return true;
}

//-----------------------------------------------------------------------------
// Name: DrawPixel(DDSURFACEDESC2 ddsd, int x, int y, int r, int g, int b)
// Desc: draws a 16bit pixel to a surface
//-----------------------------------------------------------------------------
bool CUtilities::DrawPixel(DDSURFACEDESC2 ddsd, int x, int y, int r, int g, int b)
{
	unsigned short *video_buffer= (unsigned short*)ddsd.lpSurface;
	
	video_buffer[x + (y*ddsd.lPitch>>1)]= (unsigned short)BIULDCOLOR((int)r,(int)g,(int)b);

	return true;
}

//-----------------------------------------------------------------------------
// Name: DrawPixel(DDSURFACEDESC2 ddsd, int lPitch, int x, int y, int r, int g, int b)
// Desc: draws a 16bit pixel to a surface
//-----------------------------------------------------------------------------
bool CUtilities::DrawPixel(USHORT *video_buffer, int lPitch, int x, int y, int r, int g, int b)
{
	video_buffer[x + (y*lPitch)]= (unsigned short)BIULDCOLOR((int)r,(int)g,(int)b);
	return true;
}

//-----------------------------------------------------------------------------
// Name: DrawPixel32()
// Desc: draws a 32bit pixel to a surface
//-----------------------------------------------------------------------------
bool CUtilities::DrawPixel32(DDSURFACEDESC2 ddsd, int x, int y, SCCOLOR col)
{
	DWORD *video_buffer= (DWORD*)ddsd.lpSurface;
		
	video_buffer[x + (y*(ddsd.lPitch>>2))]= (DWORD)BIULDCOLOR(col.r,col.g,col.b);

	return true;
}

//-----------------------------------------------------------------------------
// Name: DrawPixel32(DDSURFACEDESC2 ddsd, int x, int y, int r, int g, int b)
// Desc: draws a 32bit pixel to a surface
//-----------------------------------------------------------------------------
bool CUtilities::DrawPixel32(DDSURFACEDESC2 ddsd, int x, int y, int r, int g, int b)
{
	DWORD *video_buffer= (DWORD*)ddsd.lpSurface;
	
	video_buffer[x + (y*(ddsd.lPitch>>2))]= (DWORD)BIULDCOLOR((int)r,(int)g,(int)b);

	return true;
}

//-----------------------------------------------------------------------------
// Name: DrawPixel32(DWORD *video_buffer, int lpitch, int x, int y, int r, int g, int b)
// Desc: draws a 32bit pixel to a surface
//-----------------------------------------------------------------------------
bool CUtilities::DrawPixel32(DWORD *video_buffer, int lPitch, int x, int y, int r, int g, int b)
{	
	video_buffer[x + (y*lPitch)]= (DWORD)BIULDCOLOR((int)r,(int)g,(int)b);
	return true;
}

//-----------------------------------------------------------------------------
// Name: DrawParticle()
// Desc: draws an array of particles
//-----------------------------------------------------------------------------
bool CUtilities::DrawParticle(DDSURFACEDESC2 ddsd, PARTICLE *particle, int nCurrentMaxParticles)
{
	int i=0;

	unsigned short *video_buffer= (unsigned short*)ddsd.lpSurface;
	
	while(i <= nCurrentMaxParticles-1)
	{
		video_buffer[((int)particle[i].x) + (((int)particle[i].y)*ddsd.lPitch>>1)]= (unsigned short)BIULDCOLOR(particle[i].col.r,particle[i].col.g,particle[i].col.b);
		i++;
	}

	return true;
}

//-----------------------------------------------------------------------------
// Name: DrawParticle32()
// Desc: draws an array of particles
//-----------------------------------------------------------------------------
bool CUtilities::DrawParticle32(DDSURFACEDESC2 ddsd, PARTICLE *particle, int nCurrentMaxParticles)
{
	DWORD *video_buffer= (DWORD*)ddsd.lpSurface;
	
	for(int i=0; i<nCurrentMaxParticles; i++)
	{
		video_buffer[((int)particle[i].x) + (((int)particle[i].y)*ddsd.lPitch>>2)]= (DWORD)BIULDCOLOR(particle[i].col.r,particle[i].col.g,particle[i].col.b);
	}
	return true;
}

//-----------------------------------------------------------------------------
// Name: DrawStringGDI()
// Desc: Draws a string to a surface using GDI
//-----------------------------------------------------------------------------
int CUtilities::DrawStringGDI( LPDIRECTDRAWSURFACE7 lpddsSurface, int x, int y, const char *string )
{
	HDC hDC;
	HRESULT ddReturnVal = 0;
	static char cpErrorBuf[100];
	//Write text to the Secondary surface remember to Release the DC
	ddReturnVal = lpddsSurface->GetDC( &hDC );
	if( ddReturnVal != DD_OK )
		if( ddReturnVal == DDERR_SURFACELOST )
			{ OutputDebugString("DrawStringGDI( ) Surface lost\n"); lpddsSurface->Restore( ); lpddsSurface->GetDC( &hDC ); }
		else
			if( DDFailedCheck(ddReturnVal, "GetDC() failed", cpErrorBuf ) )
				{ MessageBox( NULL, cpErrorBuf, "DrawStringGDI()", MB_ICONEXCLAMATION ); return( 0 ); }
	
		//SetBkColor(hDC,RGB(0,0,0));
		SetBkMode( hDC, TRANSPARENT );
		SetTextColor( hDC, RGB(255,255,255) );
		//sprintf(buffer, "Frames/s = %d", "l");
		TextOut( hDC, x, y, string, (int)strlen( string ) );
	
	ddReturnVal = lpddsSurface->ReleaseDC( hDC );
	if( ddReturnVal != DD_OK )
		if( DDFailedCheck( ddReturnVal, "ReleaseDC( ) failed", cpErrorBuf ) )
		{	MessageBox(NULL, cpErrorBuf, "main()", MB_ICONEXCLAMATION ); return( 0 ); }

	return( 1 );
}

//-----------------------------------------------------------------------------
// Name: ClrS()
// Desc: blts a block of the given color to the destination surface. clearing it
//-----------------------------------------------------------------------------
int CUtilities::ClrS( LPDIRECTDRAWSURFACE7 lpddsSurface, RECT *destinationRec, int r, int g, int b )
{
	HRESULT ddReturnVal = 0;
	static char cpErrorBuf[100];
	static DDBLTFX ddbltfx;
	//clear the struct
	memset(&ddbltfx,0,sizeof(DDBLTFX));
	//set the size field
	ddbltfx.dwSize = sizeof(DDBLTFX);
	//set the dwfillcolor field to the desired color; this is in 16bit mode
	ddbltfx.dwFillColor = BIULDCOLOR(r,g,b);
	
	//blt to the surface
	ddReturnVal  = lpddsSurface->Blt(destinationRec,
									 NULL, //pointer to source surface
									 NULL, //pointer to the source rectangle
									 DDBLT_COLORFILL | DDBLT_WAIT,&ddbltfx);
	if( ddReturnVal != DD_OK )
		if( ddReturnVal == DDERR_SURFACELOST )
		{ OutputDebugString("Surfacelost ClrS( )\n"); lpddsSurface->Restore( ); return( 1 ); }
		else
			if( DDFailedCheck( ddReturnVal, "Clrs( ), Blt failed", cpErrorBuf ) )
				{ MessageBox(NULL, cpErrorBuf, "ClrS( )", MB_ICONEXCLAMATION ); return( 0 ); }

	return( 1 );
}
//-----------------------------------------------------------------------------
// Name: ClrS()
// Desc: blts a black block to the destination surface clearing it
//-----------------------------------------------------------------------------
int CUtilities::ClrS(LPDIRECTDRAWSURFACE7 lpddsSurface, RECT *destinationRec)
{
	HRESULT ddReturnVal = 0;
	static char cpErrorBuf[100];
	static DDBLTFX ddbltfx;

	//clear the struct
	memset(&ddbltfx,0,sizeof(DDBLTFX));
	//set the size field
	ddbltfx.dwSize = sizeof(DDBLTFX);
	//set the dwfillcolor field to the desired color; this is in 16bit mode
	ddbltfx.dwFillColor = 0;//or can use this BIULDCOLOR(0,0,0)

	//blt to the surface
	ddReturnVal  = lpddsSurface->Blt(destinationRec,
									 NULL, //pointer to source surface
									 NULL, //pointer to the source rectangle
									 DDBLT_COLORFILL | DDBLT_WAIT,&ddbltfx);
	if( ddReturnVal != DD_OK )
		if( ddReturnVal == DDERR_SURFACELOST )
		{ OutputDebugString( "Surfacelost ClrS(RECT)\n" ); lpddsSurface->Restore( ); return( 1 );	}
		else
			if( DDFailedCheck(ddReturnVal, "ClrS(RECT), Blt failed", cpErrorBuf ))
				{ MessageBox(NULL, cpErrorBuf, "ClrS)()", MB_ICONEXCLAMATION); return( 0 ); }

	return(1);
}

//-----------------------------------------------------------------------------
// Name: DrawFillRect()
// Desc: blts a rect to a surface
//-----------------------------------------------------------------------------
int CUtilities::DrawFillRect(LPDIRECTDRAWSURFACE7 lpddsSurface, int x, int y, int x2, int y2, int r, int g, int b)
{
	RECT destRect={x,y,x2,y2};
	HRESULT ddReturnVal = 0;
	static char cpErrorBuf[100];
	static DDBLTFX ddbltfx;
	//clear the struct
	memset(&ddbltfx,0,sizeof(DDBLTFX));
	//set the size field
	ddbltfx.dwSize = sizeof(DDBLTFX);
	//set the dwfillcolor field to the desired color; this is in 16bit mode
	ddbltfx.dwFillColor = BIULDCOLOR(r,g,b);//or can use this BIULDCOLOR(0,0,0)

	//blt to the surface
	ddReturnVal  = lpddsSurface->Blt(&destRect,
									 NULL, //pointer to source surface
									 NULL, //pointer to the source rectangle
									 DDBLT_COLORFILL | DDBLT_WAIT,&ddbltfx);
	if( ddReturnVal != DD_OK )
		if( ddReturnVal==DDERR_SURFACELOST )
		{ OutputDebugString( "Surfacelost DrawFillRect(x,y,...)\n" ); lpddsSurface->Restore( ); return( 1 ); }
		else
			if( DDFailedCheck(ddReturnVal, "DrawFillRect(), Blt failed", cpErrorBuf ) )
				{ MessageBox(NULL, cpErrorBuf, "ClrS( )", MB_ICONEXCLAMATION ); return( 0 ); }
	return ( 0 );
}

//-----------------------------------------------------------------------------
// Name: DrawFillRect()
// Desc: blts a rect to a surface
//-----------------------------------------------------------------------------
int CUtilities::DrawFillRect(LPDIRECTDRAWSURFACE7 lpddsSurface, RECT destinationRect, SCCOLOR col)
{
	HRESULT ddReturnVal = 0;
	static char cpErrorBuf[100];
	static DDBLTFX ddbltfx;

	//clear the struct
	memset(&ddbltfx,0,sizeof(DDBLTFX));
	//set the size field
	ddbltfx.dwSize = sizeof(DDBLTFX);
	//set the dwfillcolor field to the desired color; this is in 16bit mode
	ddbltfx.dwFillColor = BIULDCOLOR(col.r,col.g,col.b);//or can use this BIULDCOLOR(0,0,0)

	//blt to the surface
	ddReturnVal  = lpddsSurface->Blt(&destinationRect,
									 NULL, //pointer to source surface
									 NULL, //pointer to the source rectangle
									 DDBLT_COLORFILL | DDBLT_WAIT,&ddbltfx);
	if( ddReturnVal != DD_OK )
		if( ddReturnVal == DDERR_SURFACELOST )
		{ OutputDebugString( "Surfacelost DrawFilledRect(SCCOLOR), blt failed\n" ); lpddsSurface->Restore( );	return( 1 ); }
		else
			if( DDFailedCheck( ddReturnVal, "DrawFillRect(SCCOLOR), Blt failed", cpErrorBuf ) )
				{	MessageBox( NULL, cpErrorBuf, "ClrS( )", MB_ICONEXCLAMATION ); return( 0 ); }
	return ( 0 );
}

//-----------------------------------------------------------------------------
// Name: DrawLine()
// Desc: draws an unclipped line to a surface
//-----------------------------------------------------------------------------
int CUtilities::DrawLine(DDSURFACEDESC2 ddsd, int xo, int yo, int x1,int y1, SCCOLOR col)
{
	// this function draws a line from xo,yo to x1,y1 using differential error
	// terms (based on Bresenahams work)

	int dx,             // difference in x's
		dy,             // difference in y's
		x_inc,          // amount in pixel space to move during drawing
		y_inc,          // amount in pixel space to move during drawing
		error=0,        // the discriminant i.e. error i.e. decision variable
		index;          // used for looping
	USHORT color = BIULDCOLOR(col.r,col.g,col.b);
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
*/