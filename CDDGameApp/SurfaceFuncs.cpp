//-----------------------------------------------------------------------------
// File: SurfaceFuncs.cpp
//
// Desc: All surface manipulations, or actions between or to functions
//
//-----------------------------------------------------------------------------


////INCLUDES/////////
#include "CDDGameApp.h"
#include "CreateRGB.h"
#include "CUtilities.h"

////GLOBALS/////////


////FUNCTIONS///////

//-----------------------------------------------------------------------------
// Name: AttachClipper()
// Desc: creates and attaches a clipper to a surface using a window handle
//-----------------------------------------------------------------------------
LPDIRECTDRAWCLIPPER CUtilities::AttachClipper( LPDIRECTDRAWSURFACE7 lpddsSurface, HWND hWnd)
{
	LPDIRECTDRAWCLIPPER lpddClipper;  //pointer to the newly created dd clipper
	HRESULT ddReturnVal = 0;

	//first create the DD clipper
	ddReturnVal = GetGameApp()->getDirectDObj()->CreateClipper( 0,&lpddClipper, NULL );
	if(DDFailedCheck( ddReturnVal, "CreateClipper( ) failed", m_cpErrorBuf ) )
	{ MessageBox( m_hWnd, m_cpErrorBuf, "AttachClipper( )", MB_ICONEXCLAMATION );   return( NULL ); }
	
	ddReturnVal = lpddClipper->SetHWnd( NULL, hWnd );
	if( DDFailedCheck( ddReturnVal, "lpddClipper->SetHWnd( ) failed", m_cpErrorBuf ) )
	{ MessageBox( m_hWnd, m_cpErrorBuf, "AttachClipper( )", MB_ICONEXCLAMATION); return( NULL ); }

	// now attach the clipper to the surface
	ddReturnVal = lpddsSurface->SetClipper( lpddClipper );
	if( DDFailedCheck( ddReturnVal, "SetClipper( ) failed", m_cpErrorBuf ) )
	{	
		MessageBox( m_hWnd, m_cpErrorBuf, "AttachClipper( )", MB_ICONEXCLAMATION );   
		// release memory and return error
		return( NULL ); 
	}

	return(lpddClipper);
}

//-----------------------------------------------------------------------------
// Name: AttachClipper()
// Desc: creates and attaches a clipper to a surface
//-----------------------------------------------------------------------------
LPDIRECTDRAWCLIPPER CUtilities::AttachClipper( LPDIRECTDRAWSURFACE7 lpddsSurface, int iNumRects, LPRECT cliplist )
{
	int index;                        //loop va
	LPDIRECTDRAWCLIPPER lpddClipper;  //pointer to the newly created dd clipper
	LPRGNDATA regionData;             //pointer to the region data that contains the header and clip list
	HRESULT ddReturnVal = 0;

	//first create the DD clipper
	ddReturnVal = GetGameApp()->getDirectDObj()->CreateClipper( 0, &lpddClipper, NULL );
	if( DDFailedCheck( ddReturnVal, "CreateClipper( ) failed", m_cpErrorBuf ) )
	{ MessageBox( m_hWnd, m_cpErrorBuf, "AttachClipper( )", MB_ICONEXCLAMATION );   return( NULL ); }
	
	//now create the clip list from the sent data
	//first allocate memory for region data
	regionData = (LPRGNDATA)malloc( sizeof(RGNDATAHEADER) + iNumRects*sizeof(RECT) );
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
	for( index=0; index<iNumRects; index++ )
    {
		// test if the next rectangle unioned with the current bound is larger
		if ( cliplist[index].top < regionData->rdh.rcBound.top )
			regionData->rdh.rcBound.top = cliplist[index].top;

		if ( cliplist[index].left < regionData->rdh.rcBound.left )
			 regionData->rdh.rcBound.left = cliplist[index].left;

		if ( cliplist[index].bottom > regionData->rdh.rcBound.bottom )
			regionData->rdh.rcBound.bottom = cliplist[index].bottom;

		if ( cliplist[index].right > regionData->rdh.rcBound.right )
			regionData->rdh.rcBound.right = cliplist[index].right;

    } // end for index

	// now we have computed the bounding rectangle region and set up the data
	// now let's set the clipping list
	
	ddReturnVal = lpddClipper->SetClipList(regionData, 0); //can also give lpddClipper a hWnd
	if( DDFailedCheck( ddReturnVal, "SetClipList() failed", m_cpErrorBuf ) )
	{	
		MessageBox( m_hWnd, m_cpErrorBuf, "AttachClipper()", MB_ICONEXCLAMATION );   
		// release memory and return error
		free(regionData);
		return(NULL); 
	}
	
	// now attach the clipper to the surface
	ddReturnVal = lpddsSurface->SetClipper( lpddClipper );
	if( DDFailedCheck( ddReturnVal, "SetClipper( ) failed", m_cpErrorBuf ) )
	{	
		MessageBox( m_hWnd, m_cpErrorBuf, "AttachClipper()", MB_ICONEXCLAMATION );   
		// release memory and return error
		free(regionData);
		return(NULL); 
	}
	
	// all is well, so release memory and send back the pointer to the new clipper
	free( regionData );
	return( lpddClipper );

} // end AttachClipper

