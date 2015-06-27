//-----------------------------------------------------------------------------
// File: WinMain.cpp
//
// Desc: Creates the window and handles messages
//
//-----------------------------------------------------------------------------

/////SYSTEM DEFINES/////////////////////////////////
#define WIN32_LEAN_AND_MEAN

////INCLUDES////////////////////////////////////////
#include "General.h"
#include "main.h"
#include "resource.h"
#include "Bitmap.h"
#include "SCTimer.h"
#include "CreateRGB.h"
#include "Sprite.h"
#include "PrimitiveShapes.h"

////STRUCTS////////////////////////////////////////

////GLOBALS/////////////////////////////////////////
char*                cpWindowName   = "Test Normal Mode";
HWND                 main_window_handle= NULL;  // save the window handle
HINSTANCE            main_instance  = NULL;  // save the instance
HDC                  hDC            = NULL;  // creates a handle to a device contexts
BOOL                 bActiveApp     = false; // tells whether app is active or not
LPDIRECTDRAW7        lpddObj        = NULL;  // dd object
LPDIRECTDRAWSURFACE7 lpddsPrimary   = NULL;  // dd primary surface
LPDIRECTDRAWSURFACE7 lpddsSecondary = NULL;  // dd back surface for window
LPDIRECTDRAWSURFACE7 lpddsTempSurface[MAX_TEMPSURFACES];  // dd temp surface
LPDIRECTDRAWPALETTE  lpddpal        = NULL;  // a pointer to the created dd palette
DDSURFACEDESC2       ddsd;                   // a direct draw surface description struct
DDSCAPS2             ddscaps;                // a direct draw surface capabilities struct
DDBLTFX              ddbltfx;                // The Blit Fx structure used for all blting
HRESULT              ddReturnVal    = 0;     // result back from dd calls
bool                 iKeyStatus[256];        // keeps track of key status
char                 cpErrorBuf[1024];       // Build up error messages
RECT                 wndRect;                // A rect the size of the window
CTIMER               gametimer;
CTIMER2              gametimer2,frametimer;
SCCOLOR              TransColor     ={255,0,255};//The transparent color for all bitmaps
CSPRITE              Sprites[MAX_SPRITES];
int                  nWindowWidth = WINDOW_WIDTH,nClientOffsetX=5;              // size of window
int                  nWindowHeight = WINDOW_HEIGHT, nClientOffsetY=20;
float                frametime;
int                  iMouse_x, iMouse_y, iWindow_x, iWindow_y;

////PROTOTYPES/////////////////////////////////////
int InitDD();
LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
int WINAPI WinMain(	HINSTANCE hinstance, HINSTANCE hprevinstance, LPSTR lpcmdline, int ncmdshow);
HWND SetupWindow(WNDCLASS *winclass, HINSTANCE hInstance);
int ShutDown();
LPDIRECTDRAWCLIPPER AttachClipper(LPDIRECTDRAWSURFACE7 lpddsSurface, int iNumRects, LPRECT cliplist);
LPDIRECTDRAWCLIPPER AttachClipper(LPDIRECTDRAWSURFACE7 lpddsSurface, HWND hWnd);

/////FUNCTIONS/////////////////////////////////////

