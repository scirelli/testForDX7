//-----------------------------------------------------------------------------
// File: CDDInitialize.cpp
//
// Desc: DirectD initialization classes.
//-----------------------------------------------------------------------------

////INCLUDES///////////////////////
#include "DDInitialize.h"


//-----------------------------------------------------------------------------
// Name : CDDInitialize () (Constructor)
// Desc : CDDInitialize Class Constructor
//-----------------------------------------------------------------------------
CDDInitialize::CDDInitialize()
{
	m_lpddObj         = NULL;
	m_lpddsPrimary    = NULL;  
	m_lpddsSecondary  = NULL;
	m_hWnd            = NULL;          
}

//-----------------------------------------------------------------------------
// Name : ~CDDInitialize () (Destructor)
// Desc : CDDInitialize Class Destructor
//-----------------------------------------------------------------------------
CDDInitialize::~CDDInitialize()
{
	if( m_lpddsSecondary != NULL ) { m_lpddsSecondary->Release(); m_lpddsSecondary  = NULL; }
	if( m_lpddsPrimary != NULL )   { m_lpddsPrimary->Release(); m_lpddsPrimary = NULL; }
	if( m_lpddObj != NULL )        { m_lpddObj->Release(); m_lpddObj = NULL; }
	m_hWnd = NULL; 
}

//-----------------------------------------------------------------------------
// Name : Enumerate()
// Desc : 
//-----------------------------------------------------------------------------
bool CDDInitialize::Enumerate(LPDIRECTDRAW7 lpddObj) 
{ 
	return true;
}

//-----------------------------------------------------------------------------
// Name : setDirectDObj()
// Desc : sets the directdraw object
//-----------------------------------------------------------------------------
void CDDInitialize::setDirectDObj( LPDIRECTDRAW7 lpddObj)
{
	if( m_lpddObj != NULL ) { m_lpddObj->Release(); m_lpddObj = NULL; }
	m_lpddObj = lpddObj;
	if( m_lpddObj != NULL ) m_lpddObj->AddRef();
}

