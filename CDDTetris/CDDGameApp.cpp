//-----------------------------------------------------------------------------
// File: CDDGameApp.cpp
//
// Desc: Game Application class, this is the central hub for all app processing
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// CGameApp Specific Includes
//-----------------------------------------------------------------------------
#include "CDDGameApp.h"

//-----------------------------------------------------------------------------
// CDDGameApp Member Functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Name : CDDGameApp () (Constructor)
// Desc : CDDGameApp Class Constructor
//-----------------------------------------------------------------------------
CDDGameApp::CDDGameApp()
{
	m_LastFrameRate    = 0;    
    
	m_OldCursorPos.x = m_OldCursorPos.y = 0;     // Old cursor position for tracking
	m_wndRect.bottom = m_wndRect.top = m_wndRect.left = m_wndRect.right = 0;
	for( int i=0; i<256; i++ ) m_pbKeyStatus[i] = false;

    m_hWnd             = NULL;             
    m_hIcon            = NULL;            
    m_hMenu            = NULL;
	
	LoadString((HINSTANCE)GetModuleHandle(NULL), IDS_STRING112,m_WindowTitle, 40);
	//m_WindowTitle      ;
    
    m_bLostDevice      = false;      
    m_bActiveApp       = true;          
    m_bDebugMode       = false;
	m_bExitApp         = false;

	m_pSprites          = NULL;
	m_nppAnimationList  = NULL;

	m_lpddObj          = NULL;          
	m_lpddsPrimary     = NULL;     
	m_lpddsSecondary   = NULL;   
	m_lpddpal          = NULL; 
}

//-----------------------------------------------------------------------------
// Name : ~CDDGameApp () (Destructor)
// Desc : CDDGameApp Class Destructor
//-----------------------------------------------------------------------------
CDDGameApp::~CDDGameApp()
{
	// Shut the engine down
    ShutDown();
}

//-----------------------------------------------------------------------------
// Name : setDisplaySettings()
// Desc : fills in the display settings class
//-----------------------------------------------------------------------------
bool CDDGameApp::setDisplaySettings( bool windowed, int bitsPerPixel, int windowWidth, int windowHeight,
		                             int backBufferWidth, int backBufferHeight, RECT windowRect, SCCOLOR  transparentColor )
{
	m_DDSettings.Windowed = windowed;
	(m_DDSettings.GetSettings())->nBPP = bitsPerPixel;
	(m_DDSettings.GetSettings())->nWindowHeight = windowHeight;
	(m_DDSettings.GetSettings())->nWindowWidth = windowWidth;
	(m_DDSettings.GetSettings())->nBackBufferHeight = backBufferHeight;
	(m_DDSettings.GetSettings())->nBackBufferWidth = backBufferWidth;
	(m_DDSettings.GetSettings())->wndRect = windowRect;
	(m_DDSettings.GetSettings())->TransColorR = (int)transparentColor.r;
	(m_DDSettings.GetSettings())->TransColorG = (int)transparentColor.g;
	(m_DDSettings.GetSettings())->TransColorB = (int)transparentColor.b;
	return true;
}

//-----------------------------------------------------------------------------
// Name : InitInstance ()
// Desc : Initialises the entire Engine here.
//-----------------------------------------------------------------------------
bool CDDGameApp::InitInstance( HANDLE hInstance, LPCTSTR lpCmdLine, int iCmdShow )
{
    // Create the primary display device
    if (!CreateDisplay()) { ShutDown(); return false; }

    // Build Objects
    if (!BuildSprites()) { ShutDown(); return false; }

    // Set up all required game states
    SetupGameState();

    // Setup our rendering environment
    //SetupRenderStates();

    // Success!
	return true;
}

