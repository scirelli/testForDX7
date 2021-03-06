//-----------------------------------------------------------------------------
// File: BasicShapes.h
//
// Desc: Code for all basic shape drawing. Like lines circles etc.
//
//-----------------------------------------------------------------------------
#include <ddraw.h>
#include "General.h"
#include "CreateRGB.h"
#include "SCMath.h"
#include "ErrorCheck.h"

#ifndef BasicShapes_h
#define BasicShapes_h

extern LPDIRECTDRAWSURFACE7 lpddsSecondary;
extern RECT                 wndRect;

////STRUCTURES/////////////////
typedef struct PIXEL
{
	CVECTOR velVector();
	float x;
	float y;
	float vx;
	float vy;
	float vx0;
	float vy0;
	float initvel;
	float angle;
	float gravity;
	int   fadefactor;    //duration in milliseconds or units
	int   r,g,b;
	SCCOLOR col;
	bool Active;   //is this particle dead or alive
}PARTICLE, *PTR_PARTICLE;

typedef struct _tParticalExplosion
{
	PARTICLE  particle[MAX_PARTICLES]; //list of particles making up this effect
	float initVel;  //initial vel, or strength, of effect
	float fStartTime;
	int   x;        //initial x location
	int   y;        //initial y location
	bool  Active;   //is effect active or dead
} tParticalExplotion;

//16 bit
bool DrawPixel(DDSURFACEDESC2 ddsd, int x, int y, SCCOLOR col);
bool DrawPixel(DDSURFACEDESC2 ddsd, int x, int y, int r, int g, int b);
bool DrawPixel(USHORT *video_buffer, int lPitch, int x, int y, int r, int g, int b);
bool DrawPixel32(DDSURFACEDESC2 ddsd, int x, int y, SCCOLOR col);
bool DrawPixel32(DDSURFACEDESC2 ddsd, int x, int y, int r, int g, int b);
bool DrawPixel32(DWORD *video_buffer, int lPitch, int x, int y, int r, int g, int b);
bool DrawParticle(DDSURFACEDESC2 ddsd, PARTICLE*, int);
bool DrawParticle32(DDSURFACEDESC2 ddsd, PARTICLE *particle, int nCurrentMaxParticles);
int  DrawStringGDI(LPDIRECTDRAWSURFACE7, int x, int y, const char *string);
int  DrawFillRect(LPDIRECTDRAWSURFACE7, int x, int y, int x2, int y2, int r, int g, int b);
int  DrawFillRect(LPDIRECTDRAWSURFACE7, RECT, SCCOLOR);
int  ClrS(LPDIRECTDRAWSURFACE7 lpddsSurface =lpddsSecondary, RECT *destinationRec =&wndRect);
int  ClrS(LPDIRECTDRAWSURFACE7 lpddsSurface, RECT *destinationRec, int r, int g, int b);
int  DrawLine(DDSURFACEDESC2 ddsd, int xo, int yo, int x1,int y1, SCCOLOR col);

#endif