//-----------------------------------------------------------------------------
// Name: WindowProc[]()
// Desc: Handles messages for main window
//-----------------------------------------------------------------------------
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{// this is the main message handler of the system

	PAINTSTRUCT	ps;		   // used in WM_PAINT
	
	//find out what the message is
	switch(uMsg)
	{	
	
	case WM_CREATE: //called when window is created
		{// do initialization stuff here
			
			return(0);
		} break;

	case WM_PAINT:
         {
         // start painting
         hDC = BeginPaint(hWnd,&ps);

         // end painting
         EndPaint(hWnd,&ps);
		 ReleaseDC(hWnd, hDC);
		 RECT winRect;

		 return(0);
        } break;
	
	case WM_ACTIVATEAPP: 
		{
			bActiveApp=wParam;
			
			break;
		}
	
	case WM_SIZE:
		{
			nWindowWidth = (int)(short) LOWORD(lParam);   // horizontal position 
			nWindowHeight = (int)(short) HIWORD(lParam);   // vertical position 
			
			wndRect.top    = iWindow_y;
			wndRect.left   = iWindow_x;
			wndRect.bottom = iWindow_y+nWindowHeight;
			wndRect.right  = iWindow_x+nWindowWidth;
			break;
		}
	case WM_MOVE:
		{
			iWindow_x = (int)(short) LOWORD(lParam);   // horizontal position 
			iWindow_y = (int)(short) HIWORD(lParam);   // vertical position 
			
			wndRect.top    = iWindow_y;
			wndRect.left   = iWindow_x;
			wndRect.bottom = iWindow_y+nWindowHeight;
			wndRect.right  = iWindow_x+nWindowWidth;
			break;
		}
		case WM_MOVING:
		{
			wndRect.top    = ((RECT*)lParam)->top;
			wndRect.left   = ((RECT*)lParam)->left;
			wndRect.bottom = ((RECT*)lParam)->bottom;
			wndRect.right  = ((RECT*)lParam)->right;

			iWindow_x = ((RECT*)lParam)->left;
			iWindow_y = ((RECT*)lParam)->top;

			break;
		}
	
	case WM_KEYDOWN:
		{
			iKeyStatus[wParam] = true;
			break;
		}
	case WM_KEYUP:
		{
			iKeyStatus[wParam] = false;
			break;
		}
	case WM_MOUSEMOVE: // whenever the mouse moves this is sent
        {
			static buffer[10];

			// extract x,y and buttons
			iMouse_x = (int)LOWORD(lParam);
			iMouse_y = (int)HIWORD(lParam);
			break;
		}
	case WM_DESTROY: 
		{// kill the application			
			
			//close the program
			PostQuitMessage(0);
			return(0);
		} break;

	default:break;

    } // end main switch

	// process any messages that we didn't take care of 
	return (DefWindowProc(hWnd, uMsg, wParam, lParam));

} // end WinProc

//-----------------------------------------------------------------------------
// Name: WinMain()
// Desc: Creates/regisgters main window
//-----------------------------------------------------------------------------
int WINAPI WinMain(	HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	MSG		 msg;		// generic message
	WNDCLASS winclass;	// this will hold the class created
	bool bDone = true;
	
	
	//Make sure there is no PrevInstance of this program...Ensures only one program is run. Dosn't seem to work.
	if(hPrevInstance) return (0);

	// save the window handle and instance in a global
	main_window_handle = SetupWindow(&winclass, hInstance);
	main_instance      = hInstance;

	ShowWindow(main_window_handle,nCmdShow);//SW_HIDE in place of nCmdShow SW_MINIMIZE SW_SHOW
	//UpdateWindow(main_window_handle);
	SetFocus(main_window_handle);
	//ShowCursor(FALSE);
	
	//change handle to the desktop
	//main_window_handle = GetDesktopWindow();//GetWindow(main_window_handle,GW_OWNER);//GetParent(main_window_handle);//GetDesktopWindow();
	//nWindowWidth = GetSystemMetrics(SM_CXSCREEN);
	//nWindowHeight = GetSystemMetrics(SM_CYSCREEN);

	// perform all game console specific initialization
	InitDD();
	gametimer.StartTimer();
	gametimer2.StartTimer();
	frametime=0;
	frametimer.StartTimer();

	// enter main event loop
	while(bDone)
	{
		if (PeekMessage(&msg,NULL,0,0,PM_REMOVE))
		{ 
			// test if this is a quit message
			if (msg.message == WM_QUIT)
				break;
	
			// translate any accelerator keys
			TranslateMessage(&msg);

			// send the message to the window proc
			DispatchMessage(&msg);
		} // end if
		
		//Get the windows quardinates
		//GetWindowRect(main_window_handle, &wndRect);
		//iWindow_x = wndRect.left; iWindow_y = wndRect.top;

		//if(bActiveApp)//only process frames if window is active
			bDone = main();
		//else
		//	Sleep(10);
	
	} // end while

	ShutDown();

	// return to Windows like this
	return(msg.wParam);

} // end WinMain

