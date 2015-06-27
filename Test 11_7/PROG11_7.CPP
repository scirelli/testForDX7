// PROG11_7.CPP - Windowed DirectDraw demo
// works in any mode 8/16/24

// INCLUDES ///////////////////////////////////////////////
#define WIN32_LEAN_AND_MEAN  
//#define INITGUID

#include <windows.h>   // include important windows stuff
#include <windowsx.h> 
#include <mmsystem.h>
#include <iostream.h>  // include important C/C++ stuff
#include <conio.h>
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include <math.h>
#include <io.h>
#include <fcntl.h>

#include <ddraw.h>    // directX includes

// DEFINES ////////////////////////////////////////////////

// defines for windows 
#define WINDOW_CLASS_NAME "WINXCLASS"  // class name

#define WINDOW_WIDTH  320              // size of window
#define WINDOW_HEIGHT 240
#define SCREEN_WIDTH  640              // size of screen
#define SCREEN_HEIGHT 480
#define SCREEN_BPP    16               // bits per pixel

// MACROS /////////////////////////////////////////////////

// these read the keyboard asynchronously
#define KEY_DOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)
#define KEY_UP(vk_code)   ((GetAsyncKeyState(vk_code) & 0x8000) ? 0 : 1)

// this builds a 16 bit color value in 5.5.5 format (1-bit alpha mode)
#define _RGB16BIT555(r,g,b) ((b & 31) + ((g & 31) << 5) + ((r & 31) << 10))

// this builds a 16 bit color value in 5.6.5 format (green dominate mode)
#define _RGB16BIT565(r,g,b) ((b & 31) + ((g & 63) << 5) + ((r & 31) << 11))

// this builds a 24 bit color value in 8.8.8 format 
#define _RGB24BIT(r,g,b) ((b) + ((g) << 8) + ((r) << 16) )

// this builds a 32 bit color value in A.8.8.8 format (8-bit alpha mode)
// this is the most common format of all new video cards
#define _RGB32BIT(a,r,g,b) ((b) + ((g) << 8) + ((r) << 16) + ((a) << 24))

// TYPES //////////////////////////////////////////////////

typedef unsigned short USHORT;
typedef unsigned short WORD;
typedef unsigned char  UCHAR;
typedef unsigned char  BYTE;

// PROTOTYPES /////////////////////////////////////////////

int Game_Init(void *parms=NULL);
int Game_Shutdown(void *parms=NULL);
int Game_Main(void *parms=NULL);

// GLOBALS ////////////////////////////////////////////////

HWND main_window_handle = NULL; // save the window handle
HINSTANCE main_instance = NULL; // save the instance
char buffer[80];                // used to print text

LPDIRECTDRAW7        lpdd         = NULL;  // dd object
LPDIRECTDRAWSURFACE7 lpddsprimary = NULL;  // dd primary surface
LPDIRECTDRAWSURFACE7 lpddsback    = NULL;  // dd back surface
LPDIRECTDRAWPALETTE  lpddpal      = NULL;  // a pointer to the created dd palette
DDSURFACEDESC2       ddsd;                 // a direct draw surface description struct
DDSCAPS2             ddscaps;              // a direct draw surface capabilities struct
HRESULT              ddrval;               // result back from dd calls
         
// FUNCTIONS //////////////////////////////////////////////

LRESULT CALLBACK WindowProc(HWND hwnd, 
						    UINT msg, 
                            WPARAM wparam, 
                            LPARAM lparam)
{
// this is the main message handler of the system
PAINTSTRUCT	ps;		   // used in WM_PAINT
HDC			hdc;	   // handle to a device context

// what is the message 
switch(msg)
	{	
	case WM_CREATE: 
        {
		// do initialization stuff here
		return(0);
		} break;

    case WM_PAINT:
         {
         // start painting
         hdc = BeginPaint(hwnd,&ps);

         // end painting
         EndPaint(hwnd,&ps);
         return(0);
        } break;

	case WM_DESTROY: 
		{
		// kill the application			
		PostQuitMessage(0);
		return(0);
		} break;

	default:break;

    } // end switch

// process any messages that we didn't take care of 
return (DefWindowProc(hwnd, msg, wparam, lparam));

} // end WinProc

