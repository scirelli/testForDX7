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
#include "SurfaceFunc.h"
#include "DynamicSpaceField.h"

////GLOBALS/////////////////////////////////////////
char                *cpWindowName   = "Template Test";
HWND                 main_window_handle= NULL;  // save the window handle
HINSTANCE            main_instance  = NULL;  // save the instance
HDC                  hDC            = NULL;  // creates a handle to a device contexts
BOOL                 bActiveApp     = false; // tells whether app is active or not
LPDIRECTDRAW7        lpddObj        = NULL;  // dd object
LPDIRECTDRAWSURFACE7 lpddsPrimary   = NULL;  // dd primary surface
LPDIRECTDRAWSURFACE7 lpddsSecondary = NULL;  // dd back surface
LPDIRECTDRAWPALETTE  lpddpal        = NULL;  // a pointer to the created dd palette
DDSURFACEDESC2       ddsd;                   // a direct draw surface description struct
DDSCAPS2             ddscaps;                // a direct draw surface capabilities struct
DDBLTFX              ddbltfx;                // The Blit Fx structure used for all blting
HRESULT              ddReturnVal    = 0;     // result back from dd calls
bool                 iKeyStatus[256];        // keeps track of key status
char                 cpErrorBuf[1024];       // Build up error messages
RECT                 wndRect;                // A rect the size of the window
CTIMER               gameTimer, frameTimer2;
CTIMER2              gameTimer2,frameTimer;
float                fFrameTime     = 0;     //ms
SCCOLOR              TransColor     ={255,0,255};//The transparent color for all bitmaps
CSPRITE              Sprites[MAX_SPRITES];
PARTICLE             particles[MAX_PARTICLES];
bool                 bDebugMode     = false;

////PROTOTYPES/////////////////////////////////////
int InitDD();
int InitSprites();
LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
int WINAPI WinMain(	HINSTANCE hinstance, HINSTANCE hprevinstance, LPSTR lpcmdline, int ncmdshow);
HWND SetupWindow(WNDCLASS *winclass, HINSTANCE hInstance);
int ShutDown();

/////FUNCTIONS/////////////////////////////////////