//-----------------------------------------------------------------------------
// Name: CreateSurface()
// Desc: creates a offscreen plain surface
//-----------------------------------------------------------------------------
LPDIRECTDRAWSURFACE7 CUtilities::CreateSurface( int width, int height, SCCOLOR TransparentColor )
{// this function creates an offscreen plain surface
	HRESULT ddReturnVal = 0;
	//DDSURFACEDESC2 ddsd;         // working description
	LPDIRECTDRAWSURFACE7 lpdds;   // temporary surface
    
	// set to access caps, width, and height
	memset( &m_ddsd, 0, sizeof(m_ddsd) );
	m_ddsd.dwSize  = sizeof( m_ddsd );
	m_ddsd.dwFlags = DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT;

	// set dimensions of the new bitmap surface
	m_ddsd.dwWidth  =  width;
	m_ddsd.dwHeight =  height;

	// set surface to offscreen plain
	m_ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN;//default is video memory VRAM
	
	// create the surface
	ddReturnVal  = GetGameApp()->getDirectDObj()->CreateSurface( &m_ddsd, &lpdds, NULL );
	if( ddReturnVal == DDERR_OUTOFVIDEOMEMORY )//out of vram
	{
		// set surface to offscreen plain system memory
		m_ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_SYSTEMMEMORY;
		ddReturnVal  = GetGameApp()->getDirectDObj()->CreateSurface( &m_ddsd, &lpdds,NULL );
	}
	if (DDFailedCheck( ddReturnVal, "CreateSurface( ) failed", m_cpErrorBuf ) )//DDERR_OUTOFVIDEOMEMORY DDSCAPS_SYSTEMMEMORY
	{ MessageBox( m_hWnd, m_cpErrorBuf, "CreateSurface( )", MB_ICONEXCLAMATION );   return( NULL ); }
	
	// set color key to TransparentColor
	DDCOLORKEY color_key; // used to set color key
	color_key.dwColorSpaceLowValue  = CreateRGB( TransparentColor.r,TransparentColor.g,TransparentColor.b );
	color_key.dwColorSpaceHighValue = CreateRGB( TransparentColor.r,TransparentColor.g,TransparentColor.b );

	// now set the color key for source blitting
	ddReturnVal  = lpdds->SetColorKey( DDCKEY_SRCBLT, &color_key );
	if( DDFailedCheck(ddReturnVal, "SetColorKey() failed", m_cpErrorBuf ) )
	{	MessageBox(m_hWnd, m_cpErrorBuf, "CreateSurface()", MB_ICONEXCLAMATION);   return(NULL); }

	// return surface
	return( lpdds );

} // end CreateSurface()