// WINMAIN ////////////////////////////////////////////////

int WINAPI WinMain(	HINSTANCE hinstance,
					HINSTANCE hprevinstance,
					LPSTR lpcmdline,
					int ncmdshow)
{

WNDCLASS winclass;	// this will hold the class we create
HWND	 hwnd;		// generic window handle
MSG		 msg;		// generic message
HDC      hdc;       // generic dc
PAINTSTRUCT ps;     // generic paintstruct

// first fill in the window class stucture
winclass.style			= CS_DBLCLKS | CS_OWNDC | 
                          CS_HREDRAW | CS_VREDRAW;
winclass.lpfnWndProc	= WindowProc;
winclass.cbClsExtra		= 0;
winclass.cbWndExtra		= 0;
winclass.hInstance		= hinstance;
winclass.hIcon			= LoadIcon(NULL, IDI_APPLICATION);
winclass.hCursor		= LoadCursor(NULL, IDC_ARROW);
winclass.hbrBackground	= (HBRUSH)GetStockObject(BLACK_BRUSH);
winclass.lpszMenuName	= NULL; 
winclass.lpszClassName	= WINDOW_CLASS_NAME;

// register the window class
if (!RegisterClass(&winclass))
	return(0);

// create the window, note the use of WS_POPUP
if (!(hwnd = CreateWindow(WINDOW_CLASS_NAME, // class
						  "WinX Game Console",	 // title
						  WS_OVERLAPPED | WS_VISIBLE,
					 	  200,200,	   // x,y
						  WINDOW_WIDTH,  // width
                          WINDOW_HEIGHT, // height
						  NULL,	   // handle to parent 
						  NULL,	   // handle to menu
						  hinstance,// instance
						  NULL)))	// creation parms
return(0);

// save the window handle and instance in a global
main_window_handle = hwnd;
main_instance      = hinstance;

// perform all game console specific initialization
Game_Init();

// enter main event loop
while(1)
	{
	if (PeekMessage(&msg,NULL,0,0,PM_REMOVE))
		{ 
		// test if this is a quit
        if (msg.message == WM_QUIT)
           break;
	
		// translate any accelerator keys
		TranslateMessage(&msg);

		// send the message to the window proc
		DispatchMessage(&msg);
		} // end if
    
    // main game processing goes here
    Game_Main();

	} // end while

// shutdown game and release all resources
Game_Shutdown();

// return to Windows like this
return(msg.wParam);

} // end WinMain

// WINX GAME PROGRAMMING CONSOLE FUNCTIONS ////////////////

int Game_Init(void *parms)
{
// this function is where you do all the initialization 
// for your game

// create object and test for error
if (DirectDrawCreateEx(NULL, (void **)&lpdd, IID_IDirectDraw7, NULL)!=DD_OK)
   return(0);

// set cooperation level to windowed mode normal
if (lpdd->SetCooperativeLevel(main_window_handle, DDSCL_NORMAL)!=DD_OK)
    return(0);

// Create the primary surface
memset(&ddsd,0,sizeof(ddsd));
ddsd.dwSize         = sizeof(ddsd);
ddsd.dwFlags        = DDSD_CAPS;
ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;

if (lpdd->CreateSurface(&ddsd,&lpddsprimary,NULL)!=DD_OK)
   return(0);

// return success
return(1);

} // end Game_Init

///////////////////////////////////////////////////////////

int Game_Shutdown(void *parms)
{
// this function is where you shutdown your game and
// release all resources that you allocated

// first release the primary surface
if (lpddsprimary!=NULL)
   lpddsprimary->Release();
       
// release the directdraw object
if (lpdd!=NULL)
   lpdd->Release();

// return success
return(1);
} // end Game_Shutdown

