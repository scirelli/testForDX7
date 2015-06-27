//---------------------------------------------------------------------
// Name: General.h
//
// Desc: A gernal header file that contains things that all other 
//       files might need
//---------------------------------------------------------------------

#ifndef General_h
#define General_h

////INCLUDES////////////////////////////////
#include <ddraw.h>
#include <dxerr9.h>
#include "ErrorCheck.h"
#include <tchar.h>
#include "Main.h"

////MACROS//////////////////////////////////
#define KEY_DOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)
#define KEY_UP(vk_code)   ((GetAsyncKeyState(vk_code) & 0x8000) ? 0 : 1)
#define DDRAW_INIT_STRUCT( ddStruct ) { memset(&ddStruct,0,sizeof(ddStruct)); ddStruct.dwSize=sizeof(ddStruct); }

////DEFINES/////////////////////////////////
#define DD_VERSION IID_IDirectDraw7

////CONST///////////////////////////////////
const int WINDOW_WIDTH      = 640;
const int WINDOW_HEIGHT     = 480;
const int BPP               = 32;
const int MAX_PARTICLES     = 100;
const int BACK_BUFFER_COUNT = 1;

//-----------------------------------------------------------------------------
// Name : CDDSettings (Support Class)
// Desc : Allows us to set up the various options we will be using.
//-----------------------------------------------------------------------------
class CDDSettings 
{
public:
    
    struct Settings
    {
       int			  nBPP;
	   int			  nWindowHeight;
	   int			  nWindowWidth;
	   int            nBackBufferHeight;
	   int            nBackBufferWidth;
	   DWORD          dwBackBufferCount;
	   RECT           wndRect;
	   DDSCAPS2       ddscaps;   // a direct draw surface capabilities struct
	   DDBLTFX        ddbltfx;   // The Blit Fx structure used for all blting
	   DDSURFACEDESC2 ddsd;
	   float          TransColorR,TransColorG,TransColorB;//{255,0,255};//The transparent color for all bitmaps

	   Settings() : nBPP(BPP), nWindowHeight(WINDOW_HEIGHT), nWindowWidth(WINDOW_WIDTH), nBackBufferHeight(WINDOW_HEIGHT), nBackBufferWidth(WINDOW_WIDTH), dwBackBufferCount(BACK_BUFFER_COUNT)
	   {
		   wndRect.top = wndRect.left = 0;
		   wndRect.bottom = WINDOW_HEIGHT; wndRect.right = WINDOW_WIDTH;
		   DDRAW_INIT_STRUCT(ddbltfx);
		   DDRAW_INIT_STRUCT(ddsd);
           TransColorR = TransColorB = 255;
           TransColorG = 0;
		   memset(&ddscaps,0,sizeof(ddscaps));
	   }
    };

    bool        Windowed;
    Settings    Windowed_Settings;
    Settings    Fullscreen_Settings;
	
	CDDSettings() : Windowed(false) {}
    Settings*   GetSettings() { return (Windowed) ? &Windowed_Settings : &Fullscreen_Settings; }

};

#endif