//-----------------------------------------------------------------------------
// Name: SetupWindow()
// Desc: Fills in class struct/registers window and returns a window handle
//-----------------------------------------------------------------------------
HWND SetupWindow(WNDCLASS *winclass, HINSTANCE hInstance)
{
	HWND hWnd = NULL;

	//fill in the window class stucture
	winclass->style			= CS_DBLCLKS | CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
	winclass->lpfnWndProc	= WindowProc;
	winclass->cbClsExtra	= 0;
	winclass->cbWndExtra	= 0;
	winclass->hInstance		= hInstance;
	winclass->hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
	winclass->hCursor		= LoadCursor(NULL, IDC_ARROW);
	winclass->hbrBackground	= (HBRUSH) GetStockObject(GRAY_BRUSH);//GRAY_BRUSH
	winclass->lpszMenuName	= NULL; //MAKEINTRESOURCE(IDR_MENU1);
	winclass->lpszClassName	= WINDOW_CLASS_NAME;

	// register the window class
	if (!RegisterClass(winclass)) //RegisterClass(&winclass)
	{
		MessageBox(NULL, "Window Could not be registered.", "Error Registering window", MB_ICONEXCLAMATION); //NULL is default for Error
		return(0);
	}
	
	
	// create the window
	if (!(hWnd = CreateWindow(WINDOW_CLASS_NAME, // class
							  cpWindowName,	 // title
							  WS_OVERLAPPEDWINDOW | WS_VISIBLE,  //WS_POPUP | WS_VISIBLE, WS_OVERLAPPEDWINDOW,WS_OVERLAPPED, NULL for no window
						 	  0,0,	   // x,y
							  nWindowWidth,  // width
						      nWindowHeight, // height
							  NULL,	   // handle to parent 
							  NULL,	   // handle to menu
							  hInstance,// instance
							  NULL)))	// creation parms
	{
		MessageBox(hWnd, "Window Could not be Created", NULL, MB_ICONEXCLAMATION); //NULL is default for Error
		return(0);
	}
	
	return hWnd;
}