/*
//-----------------------------------------------------------------------------
// Name: CopySurface()
// Desc: this function copies one surface to another
//-----------------------------------------------------------------------------   
int CUtilities::CopySurface( LPDIRECTDRAWSURFACE7 lpddsDestSurface, RECT *destRect,LPDIRECTDRAWSURFACE7 lpddsSorSurface, RECT *sorRect )
{// this function flips a surface onto the primary surface
	HRESULT ddReturnVal = 0;
	//Blt the frame
	ddReturnVal = lpddsDestSurface->Blt(destRect, //dest rect
										lpddsSorSurface, //pointer to source surface
										sorRect, //pointer to the source rectangle
										DDBLT_WAIT | DDBLT_KEYSRC, NULL);//NULL means entire surface ddbltfx struct
	if( ddReturnVal!=DD_OK )
		if( ddReturnVal==DDERR_SURFACELOST )
		{	OutputDebugString("Surfacelost CopySurface()\n"); lpddsSorSurface->Restore( ); lpddsDestSurface->Restore( ); return(1);	}
		else
			if( DDFailedCheck(ddReturnVal, "CopySurface(), Blt failed", m_cpErrorBuf ) )
			{ MessageBox( NULL, m_cpErrorBuf, "SurfaceFuncs", MB_ICONEXCLAMATION );   return(0); }
		
	return 1;
}
*/
//-----------------------------------------------------------------------------
// Name: Flip()
// Desc: this function flips a surface onto the primary surface at a certain 
//       piont
//-----------------------------------------------------------------------------   
int CUtilities::Flip( LPDIRECTDRAWSURFACE7 lpddsSurface, int x, int y, int x2, int y2 )
{// this function flips a surface onto the primary surface
	static RECT destrect;
	HRESULT ddReturnVal = 0;
	static char cpErrorBuf[1000];

	destrect.left = x; destrect.top = y;
	destrect.right = x2; destrect.bottom = y2;

	//Blt the frame
	ddReturnVal = GetGameApp()->getPrimarySurface()->Blt( &destrect/*dest rect*/,lpddsSurface, //pointer to source surface
									  NULL, //pointer to the source rectangle
									  DDBLT_WAIT | DDBLT_KEYSRC, NULL/*ddbltfx struct*/ );//NULL means entire surface
	if(ddReturnVal!=DD_OK)
		if(ddReturnVal==DDERR_SURFACELOST)
		{	OutputDebugString("Surfacelost Flip(x,y,...)\n"); lpddsSurface->Restore( ); GetGameApp()->getPrimarySurface()->Restore(); return(1);	}
		else
			if( DDFailedCheck(ddReturnVal, "Flip(lpddsSurface,x,y,...), Blt failed", cpErrorBuf ) )
			{	MessageBox( NULL, cpErrorBuf, "SurfaceFuncs", MB_ICONEXCLAMATION );   return( 0 ); }
		
	return 1;
}

//-----------------------------------------------------------------------------
// Name: Flip()
// Desc: this function flips a surface onto the primary surface
//-----------------------------------------------------------------------------   
int CUtilities::Flip( LPDIRECTDRAWSURFACE7 lpddsSurface, RECT destrect )
{// this function flips a surface onto the primary surface
	HRESULT ddReturnVal = 0;
	static char cpErrorBuf[1000];

	//Blt the frame
	ddReturnVal = GetGameApp()->getPrimarySurface()->Blt( &destrect/*dest rect*/,lpddsSurface, //pointer to source surface
									  NULL, //pointer to the source rectangle
									  DDBLT_WAIT | DDBLT_KEYSRC, NULL/*ddbltfx struct*/ );//NULL means entire surface
	if(ddReturnVal!=DD_OK)
		if(ddReturnVal==DDERR_SURFACELOST)
		{ OutputDebugString("Surfacelost Flip(RECT)\n"); lpddsSurface->Restore(); GetGameApp()->getPrimarySurface()->Restore(); return(1);	}
		else
			if( DDFailedCheck(ddReturnVal, "Flip(lpddsSurface, RECT), Blt failed", cpErrorBuf ) )
			{ MessageBox(NULL, cpErrorBuf, "SurfaceFuncs", MB_ICONEXCLAMATION);   return(0); }
		
	return 1;
}

//-----------------------------------------------------------------------------
// Name: Flip()
// Desc: this function flips a surface onto the primary surface 
//       primary surface expected to be a normal window (non-fullscreen)
//       This blts to entire window.
//-----------------------------------------------------------------------------   
int CUtilities::FlipToWindow( LPDIRECTDRAWSURFACE7 lpddsSurface )
{// this function flips a surface onto the primary surface
	static RECT temp;
	HRESULT ddReturnVal = 0;
	static char cpErrorBuf[1000];

	GetWindowRect( m_hWnd, &temp );
	
	//Blt the frame
	ddReturnVal = GetGameApp()->getPrimarySurface()->Blt(&temp/*dest rect*/,lpddsSurface, //pointer to source surface
									  NULL, //pointer to the source rectangle
									  DDBLT_WAIT | DDBLT_KEYSRC, NULL/*ddbltfx struct*/);//NULL means entire surface
	if( ddReturnVal!=DD_OK )
		if( ddReturnVal==DDERR_SURFACELOST )
		{ OutputDebugString("Surfacelost FlipToWindow()\n"); lpddsSurface->Restore(); GetGameApp()->getPrimarySurface()->Restore(); return(1);	}
		else
			if ( DDFailedCheck(ddReturnVal, "FlipToWindow(lpddsSurface), Blt failed", cpErrorBuf ) )
			{ MessageBox(NULL, cpErrorBuf, "SurfaceFuncs", MB_ICONEXCLAMATION);   return( 0 ); }
		
	return 1;
}

