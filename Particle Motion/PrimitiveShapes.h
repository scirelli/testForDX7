//-----------------------------------------------------------------------------
// File: BasicShapes.h
//
// Desc: Code for all basic shape drawing. Like lines circles etc.
//
//-----------------------------------------------------------------------------
#include "CreateRGB.h"

#ifndef BasicShapes_h
#define BasicShapes_h

////STRUCTURES/////////////////
typedef struct PIXEL
{
	float x;
	float y;
	float vx;
	float vy;
	SCCOLOR col;
	bool InUse;
}PARTICLE, *PTR_PARTICLE;

//16 bit
bool DrawPixel(DDSURFACEDESC2 ddsd, int x, int y, SCCOLOR col);
bool DrawPixel(DDSURFACEDESC2 ddsd, int x, int y, int r, int g, int b);
bool DrawParticle(DDSURFACEDESC2 ddsd, PARTICLE*, int);
int DrawStringGDI(LPDIRECTDRAWSURFACE7, int x, int y, const char *string);
int DrawFillRect(LPDIRECTDRAWSURFACE7, int x, int y, int x2, int y2, int r, int g, int b);
int DrawFillRect(LPDIRECTDRAWSURFACE7, RECT, SCCOLOR);
int ClrS(LPDIRECTDRAWSURFACE7, RECT);
int DrawLine(DDSURFACEDESC2 ddsd, int xo, int yo, int x1,int y1, SCCOLOR col);

#endif