//-----------------------------------------------------------------------------
// Name: WindowProc()
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
			//get the device context of the client area
			//hDC= GetDC(hWnd);
			
			return(0);
		} break;
	case WM_PAINT:
         {
			// start painting
			hDC = BeginPaint(hWnd,&ps);

			// end painting
			EndPaint(hWnd,&ps);
			ReleaseDC(hWnd, hDC);
			return(0);
        } break;
	case WM_ACTIVATEAPP: 
		{
			bActiveApp = wParam;
			break;
		}
	case WM_SETFOCUS:
		{
			//for(int i = 0; i<MAX_SPRITES; i++)
			//	Sprites[i].RestoreSurface();
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
	
	// save the window handle and instance in a global
	main_window_handle = SetupWindow(&winclass, hInstance);
	main_instance      = hInstance;

	ShowWindow(main_window_handle,nCmdShow);
	UpdateWindow(main_window_handle);
	SetFocus(main_window_handle);
	ShowCursor(FALSE);
	if( (strcmp(lpCmdLine,"debug") == 0) || (strcmp(lpCmdLine,"Debug") == 0) )
		bDebugMode = true;

	// perform all game console specific initialization
	InitDD();
	InitSprites();
	gameTimer.StartTimer();
	gameTimer2.StartTimer();
	fFrameTime=0;
	frameTimer.StartTimer();

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
		
		if(bActiveApp)//only process frames if window is active
			bDone = main();
		else
			Sleep(10);
	
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
	winclass->hbrBackground	= (HBRUSH) GetStockObject(BLACK_BRUSH);//GRAY_BRUSH
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
							  WS_POPUP | WS_VISIBLE,  //WS_POPUP | WS_VISIBLE, WS_OVERLAPPEDWINDOW,WS_OVERLAPPED
						 	  0,0,	   // x,y
							  WINDOW_WIDTH,  // width
						      WINDOW_HEIGHT, // height
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
// Name: InitDD()
// Desc: Initializes Driect draw and anything that needs to be inited
//-----------------------------------------------------------------------------
int InitDD()
{// this function is where i do all the initialization 
 // for the game

	// create dd object and test for error
	ddReturnVal = DirectDrawCreateEx(NULL, (void **)&lpddObj, IID_IDirectDraw7, NULL);
	if(DDFailedCheck(ddReturnVal, "DirectDrawCreateEx() failed", cpErrorBuf ))
	{	MessageBox(main_window_handle, cpErrorBuf, "Init()", MB_ICONEXCLAMATION); return (0); }
	
	/*
	//set cooperation level to windowed mode normal
	ddReturnVal = lpddObj->SetCooperativeLevel(main_window_handle, DDSCL_NORMAL);
	if (DDFailedCheck(ddReturnVal, "SetCooperativeLevel() failed", cpErrorBuf ))
	{	MessageBox(main_window_handle, cpErrorBuf, "Init()", MB_ICONEXCLAMATION);   return(0); }
	*/
	//set cooperation level to full screen
	ddReturnVal = lpddObj->SetCooperativeLevel(main_window_handle,	DDSCL_ALLOWMODEX | DDSCL_FULLSCREEN | 
																	DDSCL_EXCLUSIVE | DDSCL_ALLOWREBOOT);
	if (DDFailedCheck(ddReturnVal, "SetCooperativeLevel() failed", cpErrorBuf ))
	{	MessageBox(main_window_handle, cpErrorBuf, "Init()", MB_ICONEXCLAMATION);   return(0); }

	// set the display mode
	ddReturnVal = lpddObj->SetDisplayMode(WINDOW_WIDTH,WINDOW_HEIGHT,BPP,0,0);
	if (DDFailedCheck(ddReturnVal, "SetDisplayMode() failed", cpErrorBuf ))
	{	MessageBox(main_window_handle, cpErrorBuf, "Init()", MB_ICONEXCLAMATION);   return(0); }

	// Create the primary surface first set the fields
	memset(&ddsd,0,sizeof(ddsd));
	ddsd.dwSize         = sizeof(ddsd);
	ddsd.dwFlags        = DDSD_CAPS | DDSD_BACKBUFFERCOUNT; //set the flags to validate both capabilites field adn the backbuffer count field
	ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE | DDSCAPS_FLIP | DDSCAPS_COMPLEX; //tell dd that u have a complex flippable surface
	ddsd.dwBackBufferCount = 1; //set the back buffer count

	//Createt the primary surface
	ddReturnVal = lpddObj->CreateSurface(&ddsd,&lpddsPrimary,NULL);
    if (DDFailedCheck(ddReturnVal, "SetCooperativeLevel() failed", cpErrorBuf ))
	{	MessageBox(main_window_handle, cpErrorBuf, "Init()", MB_ICONEXCLAMATION);   return(0); }
	
	//Query for the backbuffer, use the ddscaps to indicate what you're requesting
	ddscaps.dwCaps = DDSCAPS_BACKBUFFER;
	//get the surface
	ddReturnVal = lpddsPrimary->GetAttachedSurface(&ddscaps, &lpddsSecondary);
	if (DDFailedCheck(ddReturnVal, "GetAttachedSurface() failed", cpErrorBuf ))
	{	MessageBox(main_window_handle, cpErrorBuf, "Init()", MB_ICONEXCLAMATION);   return(0); }
	
	//Attach a clipper
	RECT cliplist[1];
	cliplist[0].top = 0; cliplist[0].left = 0;
	cliplist[0].bottom = GetSystemMetrics(SM_CYSCREEN);
	cliplist[0].right = GetSystemMetrics(SM_CXSCREEN);
	AttachClipper(lpddsSecondary,1,cliplist);
	AttachClipper(lpddsPrimary,main_window_handle);
	
	//attack a color key
	// set color key to TransparentColor
	DDCOLORKEY color_key; // used to set color key
	if(BPP == 16)
	{
		color_key.dwColorSpaceLowValue  = BIULDCOLOR(TransColor.r,TransColor.g,TransColor.b);
		color_key.dwColorSpaceHighValue = BIULDCOLOR(TransColor.r,TransColor.g,TransColor.b);
	}
	else
		if(BPP == 32)
		{
			color_key.dwColorSpaceLowValue  = _ARGB24BIT(0,TransColor.r,TransColor.g,TransColor.b);
			color_key.dwColorSpaceHighValue = _ARGB24BIT(0,TransColor.r,TransColor.g,TransColor.b);
		}
	
	// now set the color key for source blitting
	ddReturnVal  = lpddsSecondary->SetColorKey(DDCKEY_SRCBLT, &color_key);
	if (DDFailedCheck(ddReturnVal, "SetColorKey() failed", cpErrorBuf ))
	{	MessageBox(main_window_handle, cpErrorBuf, "Init()", MB_ICONEXCLAMATION);   return(0); }
	
	//Fill in the wndRect struct
	wndRect.top    = 0;
	wndRect.left   = 0;
	wndRect.bottom = WINDOW_HEIGHT-1;
	wndRect.right  = WINDOW_WIDTH-1;

	return(1);
}


//-----------------------------------------------------------------------------
// Name: InitSprites()
// Desc: initiates all sprites
//	Sprites[2].InitSprite(23,19,32,16,2,120);
//	Sprites[2].SetAnimationIndex(0);
//	Sprites[2].SetAnimations(array,1,SPRITE_ANIM_ONCE,0);
//	Sprites[2].LoadSpriteImage24("./bitmaps\\Blaze16.bmp");
//	Sprites[2].SetState(SPRITE_STATE_ALIVE);
//	Sprites[2].SetXPos(100);
//	Sprites[2].SetYPos(100);
//-----------------------------------------------------------------------------
int InitSprites()
{
	static int npLogo[]={0,1,2,3,4,5,6,7,8,9},
		       npBlock1[]={0,0}, npBlock2[]={1,1};

	//Logo Sprite
	Sprites[0].InitSprite(154,66,10,4,3,110);
	Sprites[0].SetAnimations(npLogo,10,SPRITE_ANIM_ONCE);
	Sprites[0].SetCurrentAnim(0);
	Sprites[0].LoadSpriteImage24("./bitmaps\\TAOLogoBlackFadeHolo_154x66.bmp");
	Sprites[0].SetState(SPRITE_STATE_ALIVE);
	Sprites[0].physProp.fCenterofMassX = ( ((float)Sprites[0].GetWidth())*0.5f );
	Sprites[0].physProp.fCenterofMassY = ( ((float)Sprites[0].GetHeight())*0.5f );
	Sprites[0].SetXPos( (WINDOW_WIDTH/2)-154 );//(WINDOW_WIDTH/2)-Sprites[0].physProp.fCenterofMassX);
	Sprites[0].SetYPos( (WINDOW_HEIGHT/2)-66 );//(WINDOW_HEIGHT/2)-Sprites[0].physProp.fCenterofMassY);
	Sprites[0].physProp.fMass = Lb_Kg(0.f); //kg
		
	//Logo block 1 sprite
	Sprites[1].InitSprite(24,32,2,2,1,0);
	Sprites[1].SetAnimations(npBlock1,1,SPRITE_ANIM_ONCE);
	Sprites[1].SetCurrentAnim(0);
	Sprites[1].LoadSpriteImage24("./bitmaps\\logoBlocks24x32_b.bmp");
	Sprites[1].SetState(SPRITE_STATE_ALIVE);
	Sprites[1].physProp.fCenterofMassX = ( ((float)Sprites[1].GetWidth())*0.5f );
	Sprites[1].physProp.fCenterofMassY = ( ((float)Sprites[1].GetHeight())*0.5f );
	Sprites[1].SetXPos(Sprites[0].GetXPos()+83);//83
	Sprites[1].SetYPos(-Sprites[1].GetHeight()-15);//Sprites[0].GetYPos()+4
	Sprites[1].physProp.fMass = Lb_Kg(300.f); //kg

	//Logo block 2 sprite
	Sprites[2].InitSprite(24,32,2,2,1,0);
	Sprites[2].SetAnimations(npBlock2,1,SPRITE_ANIM_ONCE);
	Sprites[2].SetCurrentAnim(0);
	Sprites[2].LoadSpriteImage24("./bitmaps\\logoBlocks24x32_b.bmp");
	Sprites[2].SetState(SPRITE_STATE_ALIVE);
	Sprites[2].physProp.fCenterofMassX = ( ((float)Sprites[2].GetWidth())*0.5f );
	Sprites[2].physProp.fCenterofMassY = ( ((float)Sprites[2].GetHeight())*0.5f );
	Sprites[2].SetXPos(Sprites[0].GetXPos()+97);//97
	Sprites[2].SetYPos(-Sprites[2].GetHeight()-15);//Sprites[0].GetYPos()+4
	Sprites[2].physProp.fMass = Lb_Kg(300.f); //kg
	
	return (1);
}

//-----------------------------------------------------------------------------
// Name: ShutDown()
// Desc: cleans up evreything. releases mem used
//-----------------------------------------------------------------------------
int ShutDown()
{
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