//-----------------------------------------------------------------------------
// Name: Flip()
// Desc: this function flips a surface onto the primary surface 
//       primary surface expected to be a normal window (non-fullscreen).
//       flips to client area only
//-----------------------------------------------------------------------------   
int CUtilities::FlipToClient( LPDIRECTDRAWSURFACE7 lpddsSurface )
{// this function flips a surface onto the primary surface
	static RECT temp;
	HRESULT ddReturnVal = 0;
	static char cpErrorBuf[1000];

	GetWindowRect( m_hWnd, &temp );
	temp.top +=20;
	temp.left +=4;
	temp.right -=4;
	temp.bottom -=4;
	//Blt the frame
	ddReturnVal = GetGameApp()->getPrimarySurface()->Blt(&temp/*dest rect*/,lpddsSurface, //pointer to source surface
														 NULL, //pointer to the source rectangle
														 DDBLT_WAIT | DDBLT_KEYSRC, NULL/*ddbltfx struct*/);//NULL means entire surface
	if( ddReturnVal!=DD_OK )
		if( ddReturnVal==DDERR_SURFACELOST )
		{	OutputDebugString("Surfacelost FlipToClient( )\n"); lpddsSurface->Restore(); GetGameApp()->getPrimarySurface()->Restore(); return(1);	}
		else
			if( DDFailedCheck(ddReturnVal, "FlipToClient(lpddsSurface), Blt failed", cpErrorBuf ) )
			{ MessageBox(NULL, cpErrorBuf, "SurfaceFuncs", MB_ICONEXCLAMATION);   return( 0 ); }
		
	return 1;
}

//-----------------------------------------------------------------------------
// Name: Flip()
// Desc: this function flip the primary surface with the secondary surface
//-----------------------------------------------------------------------------   
int CUtilities::Flip()
{// this function flip the primary surface with the secondary surface
	HRESULT ddReturnVal = 0;
	static char cpErrorBuf[1000];

	// flip pages
	while( ( ddReturnVal=GetGameApp()->getPrimarySurface()->Flip(NULL, DDFLIP_WAIT) ) != DD_OK )
	{
		if( ddReturnVal!=DD_OK )
			if( ddReturnVal==DDERR_SURFACELOST )
			{ OutputDebugString("Surfacelost Flip( )\n"); GetGameApp()->getPrimarySurface()->Restore( ); return( 1 ); }
			else
				if( DDFailedCheck( ddReturnVal, "Flip( )", cpErrorBuf ) )
				{ MessageBox(NULL, cpErrorBuf, "SurfaceFuncs", MB_ICONEXCLAMATION);   return( 0 ); }
	}

	// return success
	return( 1 );

} // end DD_Flip
/*
//-----------------------------------------------------------------------------
// Name: Lock()
// Desc: Fucntion to lock the entire surface
//-----------------------------------------------------------------------------
int CUtilities::Lock( LPDIRECTDRAWSURFACE7 lpddsSurface, DDSURFACEDESC2 &ddsd )
{
	HRESULT ddReturnVal = 0;
	static char cpErrorBuf[1000];

	memset(&ddsd,0,sizeof(ddsd));
	ddsd.dwSize = sizeof(ddsd);
	
	ddReturnVal  = lpddsSurface->Lock(NULL,&ddsd,DDLOCK_SURFACEMEMORYPTR | DDLOCK_WAIT, NULL);
	if( DDFailedCheck(ddReturnVal, "Lock( ), failed", cpErrorBuf ) )
	{ MessageBox(NULL, cpErrorBuf, "SurfaceFuncs, Lock( )", MB_ICONEXCLAMATION ); return( 0 ); }
	
	return (1);
}

//-----------------------------------------------------------------------------
// Name: UnLock()
// Desc: UnLocks entire surface that has been locked
//-----------------------------------------------------------------------------
int CUtilities::UnLock( LPDIRECTDRAWSURFACE7 lpddsSurface )
{
	HRESULT ddReturnVal = 0;
	static char cpErrorBuf[1000];

	ddReturnVal  = lpddsSurface->Unlock(NULL);
	if ( DDFailedCheck(ddReturnVal, "Unlock( ), failed", cpErrorBuf ) )
	{ MessageBox( NULL, cpErrorBuf, "UnLock( )", MB_ICONEXCLAMATION ); return( 0 ); }
	
	return(1);
}*/