//-----------------------------------------------------------------------------
// File: CUtilities.cpp
//
// Desc: 
//-----------------------------------------------------------------------------

////INCLUDES//////////////////////
#include "CUtilities.h"
#include "CDDGameApp.h"

//-----------------------------------------------------------------------------
// CUtilities Member Functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Name : CUtilities() (Constructor)
// Desc : CUtilities Class Constructor
//-----------------------------------------------------------------------------
CUtilities::CUtilities() : CDisplay()
{
	//m_hWnd           = NULL;          
	m_hDC            = NULL;
	m_rcWindow.bottom = m_rcWindow.left = m_rcWindow.right = m_rcWindow.top = 0;
	memset( &m_ddscaps, 0, sizeof(m_ddscaps) );
	DDRAW_INIT_STRUCT( m_ddbltfx );
	DDRAW_INIT_STRUCT( m_ddsd );
}

//-----------------------------------------------------------------------------
// Name : CUtilities() (Constructor)
// Desc : CUtilities Class copy Constructor
//-----------------------------------------------------------------------------
CUtilities::CUtilities( CUtilities &util )
{
	m_hWnd = util.getWindowHandle();          
	m_hDC  = util.getHandleDC();
	m_rcWindow = util.getWndRect();
	
	//m_lpddObj = util.getDirectDObj(); 
	//if( m_lpddObj != NULL ) m_lpddObj->AddRef();

	//m_lpddsPrimary = util.getPrimarySurface();
	//if( m_lpddsPrimary != NULL ) m_lpddsPrimary->AddRef();

	//m_lpddsSecondary = util.getSecondarySurface();
	//if( m_lpddsSecondary != NULL ) m_lpddsSecondary->AddRef();
	
	m_ddscaps = util.getDirectDSurfaceCaps();       
	m_ddbltfx = util.getDirectDBlitFx();       
	m_ddsd = util.getDirectDSurfDesc();
}

//-----------------------------------------------------------------------------
// Name : CUtilities() (Constructor)
// Desc : CUtilities Class Constructor
//-----------------------------------------------------------------------------
/*
CUtilities::CUtilities( LPDIRECTDRAW7 lpddObj)
{
    m_lpddObj = lpddObj;
	if( m_lpddObj != NULL ) m_lpddObj->AddRef();

	m_hWnd           = NULL;          
	m_hDC            = NULL;
	m_wndRect.bottom = m_wndRect.left = m_wndRect.right = m_wndRect.top = 0;
	m_lpddsPrimary   = NULL;  
	m_lpddsSecondary = NULL;
	memset( &m_ddscaps, 0, sizeof(m_ddscaps) );
	DDRAW_INIT_STRUCT( m_ddbltfx );
	DDRAW_INIT_STRUCT( m_ddsd );
}
*/
//-----------------------------------------------------------------------------
// Name : CUtilities() (Destructor)
// Desc : CUtilities Class Denstructor
//-----------------------------------------------------------------------------
CUtilities::~CUtilities()
{
	//if( m_lpddsSecondary != NULL ) { m_lpddsSecondary->Release( ); m_lpddsSecondary = NULL; }
	//if( m_lpddsPrimary != NULL )   { m_lpddsPrimary->Release( );   m_lpddsPrimary = NULL;   }
	//if( m_lpddObj != NULL ) m_lpddObj->Release( );
	m_hWnd = NULL;          
	m_hDC  = NULL;
	m_rcWindow.bottom = m_rcWindow.left = m_rcWindow.right = m_rcWindow.top = 0;
	//memset( &m_ddscaps, 0, sizeof(m_ddscaps) );
	//DDRAW_INIT_STRUCT( m_ddbltfx );
	//DDRAW_INIT_STRUCT( m_ddsd );
}