//-----------------------------------------------------------------------------
// Name : CreateDisplay ()
// Desc : Create the display windows, devices etc, ready for rendering.
//-----------------------------------------------------------------------------
bool CDDGameApp::CreateDisplay()
{
    CDDInitialize   Initialize;
    HRESULT         ddReturnVal = 0;
	char cpErrorBuf[1000];
	
	// create dd object and test for error
	ddReturnVal = DirectDrawCreateEx(NULL, (void **)&m_lpddObj, DD_VERSION, NULL);
	if(DDFailedCheck(ddReturnVal, "DirectDrawCreateEx() failed", cpErrorBuf ))
	{	MessageBox(m_hWnd, cpErrorBuf, "Init()", MB_ICONEXCLAMATION); return (0); }
	
	//Give the utilities class to the init class for use with clippers and creating surfaces
	Initialize.setUtilities( m_Utils );
	Initialize.setDirectDObj( m_lpddObj );
	
	//Create the window and setup dd
	if ( FAILED( Initialize.CreateDisplay( m_DDSettings, 0, m_hWnd, StaticWndProc, m_WindowTitle, this ) ))
    {
        MessageBox( m_hWnd, _T("Device creation failed. The application will now exit."), _T("Fatal Error!"), MB_OK | MB_ICONSTOP | MB_APPLMODAL );
        return false;
    
    } // End if Failed
    
	// Retrieve created items
	setPrimarySurf( Initialize.getPrimarySurface() );
	setSecondarySurf( Initialize.getSecondarySurface() );
	m_hWnd = Initialize.getHWND();

	// Load icon and menu
    m_hIcon = LoadIcon( (HINSTANCE)GetModuleHandle(NULL), MAKEINTRESOURCE( IDI_ICON1 ) );
    m_hMenu = LoadMenu( (HINSTANCE)GetModuleHandle(NULL), MAKEINTRESOURCE( IDR_MENU1 ) );

    // Set application icon
    SetClassLong( m_hWnd, GCL_HICON, (long)m_hIcon );
	
    // Set menu only in windowed mode
    if ( m_DDSettings.Windowed )
    {
        SetMenu( m_hWnd, m_hMenu );
    } // End if Windowed

	 // Retrieve the final client size of the window
	if(m_DDSettings.Windowed)
		::GetClientRect( m_hWnd, &m_wndRect );
	else
		::GetWindowRect(m_hWnd, &m_wndRect );

    //Fill in the utilities class
	m_Utils.setWindowHandle( Initialize.getHWND() );
	m_Utils.setWndRect( m_wndRect );
	m_Utils.setSettingsClass( m_DDSettings );
   
    // Show the window
	ShowWindow(m_hWnd, SW_SHOW);

    // Success!!
    return true;
}

//----------------------------------------------------------------------
// Name: SetupUtilitiesClasses()
// Desc: fills in all the Utility classes for any obj that has a instance
//       of the Utilities class
//----------------------------------------------------------------------
void CDDGameApp::UpdateUtilitiesClasses(  )
{
	for( int i=0; i<MAX_SPRITES; i++ )
		m_pSprites[i].SetUtilityClass( m_Utils );
}

//-----------------------------------------------------------------------
// Name: BiuldSprites()
// Desc: This is where you do all Initialization of the sprites. 
//       animations can be stored in m_nppAnimationList
// Example:
//		Sprites[2].InitSprite(23,19,32,16,2,120);
//		Sprites[2].SetAnimationIndex(0);
//		Sprites[2].SetAnimations(array,1,SPRITE_ANIM_ONCE,0);
//		Sprites[2].LoadSpriteImage24("./bitmaps\\Blaze16.bmp");
//		Sprites[2].SetState(SPRITE_STATE_ALIVE);
//		Sprites[2].SetXPos(100);
//		Sprites[2].SetYPos(100);
//-----------------------------------------------------------------------
bool CDDGameApp::BuildSprites()
{
	//Make sure nothing is created already if so release it all
	//ReleaseSprites();
	//Create the space
	//m_nppAnimationList = new int*[MAX_SPRITE_ANIMATIONS];
	//for( int i=0; i<MAX_SPRITE_ANIMATIONS; i++ ) m_nppAnimationList[i] = NULL;
	//m_pSprites = new CSPRITE[MAX_SPRITES];
	//Update the sprites utility classes
	//UpdateUtilitiesClasses();
	
	return true;
}

//----------------------------------------------------------------------
// Name: ReleaseSprites()
// Desc: releases all sprites and all animations
//----------------------------------------------------------------------
void CDDGameApp::ReleaseSprites()
{
	if( m_nppAnimationList != NULL )
	{
		for( int i=0; i<MAX_SPRITE_ANIMATIONS; i++ )
		{
			if( m_nppAnimationList[i] != NULL )
			{ 
				delete [] m_nppAnimationList[i]; 
				m_nppAnimationList[i] = NULL; 
			}
		}
		if( m_nppAnimationList ) { delete [] m_nppAnimationList; m_nppAnimationList = NULL;	}
	}
	if( m_pSprites != NULL )  { delete [] m_pSprites; m_pSprites = NULL; }
}

