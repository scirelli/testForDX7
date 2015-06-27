//-----------------------------------------------------------------------------
// File: BasicShapes.cpp
//
// Desc: Code for all basic shape drawing. Like lines circles etc.
//
//-----------------------------------------------------------------------------

////INCLUDES////
#include <ddraw.h>
#include "CreateRGB.h"

//16 bit
bool DrawPixel(DDSURFACEDESC2 ddsd, int x, int y, SCCOLOR col)
{
	unsigned short *video_buffer= (unsigned short*)ddsd.lpSurface;
	
	video_buffer[x + (y*ddsd.lPitch>>1)]= (unsigned short)_RGB16BIT565(col.r,col.g,col.b);

	return true;
}