CUtilities& CUtilities::operator =(CUtilities &u)
{
	//m_lpddObj = u.getDirectDObj();
	//if( m_lpddObj != NULL ) m_lpddObj->AddRef();

	m_hWnd           = u.getWindowHandle();          
	m_hDC            = u.getHandleDC();
	m_rcWindow        = u.getWndRect();
	
	//m_lpddsPrimary   = u.getPrimarySurface(); 
	//if( m_lpddsPrimary != NULL ) m_lpddsPrimary->AddRef();
	//m_lpddsSecondary = u.getSecondarySurface();
	//if( m_lpddsSecondary != NULL ) m_lpddsSecondary->AddRef();

	m_ddscaps        = u.getDirectDSurfaceCaps();
	m_ddbltfx        = u.getDirectDBlitFx();
	m_ddsd           = u.getDirectDSurfDesc();

	return(*this);
}

//-----------------------------------------------------------------------------
// Name : CreateRGB( )
// Desc : biulds a x bit color
//-----------------------------------------------------------------------------
unsigned int CUtilities::CreateRGB( int r, int g, int b, int a )
{
	switch ( GetGameApp()->getSettings().GetSettings()->nBPP )
	{
	case 8:
		// Here you should do a palette lookup to find the closes match.
		// I'm not going to bother with that. Many modern games no
		// longer support 256-color modes, and neither should you :)
		return 0;
	case 16:
		return ((b&31)+((g&63)<<5)+((r&31)<<11));
	case 24:
		return ( b + (g<<8) + (r<<16) + (a<<24) );
	case 32:
		return ( b + (g<<8) + (r<<16) + (0<<24) );
	}
	return 0;
}
/*
//-----------------------------------------------------------------------------
// Name : Release( )
// Desc : releases surfaces and com objects for recreation of display 
//-----------------------------------------------------------------------------
bool CUtilities::Release( )
{
	if( m_lpddsSecondary != NULL ) { m_lpddsSecondary->Release(); m_lpddsSecondary = NULL; }
	if( m_lpddsPrimary != NULL )   { m_lpddsPrimary->Release(); m_lpddsPrimary = NULL; }
	if( m_lpddObj != NULL )        { m_lpddObj->Release(); m_lpddObj = NULL; }
	return true;
}
*/
//-----------------------------------------------------------------------------
// Name : setDirectDObj( )
// Desc : 
//-----------------------------------------------------------------------------
void CUtilities::setDirectDObj( LPDIRECTDRAW7 lpddObj )
{ 
	m_pDD = lpddObj; 
}
/*
//-----------------------------------------------------------------------------
// Name : setPrimarySurface( )
// Desc : 
//-----------------------------------------------------------------------------
void CUtilities::setPrimarySurface( LPDIRECTDRAWSURFACE7 lpddsPrimary )
{ 
	if( lpddsPrimary == NULL ) return;
	if( m_lpddsPrimary != NULL ) m_lpddsPrimary->Release();
	m_lpddsPrimary = lpddsPrimary; 
	m_lpddsPrimary->AddRef( ); 
}

//-----------------------------------------------------------------------------
// Name : setSecondarySurface( )
// Desc : 
//-----------------------------------------------------------------------------
void CUtilities::setSecondarySurface( LPDIRECTDRAWSURFACE7 lpddsSecondary )  
{ 
	if( lpddsSecondary == NULL ) return;
	if(m_lpddsSecondary != NULL) m_lpddsSecondary->Release();
	m_lpddsSecondary = lpddsSecondary; 
	m_lpddsSecondary->AddRef( );  
}
*/
/*
//-----------------------------------------------------------------------------
// Name : CreateRGB()
// Desc : Makes a color
//-----------------------------------------------------------------------------
unsigned int CUtilities::CreateRGB( int r, int g, int b, int a )
{
	switch ( m_Settings.GetSettings()->nBPP )
	{
	case 8:
		// Here you should do a palette lookup to find the closes match.
		// I'm not going to bother with that. Many modern games no
		// longer support 256-color modes, and neither should you :)
		return 0;
	case 16:
		return ((b&31)+((g&63)<<5)+((r&31)<<11));
	case 24:
		return ( b + (g<<8) + (r<<16) + (a<<24) );
	case 32:
		return ( b + (g<<8) + (r<<16) + (0<<24) );
	}
	return 0;
}
*/