//-----------------------------------------------------------------------------
// Name : CreateDisplay ()
// Desc : Creates the display devices ready for rendering.
//-----------------------------------------------------------------------------
HRESULT CDDInitialize::CreateDisplay( CDDSettings& DDSettings, ULONG Flags, HWND hWnd, WNDPROC pWndProc, LPCTSTR Title, LPVOID lParam )
{
	CDDSettings::Settings *pSettings   = DDSettings.GetSettings();
	char    cpErrorBuf[1000];
	HRESULT ddReturnVal = 0;
	
	if ( !hWnd )
    {
        // Register the new windows window class.
        WNDCLASS			wc;	
	    wc.style			= CS_BYTEALIGNCLIENT | CS_HREDRAW | CS_VREDRAW;
	    wc.lpfnWndProc		= pWndProc;
	    wc.cbClsExtra		= 0;
	    wc.cbWndExtra		= 0;
	    wc.hInstance		= (HINSTANCE)GetModuleHandle(NULL);
        wc.hIcon			= NULL;
	    wc.hCursor			= LoadCursor(NULL, IDC_ARROW);
	    wc.hbrBackground	= (HBRUSH )GetStockObject(GRAY_BRUSH);
	    wc.lpszMenuName		= NULL;
	    wc.lpszClassName	= Title;
	    
		// register the window class
		if ( !RegisterClass(&wc) ) //RegisterClass(&winclass)
		{
			MessageBox(NULL, "Window Could not be registered.", "Error Registering window", MB_ICONEXCLAMATION); //NULL is default for Error
			return(E_FAIL);
		}

        ULONG Left  = CW_USEDEFAULT, 
			  Top   = CW_USEDEFAULT;
        ULONG Style = WS_OVERLAPPEDWINDOW;
		int Width =  pSettings->nWindowWidth, Height = pSettings->nWindowHeight;
					 
        // Create the rendering window
        if ( !DDSettings.Windowed )
        {
            Left   = 0; Top = 0;
			Style  = WS_VISIBLE | WS_POPUP;
        } // End if Fullscreen
		//////////////////////LEFT OFF HERE
        // Create the window
        m_hWnd = CreateWindow( Title, Title, Style, Left, Top, Width, Height, NULL, NULL, wc.hInstance, lParam );

        // Bail on error
        if (!m_hWnd) return E_FAIL;

    } // End if no Window Passed
    else
    {
        // Store HWND
        m_hWnd = hWnd;
        
        // Setup styles based on windowed / fullscreen mode
        if ( !DDSettings.Windowed )
        {
            SetMenu( m_hWnd, NULL );//get rid of menu
            SetWindowLong( m_hWnd, GWL_STYLE, WS_VISIBLE | WS_POPUP );//set the window style
			SetWindowPos( m_hWnd, NULL, 0, 0, pSettings->nWindowWidth, pSettings->nWindowHeight, SWP_NOZORDER );
        
        } // End if Fullscreen
        else
        {
            RECT rc;

            // Get the windows client rectangle
            GetWindowRect( hWnd, &rc );

            // Setup the window properties
            SetWindowLong( m_hWnd, GWL_STYLE, WS_OVERLAPPEDWINDOW );
            SetWindowPos( hWnd, HWND_NOTOPMOST, rc.left, rc.top, (rc.right - rc.left), (rc.bottom - rc.top), SWP_NOACTIVATE | SWP_SHOWWINDOW );

        } // End if Windowed
    
    } // End if window passed

    // Setup DirectDraw
	if( DDSettings.Windowed )
	{//setup DD for windowed mode
		//set cooperation level to windowed mode normal
		ddReturnVal = m_lpddObj->SetCooperativeLevel(m_hWnd, DDSCL_NORMAL);
		if (DDFailedCheck(ddReturnVal, "SetCooperativeLevel() failed", cpErrorBuf ))
		{	MessageBox(m_hWnd, cpErrorBuf, "Init()", MB_ICONEXCLAMATION);   return(E_FAIL); }
		
		// Create the primary surface first set the fields
		//first check to see if there's a primary or secondary surface and release if so
		if( m_lpddsSecondary ) { m_lpddsSecondary->Release(); m_lpddsSecondary=NULL;}
		if( m_lpddsPrimary )   { m_lpddsPrimary->Release(); m_lpddsPrimary = NULL;  }
		//filling the surface descriptor for the primary surface
        memset(&pSettings->ddsd,0,sizeof(pSettings->ddsd)); //clear all the fields
		pSettings->ddsd.dwSize         = sizeof(pSettings->ddsd);
		pSettings->ddsd.dwFlags        = DDSD_CAPS; //set the flags to validate both capabilites field adn the backbuffer count field
		pSettings->ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;//tell dd that u have a complex flippable surface
	
		//Createt the primary surface
		ddReturnVal = m_lpddObj->CreateSurface(&pSettings->ddsd,&m_lpddsPrimary,NULL);
		if (DDFailedCheck(ddReturnVal, "CreateSurface() failed", cpErrorBuf ))
		{	MessageBox(m_hWnd, cpErrorBuf, "Init()", MB_ICONEXCLAMATION);   return(E_FAIL); }
		
		//Create an off-screen surface as a backBuffer
		SCCOLOR TransColor((int)pSettings->TransColorR,(int)pSettings->TransColorG,(int)pSettings->TransColorB);
		m_lpddsSecondary = m_Utils.CreateSurface(pSettings->nBackBufferWidth, pSettings->nBackBufferHeight, TransColor);
		if (DDFailedCheck(ddReturnVal, "CreateSurface() failed Second", cpErrorBuf ))
		{	MessageBox(m_hWnd, cpErrorBuf, "Init()", MB_ICONEXCLAMATION);   return(E_FAIL); }

		//Add in any additional surfaces if needed

	}//End if( DDSettings.Windowed )
	else
	{//Set up DD for full screen
		ddReturnVal = m_lpddObj->SetCooperativeLevel(m_hWnd, DDSCL_ALLOWMODEX | DDSCL_FULLSCREEN | DDSCL_EXCLUSIVE | DDSCL_ALLOWREBOOT);
		if (DDFailedCheck(ddReturnVal, "SetCooperativeLevel() Full screen failed", cpErrorBuf ))
		{	MessageBox(m_hWnd, cpErrorBuf, "Init()", MB_ICONEXCLAMATION);   /*return(E_FAIL);*/ }
		
		// set the display mode
		ddReturnVal = m_lpddObj->SetDisplayMode(WINDOW_WIDTH,WINDOW_HEIGHT,BPP,0,0);
		if (DDFailedCheck(ddReturnVal, "SetDisplayMode() Full screen failed", cpErrorBuf ))
		{	MessageBox(m_hWnd, cpErrorBuf, "Init()", MB_ICONEXCLAMATION);   return(E_FAIL); }
		
		// Create the primary surface first set the fields
		memset(&pSettings->ddsd,0,sizeof(pSettings->ddsd));
		pSettings->ddsd.dwSize         = sizeof(pSettings->ddsd);
		pSettings->ddsd.dwFlags        = DDSD_CAPS | DDSD_BACKBUFFERCOUNT; //set the flags to validate both capabilites field adn the backbuffer count field
		pSettings->ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE | DDSCAPS_FLIP | DDSCAPS_COMPLEX; //tell dd that u have a complex flippable surface
		pSettings->ddsd.dwBackBufferCount = pSettings->dwBackBufferCount; //set the back buffer count

		//Createt the primary surface
		ddReturnVal = m_lpddObj->CreateSurface(&pSettings->ddsd,&m_lpddsPrimary,NULL);
		if (DDFailedCheck(ddReturnVal, "SetCooperativeLevel() failed", cpErrorBuf ))
		{	MessageBox(m_hWnd, cpErrorBuf, "Init()", MB_ICONEXCLAMATION);   return(E_FAIL); }
	
		//Query for the backbuffer, use the ddscaps to indicate what you're requesting
		memset(&pSettings->ddscaps,0,sizeof(pSettings->ddscaps));
		pSettings->ddscaps.dwCaps = DDSCAPS_BACKBUFFER;
		//get the surface
		ddReturnVal = m_lpddsPrimary->GetAttachedSurface(&pSettings->ddscaps, &m_lpddsSecondary);
		if (DDFailedCheck(ddReturnVal, "GetAttachedSurface() failed", cpErrorBuf ))
		{	MessageBox(m_hWnd, cpErrorBuf, "Init()", MB_ICONEXCLAMATION);   return(E_FAIL); }
	}//End if( DDSettings.Windowed ) else 
    
	//Attach a clipper
	RECT cliplist[1];
	//create a cliplist
	cliplist[0].top = 0; cliplist[0].left = 0;
	cliplist[0].bottom = pSettings->nBackBufferHeight;
	cliplist[0].right = pSettings->nBackBufferWidth;
	m_Utils.AttachClipper(m_lpddsSecondary,1,cliplist);
	m_Utils.AttachClipper( m_lpddsPrimary,m_hWnd );
	
	//attack a color key
	// set color key to TransparentColor
	DDCOLORKEY color_key; // used to set color key
	
	color_key.dwColorSpaceLowValue  = m_Utils.CreateRGB((int)pSettings->TransColorR,(int)pSettings->TransColorG,(int)pSettings->TransColorB);
	color_key.dwColorSpaceHighValue = m_Utils.CreateRGB((int)pSettings->TransColorR,(int)pSettings->TransColorG,(int)pSettings->TransColorB);
		
	// now set the color key for source blitting
	ddReturnVal  = m_lpddsSecondary->SetColorKey(DDCKEY_SRCBLT, &color_key);
	if (DDFailedCheck(ddReturnVal, "SetColorKey() failed", cpErrorBuf ))
	{	MessageBox(m_hWnd, cpErrorBuf, "Init()", MB_ICONEXCLAMATION);   return(E_FAIL); }
    
	// Success
    return S_OK;
}