///////////////////////////////////////////////////////////

int Game_Main(void *parms)
{
// this is the workhorse of your game it will be called
// continuously in real-time this is like main() in C
// all the calls for you game go here!

UCHAR *primary_buffer = NULL; // used to draw
DDPIXELFORMAT ddpixelformat;  // used to determine bits per pixel
RECT client;                  // the client direct draw rectangle
     
// check of user is trying to exit
if (KEY_DOWN(VK_ESCAPE) || KEY_DOWN(VK_SPACE))
    PostMessage(main_window_handle, WM_DESTROY,0,0);

// get client rectangle first
GetWindowRect(main_window_handle, &client);
client.top += 20;
client.left += 5;
client.bottom -= 5;
client.right -= 5;
// copy the double buffer into the primary buffer
memset(&ddsd,0,sizeof(ddsd)); 
ddsd.dwSize = sizeof(ddsd);

// lock the primary surface
lpddsprimary->Lock(NULL,&ddsd, 
              DDLOCK_SURFACEMEMORYPTR | DDLOCK_WAIT,NULL);

// get video pointer to primary surfce
primary_buffer = (UCHAR *)ddsd.lpSurface;       

// test if this is a 8 bit or 16 bit video mode
memset(&ddpixelformat,0, sizeof(ddpixelformat));
ddpixelformat.dwSize = sizeof(ddpixelformat);

// get the pixel format
lpddsprimary->GetPixelFormat(&ddpixelformat);

// perform test to see what mode, 8/16/32
if (ddpixelformat.dwRGBBitCount==32)
    {
    // draw 10 random pixels in 32 bit mode
    for (int index=0; index < 10; index++)
        {
        int x=rand()%(client.right - client.left) + client.left;
        int y=rand()%(client.bottom - client.top) + client.top;
        DWORD color = _RGB32BIT(0,rand()%256, rand()%256, rand()%256);
        *((DWORD *)(primary_buffer + x*4 + y*ddsd.lPitch)) = color;
        } // end for index
    } // end if 32 bit
if (ddpixelformat.dwRGBBitCount==24)
    {
    // draw 10 random pixels in 24 bit mode
    for (int index=0; index < 10; index++)
        {
        int x=rand()%(client.right - client.left) + client.left;
        int y=rand()%(client.bottom - client.top) + client.top;
        UCHAR red   = rand()%256;
        UCHAR green = rand()%256;
        UCHAR blue  = rand()%256;
        
        // write date a byte at a time
        *(primary_buffer + x*3 + y*ddsd.lPitch + 0) = red;
        *(primary_buffer + x*3 + y*ddsd.lPitch + 1) = green;
        *(primary_buffer + x*3 + y*ddsd.lPitch + 2) = blue;

        } // end for index
    } // end if 32 bit
if (ddpixelformat.dwRGBBitCount==16)
    {
    // draw 10 random pixels in 16 bit mode
    for (int index=0; index<10; index++)
        {
        int x=rand()%(client.right - client.left) + client.left+100;
        int y=rand()%(client.bottom - client.top) + client.top-100;
        USHORT color = _RGB16BIT565(rand()%256, rand()%256, rand()%256);
        *((USHORT *)(primary_buffer + x*2 + y*ddsd.lPitch)) = color;
        } // end for index
    } // end if 16 bit
else
    {// assume 8 bits per pixel
    // draw 10 random pixels in 8 bit mode
    for (int index=0; index<10; index++)
        {
        int x=rand()%(client.right - client.left) + client.left;
        int y=rand()%(client.bottom - client.top) + client.top;
        UCHAR color = rand()%256;
        primary_buffer[x + y*ddsd.lPitch] = color;
        } // end for index
    } // end else

// unlock primary buffer
lpddsprimary->Unlock(NULL);

// wait a sec
Sleep(30);

// return success
return(1);
} // end Game_Main

///////////////////////////////////////////////////////////