//-----------------------------------------------------------------------
// Name: SetupGameState()
// Desc: Setup the state of the game, can be locations of sprites physics
//       whatever u need
//-----------------------------------------------------------------------
void CDDGameApp::SetupGameState()
{

}

//-----------------------------------------------------------------------
// Name: SetupRenderState()
// Desc: 
//-----------------------------------------------------------------------
void CDDGameApp::SetupRenderState  ( )
{
}

//-----------------------------------------------------------------------------
// Name : BeginGame ()
// Desc : Signals the beginning of the physical post-initialisation stage.
//        From here on, the game engine has control over processing.
//-----------------------------------------------------------------------------
int CDDGameApp::BeginGame()
{
    MSG		msg;

    // Start main loop
	while (1) 
    {
        // Did we recieve a message, or are we idling ?
		if ( PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) ) 
        {
			if (msg.message == WM_QUIT) break;
			TranslateMessage( &msg );
			DispatchMessage ( &msg );
		} 
        else 
        {
			// Advance Game Frame.
			FrameAdvance();

		} // End If messages waiting
	
    } // Until quit message is receieved

    return 0;
}

//-----------------------------------------------------------------------
// Name: FrameAdvance()
// Desc: 
//-----------------------------------------------------------------------
void CDDGameApp::FrameAdvance()
{
	m_Utils.ClrS(m_lpddsSecondary,NULL);
	
	Sleep(30);
	m_pSprites[0].DrawSprite(m_lpddsSecondary);
	
	if(m_DDSettings.Windowed)
		m_Utils.FlipToClient( m_lpddsSecondary );
	else
		m_Utils.Flip( );
}

//-----------------------------------------------------------------------
// Name: ShutDown()
// Desc: release all COM objects and cleans up
//-----------------------------------------------------------------------
bool CDDGameApp::ShutDown( )
{
	ReleaseSprites();
	if( m_lpddsSecondary != NULL ) { m_lpddsSecondary->Release(); m_lpddsSecondary = NULL; }
	if( m_lpddsPrimary != NULL )   { m_lpddsPrimary->Release(); m_lpddsPrimary = NULL; }
	if( m_lpddObj != NULL )        
	{ 
		while(m_lpddObj->Release() > 0 ) {}//ensure that all DDobjs are released there is a bug here they should have all been released
		
		m_lpddObj = NULL; 
	}
	return true;
}

//-----------------------------------------------------------------------
// Name: Release()
// Desc: releases surfaces and com objects for recreation of display 
//-----------------------------------------------------------------------
bool CDDGameApp::ReleaseCOMs( )
{
	if( m_lpddsSecondary != NULL ) { m_lpddsSecondary->Release(); m_lpddsSecondary = NULL; }
	if( m_lpddsPrimary != NULL )   { m_lpddsPrimary->Release(); m_lpddsPrimary = NULL; }
	if( m_lpddObj != NULL )        { m_lpddObj->Release(); m_lpddObj = NULL; }
	return true;
}

//-----------------------------------------------------------------------
// Name: setPrimarySurf()
// Desc: 
//-----------------------------------------------------------------------
void CDDGameApp::setPrimarySurf( LPDIRECTDRAWSURFACE7 lpddsSurface )
{
	if( m_lpddsPrimary != NULL ) { m_lpddsPrimary->Release(); m_lpddsPrimary = NULL;  }
	m_lpddsPrimary = lpddsSurface;
	if( m_lpddsPrimary != NULL ) m_lpddsPrimary->AddRef();
}

//-----------------------------------------------------------------------
// Name: setSecondarySurf()
// Desc: 
//-----------------------------------------------------------------------
void CDDGameApp::setSecondarySurf( LPDIRECTDRAWSURFACE7 lpddsSurface )
{
	if( m_lpddsSecondary != NULL ) { m_lpddsSecondary->Release(); m_lpddsSecondary = NULL; }
	m_lpddsSecondary = lpddsSurface;
	if( m_lpddsSecondary != NULL ) m_lpddsSecondary->AddRef();
}

