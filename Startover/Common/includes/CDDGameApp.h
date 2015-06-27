//---------------------------------------------------------------------
// File: CDDGameApp.h
//
// Desc: Definition of a class for DirectDraw setup
//
// Notes: There is a problem with all the creating and destroying of
//        the dd object. Not all of them are being destroy there are
//        some remaining at the end of the program, so in gameshutdowm
//        i added a while loop to remove them all. this is a problem
//        either get rid of all the AddRef or find the leak
//---------------------------------------------------------------------

#ifndef CDDGameApp_h
#define CDDGameApp_h

////INCLUDES////////////////////////////////////////
#include "Bitmap.h"
#include "SCTimer.h"
#include "Sprite.h"
#include "DDInitialize.h";
#include "General.h"
#include "CUtilities.h"

//-----------------------------------------------------------------------------
// Forward Declarations
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Main Class Declarations
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// Name : CDDGameApp (Class)
// Desc : Central game engine, initialises the game and handles core processes.
//-----------------------------------------------------------------------------
class CDDGameApp
{
protected:
    //-------------------------------------------------------------------------
	// Private Variables For This Class
	//-------------------------------------------------------------------------
    CTIMER3                 m_Timer;            // Game timer
    ULONG                   m_LastFrameRate;    // Used for making sure we update only when fps changes.

	CUtilities              m_Utils;
    
    HWND                    m_hWnd;             // Main window HWND
    HICON                   m_hIcon;            // Window Icon
    HMENU                   m_hMenu;            // Window Menu
	char                    m_WindowTitle[40];
    
    bool                    m_bLostDevice;      // Is the 3d device currently lost ?
    bool                    m_bActiveApp;          // Is the application active ?
    bool                    m_bDebugMode;      
	bool                    m_bExitApp;
     
    POINT                   m_OldCursorPos;     // Old cursor position for tracking
	RECT                    m_wndRect;
	bool                    m_pbKeyStatus[256];

	CSPRITE                *m_pSprites;
	int                   **m_nppAnimationList;

	LPDIRECTDRAW7           m_lpddObj;           // dd object
	LPDIRECTDRAWSURFACE7    m_lpddsPrimary;      // dd primary surface
	LPDIRECTDRAWSURFACE7    m_lpddsSecondary;    // dd back surface
	LPDIRECTDRAWPALETTE     m_lpddpal;           // a pointer to the created dd palette
	//DDBLTFX                 m_ddbltfx;           // The Blit Fx structure used for all blting
	CDDSettings             m_DDSettings;       // The settings used to initialize DD and windows

public:
    //-------------------------------------------------------------------------
	// Constructors & Destructors for This Class.
	//-------------------------------------------------------------------------
	         CDDGameApp( );
	virtual ~CDDGameApp();

	//-------------------------------------------------------------------------
	// Public Functions for This Class
	//-------------------------------------------------------------------------
    virtual LRESULT     DisplayWndProc     ( HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam );
	virtual bool        InitInstance       ( HANDLE hInstance, LPCTSTR lpCmdLine, int iCmdShow );
	bool                setDisplaySettings ( bool windowed = false, int bitsPerPixel = 32, int windowWidth = 640, 
									  int windowHeight = 480, int backBufferHeight = 640, int backBufferWidth = 480, 
									  RECT windowRect = RECT(), SCCOLOR  transparentColor = SCCOLOR(255,0,255) );
    virtual int          BeginGame   ( );
	virtual bool         ShutDown    ( );
	bool                 ReleaseCOMs ( );

	CDDSettings          getSettings       ( ) { return m_DDSettings; }
	HWND                 getWndHandle      ( ) { return m_hWnd; }
	RECT                 getWndRect        ( ) { return m_wndRect; } 
	CUtilities           getUtlities       ( ) { return m_Utils; }
	CTIMER3              getTimer          ( ) { return m_Timer; }
	LPDIRECTDRAW7		 getDirectDObj     ( ) { return m_lpddObj; }
	LPDIRECTDRAWSURFACE7 getPrimarySurface ( ) { return m_lpddsPrimary; }
	LPDIRECTDRAWSURFACE7 getSecondarySurf  ( ) { return m_lpddsSecondary; }

	void setDebugMode     ( bool mode) { m_bDebugMode =  mode; }
	void setPrimarySurf   ( LPDIRECTDRAWSURFACE7 );
	void setSecondarySurf ( LPDIRECTDRAWSURFACE7 );

protected:
    //-------------------------------------------------------------------------
	// protected Functions for This Class
	//-------------------------------------------------------------------------
    //void        FrameAdvance          ( );
    bool        CreateDisplay          ( );
	void        UpdateUtilitiesClasses ( );
		
	bool        getKeyStatus      ( int key );

	//-------------------------------------------------------------------------
	// protected virtual Functions for This Class
	//-------------------------------------------------------------------------
	virtual bool BuildSprites      ( );
    virtual void ReleaseSprites    ( );
	virtual void SetupGameState    ( );
	virtual void SetupRenderState  ( );
	virtual void AnimateObjects    ( ) { }
	virtual void ProcessInput      ( ) { }
	virtual void FrameAdvance      ( );
    //-------------------------------------------------------------------------
	// Private Static Functions For This Class
	//-------------------------------------------------------------------------
    static LRESULT CALLBACK StaticWndProc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam);
};

#endif