//-----------------------------------------------------------------------------
// Name: Init()
// Desc: Initializes Driect draw and anything that needs to be inited
//-----------------------------------------------------------------------------
int InitDD()
{// this function is where i do all the initialization 
 // for the game
	
	// create dd object and test for error
	ddReturnVal = DirectDrawCreateEx(NULL, (void **)&lpddObj, IID_IDirectDraw7, NULL);
	if (DDFailedCheck(ddReturnVal, "DirectDrawCreateEx() failed", cpErrorBuf ))
	{	MessageBox(main_window_handle, cpErrorBuf, "Init()", MB_ICONEXCLAMATION);   return(0); }
	
	// set cooperation level to windowed mode normal
	ddReturnVal = lpddObj->SetCooperativeLevel(main_window_handle, DDSCL_NORMAL);
	if (DDFailedCheck(ddReturnVal, "SetCooperativeLevel() failed", cpErrorBuf ))
	{	MessageBox(main_window_handle, cpErrorBuf, "Init()", MB_ICONEXCLAMATION);   return(0); }
	
	/*
	// set the display mode
	ddReturnVal = lpddObj->SetDisplayMode(nWindowWidth,nWindowHeight,BPP,0,0);
	if (DDFailedCheck(ddReturnVal, "SetDisplayMode() failed", cpErrorBuf ))
	{	MessageBox(main_window_handle, cpErrorBuf, "Init()", MB_ICONEXCLAMATION);   return(0); }
	*/

	// Create the primary surface first set the fields
	memset(&ddsd,0,sizeof(ddsd));
	ddsd.dwSize         = sizeof(ddsd);
	ddsd.dwFlags        = DDSD_CAPS; //set the flags to validate both capabilites field adn the backbuffer count field
	ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE; //tell dd that u have a complex flippable surface
	//ddsd.dwBackBufferCount = 1; //set the back buffer count

	//Createt the primary surface
	ddReturnVal = lpddObj->CreateSurface(&ddsd,&lpddsPrimary,NULL);
    if (DDFailedCheck(ddReturnVal, "CreateSurface() failed", cpErrorBuf ))
	{	MessageBox(main_window_handle, cpErrorBuf, "Init()", MB_ICONEXCLAMATION);   return(0); }
	
	//Create an off-screen surface as a backBuffer
	SCCOLOR col={255,0,255};
	lpddsSecondary = CreateSurface(nWindowWidth, nWindowHeight, col);// 43 37
	lpddsTempSurface[0] = CreateSurface(64, 64, col);
	lpddsTempSurface[1]= CreateSurface(32, 32, col);
	lpddsTempSurface[2]= CreateSurface(32, 32, col);

	//Attach a clipper
	RECT cliplist[1];
	//full screen clip region
	cliplist[0].top = 1; cliplist[0].left = 1;
	cliplist[0].bottom = GetSystemMetrics(SM_CYSCREEN)-1; cliplist[0].right = GetSystemMetrics(SM_CXSCREEN)-1;
	//clipper for the window
	//GetWindowRect(main_window_handle,&cliplist[0]); 
	//AttachClipper(lpddsPrimary,1,cliplist);
	//AttachClipper(lpddsPrimary,main_window_handle);
	//AttachClipper(lpddsPrimary,GetDesktopWindow());

	//attack a color key
	// set color key to TransparentColor
	DDCOLORKEY color_key; // used to set color key
	color_key.dwColorSpaceLowValue  = _RGB16BIT565(TransColor.r,TransColor.g,TransColor.b);
	color_key.dwColorSpaceHighValue = _RGB16BIT565(TransColor.r,TransColor.g,TransColor.b);
	
	// now set the color key for source blitting
	ddReturnVal  = lpddsPrimary->SetColorKey(DDCKEY_SRCBLT, &color_key);
	if (DDFailedCheck(ddReturnVal, "SetColorKey() failed", cpErrorBuf ))
	{	MessageBox(main_window_handle, cpErrorBuf, "Init()", MB_ICONEXCLAMATION);   return(0); }

	//"""""""""""""""""""""Init Sprites"""""""""""""""""""""
	//My face
    static int array[]={0,1};
	Sprites[0].InitSprite(64,64,1,1,1,120);
	Sprites[0].SetAnimations(array,0,SPRITE_ANIM_ONCE);
	Sprites[0].LoadSpriteImage("./bitmaps\\myface1.bmp");
	Sprites[0].SetState(SPRITE_STATE_ALIVE);
	Sprites[1].SetCurrentAnim(0);
	Sprites[0].SetXPos(100);
	Sprites[0].SetYPos(100);
	
	//Ship
	static int array2[]= {0,1,2,3,4,5,6,7,8,9,10,11,12,13};
	Sprites[1].InitSprite(43,37,14,6,3,120);
	Sprites[1].SetAnimations(array2,13,SPRITE_ANIM_CONT);
	Sprites[1].LoadSpriteImage("./bitmaps\\Expl16.bmp");
	Sprites[1].SetState(SPRITE_STATE_ALIVE);
	Sprites[1].SetCurrentAnim(0);
	Sprites[1].SetXPos(400);
	Sprites[1].SetYPos(400);

	//Stickman
	static int array3[]={9,10,11,12},//run
		       array3a[]={5,6,7,8},//walk
			   array3b[]={17,18,19};
	Sprites[2].InitSprite(31,31,24,8,3,120);
	Sprites[2].SetAnimations(array3,4,SPRITE_ANIM_CONT);
	Sprites[2].SetAnimations(array3a,4,SPRITE_ANIM_CONT);
	Sprites[2].SetAnimations(array3b,3,SPRITE_ANIM_CONT);

	Sprites[2].LoadSpriteImage("./bitmaps\\stickman.bmp");
	Sprites[2].SetState(SPRITE_STATE_ALIVE);
	Sprites[2].SetCurrentAnim(0);
	//Sprites[2].SetXPos(100);
	//Sprites[2].SetYPos(50);
	//""""""""""""""""""End Init Sprites""""""""""""""""""""
	
	return(1);
}
//-----------------------------------------------------------------------------
// Name: AttachClipper()
// Desc: creates and attaches a clipper to a surface
//-----------------------------------------------------------------------------
LPDIRECTDRAWCLIPPER AttachClipper(LPDIRECTDRAWSURFACE7 lpddsSurface, HWND hWnd)
{
	LPDIRECTDRAWCLIPPER lpddClipper;  //pointer to the newly created dd clipper
	
	//first create the DD clipper
	ddReturnVal = lpddObj->CreateClipper(0,&lpddClipper, NULL);
	if (DDFailedCheck(ddReturnVal, "CreateClipper() failed", cpErrorBuf ))
	{	MessageBox(main_window_handle, cpErrorBuf, "AttachClipper()", MB_ICONEXCLAMATION);   return(NULL); }
	
	ddReturnVal = lpddClipper->SetHWnd(NULL,hWnd);
	if (DDFailedCheck(ddReturnVal, "lpddClipper->SetHWnd() failed", cpErrorBuf ))
	{	MessageBox(main_window_handle, cpErrorBuf, "AttachClipper()", MB_ICONEXCLAMATION);   return(NULL); }

	// now attach the clipper to the surface
	ddReturnVal = lpddsSurface->SetClipper(lpddClipper);
	if (DDFailedCheck(ddReturnVal, "SetClipper() failed", cpErrorBuf ))
	{	
		MessageBox(main_window_handle, cpErrorBuf, "AttachClipper()", MB_ICONEXCLAMATION);   
		// release memory and return error
		return(NULL); 
	}

	return(lpddClipper);
}