//-----------------------------------------------------------------------------
// Name : StaticWndProc () (Static Callback)
// Desc : This is the main messge pump for ALL display devices, it captures
//        the appropriate messages, and routes them through to the application
//        class for which it was intended, therefore giving full class access.
// Note : It is VITALLY important that you should pass your 'this' pointer to
//        the lpParam parameter of the CreateWindow function if you wish to be
//        able to pass messages back to that app object.
//-----------------------------------------------------------------------------
LRESULT CALLBACK CDDGameApp::StaticWndProc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
    // If this is a create message, trap the 'this' pointer passed in and store it within the window.
    if ( Message == WM_CREATE ) SetWindowLong( hWnd, GWL_USERDATA, (LONG)((CREATESTRUCT FAR *)lParam)->lpCreateParams);

    // Obtain the correct destination for this message
    CDDGameApp *Destination = (CDDGameApp*)GetWindowLong( hWnd, GWL_USERDATA );
    
    // If the hWnd has a related class, pass it through
    if (Destination) return Destination->DisplayWndProc( hWnd, Message, wParam, lParam );
    
    // No destination found, defer to system...
    return DefWindowProc( hWnd, Message, wParam, lParam );
}

//-----------------------------------------------------------------------------
// Name : DisplayWndProc ()
// Desc : The display devices internal WndProc function. All messages being
//        passed to this function are relative to the window it owns.
//-----------------------------------------------------------------------------
LRESULT CDDGameApp::DisplayWndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
    PAINTSTRUCT	ps;		   // used in WM_PAINT
	HDC hDC = NULL;
	
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
			m_bActiveApp = (bool)wParam;
			break;
		}

	case WM_SIZE:
		{
            if ( wParam == SIZE_MINIMIZED )
            {
                // App is inactive
                m_bActiveApp = false;
            
            } // App has been minimized
            else
            {
                // App is active
                m_bActiveApp = true;      
            } // End if !Minimized

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
			//m_pbKeyStatus[wParam] = true;
		
			switch (wParam) 
            {
				case VK_ESCAPE:
					m_bExitApp = true;
					//PostQuitMessage(0);
					return 0;
				case 0x57: //W
					{//Switch to windowed mode
						DestroyWindow(m_hWnd);
						UnregisterClass(m_WindowTitle,(HINSTANCE)GetModuleHandle(NULL));
						m_hWnd = NULL;//(HINSTANCE)GetModuleHandle(NULL)
						setDisplaySettings(true);
						ShutDown();
						InitInstance((HINSTANCE)GetModuleHandle(NULL), NULL, 0);
						ShowCursor(TRUE);
						OutputDebugString( "W pressed window destroyed no one created" );
						return 0;
					}
				case 0x56: //V
					{
						return 0;
					}
			}
			break;
		}

	case WM_KEYUP:
		{
			//m_pbKeyStatus[wParam] = false;
			break;
		}

	case WM_CLOSE:
		m_bExitApp = true;
		break;
	case WM_COMMAND:
		{
			// Process Menu Items
			switch( LOWORD(wParam) )
			{
				case ID_VIDEO_FULLSCREEN:
					{// Signal that we want to go full screen
						DestroyWindow(m_hWnd);
						UnregisterClass(m_WindowTitle,(HINSTANCE)GetModuleHandle(NULL));
						m_hWnd = NULL;
						setDisplaySettings(false);
						ShutDown();
						InitInstance((HINSTANCE)GetModuleHandle(NULL), NULL, 0);
						ShowCursor(FALSE);
						OutputDebugString( "Full screen mode activated" );
						return 0;
					}
               
				case ID_FILE_EXIT:
					// Recieved key/menu command to exit app
					m_bExitApp = true;
					SendMessage( m_hWnd, WM_CLOSE, 0, 0 );
					return 0;

			}//End switch( LOWORD(wParam) )
			break;	
		 }
	case WM_DESTROY: 
		{// kill the application			
			
			//close the program
			if( m_bExitApp ) PostQuitMessage(0);
			return(0);
		} break;

	default:break;

    } // end main switch

	// process any messages that we didn't take care of 
	return (DefWindowProc(hWnd, uMsg, wParam, lParam));
}

//-----------------------------------------------------------------------------
// Name: GetKeyStatus()
// Desc: Tells you whether a key has been pressed/held down or not
//-----------------------------------------------------------------------------
bool CDDGameApp::getKeyStatus(int key)
{
	return m_pbKeyStatus[key];
}