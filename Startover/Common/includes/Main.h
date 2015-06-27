//-----------------------------------------------------------------------------
// File: Main.h
//
//
// Desc: Main application entry / handling header file.
//-----------------------------------------------------------------------------

#ifndef _MAIN_H_
#define _MAIN_H_

/////SYSTEM DEFINES/////////////////////////////////
#define WIN32_LEAN_AND_MEAN

//-----------------------------------------------------------------------------
// Main Application Includes
//-----------------------------------------------------------------------------
#include "resource.h"
#include <windows.h>
#include <tchar.h>

//System includes
#include <windows.h>
#include <windowsx.h> 

#include <math.h>
#include <mmsystem.h>
#include <iostream.h>  // include important C/C++ stuff
#include <conio.h>
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include <io.h>
#include <fcntl.h>
//Direct Draw includes
#include <ddraw.h>
#include <dxerr9.h>

//-----------------------------------------------------------------------------
// Forward Declarations
//-----------------------------------------------------------------------------
class CDDGameApp;

//-----------------------------------------------------------------------------
// Miscellaneous Defines, Macros and Constants
//-----------------------------------------------------------------------------
#define RANDOM_COLOR 0xFF000000 | ((rand() * 0xFFFFFF) / RAND_MAX)

//-----------------------------------------------------------------------------
// Global Functions - Main.cpp
//-----------------------------------------------------------------------------
CDDGameApp * GetGameApp();

#endif // _MAIN_H_