//-----------------------------------------------------------------------------
// Name: AttachClipper()
// Desc: creates and attaches a clipper to a surface
//-----------------------------------------------------------------------------
LPDIRECTDRAWCLIPPER AttachClipper(LPDIRECTDRAWSURFACE7 lpddsSurface, int iNumRects, LPRECT cliplist)
{
	int index;                        //loop va
	LPDIRECTDRAWCLIPPER lpddClipper;  //pointer to the newly created dd clipper
	LPRGNDATA regionData;             //pointer to the region data that contains the header and clip list

	//first create the DD clipper
	ddReturnVal = lpddObj->CreateClipper(0,&lpddClipper, NULL);
	if (DDFailedCheck(ddReturnVal, "CreateClipper() failed", cpErrorBuf ))
	{	MessageBox(main_window_handle, cpErrorBuf, "AttachClipper()", MB_ICONEXCLAMATION);   return(NULL); }
	
	//now create the clip list from the sent data
	//first allocate memory for region data
	regionData = (LPRGNDATA)malloc(sizeof(RGNDATAHEADER) + iNumRects*sizeof(RECT));
	//now copy the rects into region data
	memcpy(regionData->Buffer, cliplist, sizeof(RECT)*iNumRects);
	
	// set up fields of header
	regionData->rdh.dwSize          = sizeof(RGNDATAHEADER);
	regionData->rdh.iType           = RDH_RECTANGLES;
	regionData->rdh.nCount          = iNumRects;
	regionData->rdh.nRgnSize        = iNumRects*sizeof(RECT);

	regionData->rdh.rcBound.left    =  -64000;
	regionData->rdh.rcBound.top     =  -64000;
	regionData->rdh.rcBound.right   = 64000;
	regionData->rdh.rcBound.bottom  = 64000;

	// find bounds of all clipping regions
	for (index=0; index<iNumRects; index++)
    {
		// test if the next rectangle unioned with the current bound is larger
		if (cliplist[index].top < regionData->rdh.rcBound.top)
			regionData->rdh.rcBound.top = cliplist[index].top;

		if (cliplist[index].left < regionData->rdh.rcBound.left)
			 regionData->rdh.rcBound.left = cliplist[index].left;

		if (cliplist[index].bottom > regionData->rdh.rcBound.bottom)
			regionData->rdh.rcBound.bottom = cliplist[index].bottom;

		if (cliplist[index].right > regionData->rdh.rcBound.right)
			regionData->rdh.rcBound.right = cliplist[index].right;

    } // end for index

	// now we have computed the bounding rectangle region and set up the data
	// now let's set the clipping list
	
	ddReturnVal = lpddClipper->SetClipList(regionData, 0); //can also give lpddClipper a hWnd
	if (DDFailedCheck(ddReturnVal, "SetClipList() failed", cpErrorBuf ))
	{	
		MessageBox(main_window_handle, cpErrorBuf, "AttachClipper()", MB_ICONEXCLAMATION);   
		// release memory and return error
		free(regionData);
		return(NULL); 
	}
	
	// now attach the clipper to the surface
	ddReturnVal = lpddsSurface->SetClipper(lpddClipper);
	if (DDFailedCheck(ddReturnVal, "SetClipper() failed", cpErrorBuf ))
	{	
		MessageBox(main_window_handle, cpErrorBuf, "AttachClipper()", MB_ICONEXCLAMATION);   
		// release memory and return error
		free(regionData);
		return(NULL); 
	}
	
	// all is well, so release memory and send back the pointer to the new clipper
	free(regionData);
	return(lpddClipper);

} // end AttachClipper

//-----------------------------------------------------------------------------
// Name: ShutDown()
// Desc: cleans up evreything. releases mem used
//-----------------------------------------------------------------------------
int ShutDown()
{
	//release all the temp surfaces
	for(int i=0; i<MAX_TEMPSURFACES; i++)
		if(lpddsTempSurface[i] != NULL)
			lpddsTempSurface[i]->Release();

	//release the secondary surface
	if(lpddsSecondary != NULL)
		lpddsSecondary->Release();

	// now release the primary surface
	if (lpddsPrimary!=NULL)
		lpddsPrimary->Release();
    
	// release the directdraw object
	if (lpddObj!=NULL)
		lpddObj->Release();

	return(1);
}

//-----------------------------------------------------------------------------
// Name: GetKeyStatus()
// Desc: Tells you whether a key has been pressed/held down or not
//-----------------------------------------------------------------------------
bool GetKeyStatus(int key)
{
	return iKeyStatus[key];
}