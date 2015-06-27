//-----------------------------------------------------------------------------
// File: General.h
//
// Desc: holds prototypes and common includes
//
//-----------------------------------------------------------------------------

#ifndef general_h
#define general_h

////INCLUDES////////////////////////////////
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

#include <ddraw.h>

#include "ErrorCheck.h"
//#include "SCMath.h"
//#include "CreateRGB.h"
//#include "Bitmap.h"
//#include "BasicShapes.h"

////DEFINES/////////////////////////////////
//defines for windows
#define WINDOW_CLASS_NAME "WINCLASS1"  // class name
#define WINDOW_WIDTH  1280             // size of window
#define WINDOW_HEIGHT 1024
#define BPP 16
#define MAX_PARTICLES 9000

////MACROS//////////////////////////////////
#define KEY_DOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)
#define KEY_UP(vk_code)   ((GetAsyncKeyState(vk_code) & 0x8000) ? 0 : 1)

////PROTOTYPES//////////////////////////////
bool GetKeyStatus(int);
bool main();


#endif