//Needs CDDSettings

#ifndef _DDInitialize_h
#define _DDInitialize_h

////INCLUDES//////////////////////////
#include <ddraw.h>
#include "General.h"
#include "CreateRGB.h"
#include "CUtilities.h"

////DEFINES///////////////////////////

////MACROS////////////////////////////
#define DDRAW_INIT_STRUCT( ddStruct ) { memset(&ddStruct,0,sizeof(ddStruct)); ddStruct.dwSize=sizeof(ddStruct); }

////CLASSES///////////////////////////

//-----------------------------------------------------------------------------
// Main Class Declarations
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Name : CDDInitialize (Class)
// Desc : DirectDraw Initialization class. 
//-----------------------------------------------------------------------------
class CDDInitialize
{
private:
    //-------------------------------------------------------------------------
	// Private Functions for This Class
	//-------------------------------------------------------------------------
	    
    //-------------------------------------------------------------------------
	// Private Variables For This Class
	//-------------------------------------------------------------------------
	LPDIRECTDRAW7           m_lpddObj;           // dd object
	LPDIRECTDRAWSURFACE7    m_lpddsPrimary;      // dd primary surface
	LPDIRECTDRAWSURFACE7    m_lpddsSecondary;    // dd back surface
    HWND                    m_hWnd;              // Created window handle
	CUtilities              m_Utils;


public:
    //-------------------------------------------------------------------------
	// Constructors & Destructors for This Class.
	//-------------------------------------------------------------------------
	         CDDInitialize();
			 virtual ~CDDInitialize();

	//-------------------------------------------------------------------------
	// Public Functions for This Class
	//-------------------------------------------------------------------------
    HRESULT                 CreateDisplay          ( CDDSettings& DDSettings, ULONG Flags = 0, HWND hWnd = NULL, WNDPROC pWndProc = NULL,
                                                     LPCTSTR Title = NULL, LPVOID lParam = NULL );

    HRESULT                 ResetDisplay           ( LPDIRECTDRAWSURFACE7, LPDIRECTDRAWSURFACE7, CDDSettings& D3DSettings, HWND hWnd = NULL );
    bool 					Enumerate(LPDIRECTDRAW7 lpddObj);

    HWND                    getHWND( )              { return m_hWnd; }
    LPDIRECTDRAW7           getDirectDDObj( )       { return m_lpddObj; }
	LPDIRECTDRAWSURFACE7    getPrimarySurface( )    { return m_lpddsPrimary; }
	LPDIRECTDRAWSURFACE7    getSecondarySurface( )  { return  m_lpddsSecondary; }

	void                    setUtilities( CUtilities u ) { m_Utils = u; }
	void                    setDirectDObj( LPDIRECTDRAW7 );
};

#endif
