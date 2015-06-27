//-----------------------------------------------------------------------------
// File: CreateRGB.h
//
// Desc: Header file for createRGB.cpp
//
//-----------------------------------------------------------------------------
#ifndef CreateRGB_h
#define CreateRGB_h

////INCLUDES///////////////////////////////
#include "General.h"

//-----------------------------------------------------------------------------
// Defines, constants, and global variables
//-----------------------------------------------------------------------------
#define _RGB16BIT(r,g,b) ((b%32)+((g%32)<<5)+((r%32)<<10))
#define _RGB16BIT555(r,g,b) ((b&31)+((g&31)<<5)+((r&31)<<10))
// 99% of all video cards use 5-6-5 formatt
//((b&31)+((g&63)<<5)+((r&31)<<11))
#define BIULDCOLOR(r,g,b) ( b + (g<<8) + (r<<16) + (0<<24) )
#define _ARGB24BIT(a,r,g,b) ( b + (g<<8) + (r<<16) + (a<<24) )
#define _RGBA32BIT(r,g,b,a) ( (a) + (b<<8) + (g<<16) + (r<<24) )
/*
#if BPP == 16
	#define BIULDCOLOR(r,g,b) ((b&31)+((g&63)<<5)+((r&31)<<11))
#else
	#if BPP == 32
		#define BIULDCOLOR(r,g,b) ( b + (g<<8) + (r<<16) + (0<<24) )
	#endif
#endif
*/

#endif
