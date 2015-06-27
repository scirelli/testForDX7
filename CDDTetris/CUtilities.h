//---------------------------------------------------------------------
// File: CUtilities.h
//
// Desc: Definition of a class for all Utility methods
//---------------------------------------------------------------------

#ifndef CUtilities_h
#define CUtilities_h

//// INCLUDES /////////////////////////
#include "General.h"
#include "SCMath.h"
#include "CreateRGB.h"
#include "ErrorCheck.h"
#include "ddutil.h"

//-----------------------------------------------------------------------------
//Structures
//-----------------------------------------------------------------------------
struct SCCOLOR
{
	int r;
	int g;
	int b;
	
	SCCOLOR() : r(0), g(0), b(0) 
	{ }
	SCCOLOR( int red, int gr, int bl ) { r = red; g = gr; b = bl; }
	SCCOLOR( SCCOLOR &c) { r = c.r; g = c.g; b = c.b; }
};

//--------------------------------------------
// Class: CUtilites
//--------------------------------------------
class CUtilities : CDisplay
{
private:
	//---------------------------------
	// Private member variables
	//---------------------------------
	HDC                     m_hDC;
	DDSCAPS2                m_ddscaps;          // a direct draw surface capabilities struct
	DDBLTFX                 m_ddbltfx;          // The Blit Fx structure used for all blting
	DDSURFACEDESC2          m_ddsd;
	CDDSettings             m_Settings;
	char                    m_cpErrorBuf[1000];

	//---------------------------------
	// Private methods
	//---------------------------------

public:
	//---------------------------------
	// Public member varuables 973-443-8500 FDU CDS
	//---------------------------------
	
	//-----------------------------------------------
	// Constructors & Destructors for This Class.
	//-----------------------------------------------
	CUtilities();
	CUtilities( CUtilities& );
	//CUtilities( LPDIRECTDRAW7 );
	//CUtilities( LPDIRECTDRAW7, LPDIRECTDRAWSURFACE7, LPDIRECTDRAWSURFACE7 );
	~CUtilities();

	//---------------------------------
	// Public mothods
	//---------------------------------
	HWND                 getWindowHandle       ( )  { return m_hWnd; }
	HDC                  getHandleDC           ( )  { return m_hDC; }
	RECT                 getWndRect            ( )  { return m_rcWindow; }
	//LPDIRECTDRAW7        getDirectDObj         ( )  { return m_lpddObj; }
	//LPDIRECTDRAWSURFACE7 getPrimarySurface     ( )  { return m_lpddsPrimary; }
	//LPDIRECTDRAWSURFACE7 getSecondarySurface   ( )  { return m_lpddsSecondary; }
	DDSCAPS2             getDirectDSurfaceCaps ( )  { return m_ddscaps; }
	DDBLTFX              getDirectDBlitFx      ( )  { return m_ddbltfx; }
	DDSURFACEDESC2       getDirectDSurfDesc    ( )  { return m_ddsd; }
	const char*          getErrorBuffer        ( )  { return m_cpErrorBuf; }
	CDDSettings          getSettings           ( )  { return m_Settings; }

	void setWindowHandle       ( HWND hWnd )                            { m_hWnd = hWnd; }
	void setHandleDC           ( HDC hDC )                              { m_hDC = hDC; }
	void setWndRect            ( RECT wndRect )                         { m_rcWindow = wndRect; }
	void setDirectDObj         ( LPDIRECTDRAW7 lpddObj );
	//void setPrimarySurface     ( LPDIRECTDRAWSURFACE7 lpddsPrimary );
	//void setSecondarySurface   ( LPDIRECTDRAWSURFACE7 lpddsSecondary );
	void setDirectDSurfaceCaps ( DDSCAPS2 ddscaps )                     { m_ddscaps = ddscaps; }
	void setDirectDBlitFx      ( DDBLTFX ddbltfx )                      { m_ddbltfx = ddbltfx; }
	void setDirectDSurfDesc    ( DDSURFACEDESC2 ddsd )                  { m_ddsd = ddsd; }
	void setSettingsClass      ( CDDSettings settings )                 { m_Settings  = settings; }

	//bool Release( );
	CUtilities& operator = (CUtilities &u);

	//Color Methods
	static unsigned int CreateRGB( int r, int g, int b, int a = 0 );

	//---------------------------------
	// Public mothods Surface methods
	//---------------------------------
	LPDIRECTDRAWCLIPPER  AttachClipper  ( LPDIRECTDRAWSURFACE7 lpddsSurface, HWND );
	LPDIRECTDRAWCLIPPER  AttachClipper  ( LPDIRECTDRAWSURFACE7 lpddsSurface, int iNumRects, LPRECT cliplist );
	LPDIRECTDRAWSURFACE7 CreateSurface  ( int width, int height, SCCOLOR TransparentColor );
	static int Flip                     ( );
	int FlipToClient                    ( LPDIRECTDRAWSURFACE7 lpddsSurface);
	int FlipToWindow                    ( LPDIRECTDRAWSURFACE7 );
	static int Flip                     ( LPDIRECTDRAWSURFACE7 lpddsSurface, RECT destrect );
	static int Flip                     ( LPDIRECTDRAWSURFACE7 lpddsSurface, int x, int y, int x2, int y2 );
	
	//---------------------------------
	// Static inline public mothods Surface methods
	//---------------------------------
	
	//-----------------------------------------------------------------------------
	// Name: CopySurface()
	// Desc: this function copies one surface to another
	//----------------------------------------------------------------------------- 
	static inline int CopySurface( LPDIRECTDRAWSURFACE7 lpddsDestSurface, RECT *destRect,LPDIRECTDRAWSURFACE7 lpddsSorSurface, RECT *sorRect )
	{// this function flips a surface onto the primary surface
		HRESULT ddReturnVal = 0;
		char cpErrorBuf[100];
		//Blt the frame
		ddReturnVal = lpddsDestSurface->Blt(destRect/*dest rect*/,lpddsSorSurface, //pointer to source surface
											sorRect, //pointer to the source rectangle
											DDBLT_WAIT | DDBLT_KEYSRC, NULL/*ddbltfx struct*/);//NULL means entire surface
		if( ddReturnVal!=DD_OK )
			if( ddReturnVal==DDERR_SURFACELOST )
			{	OutputDebugString("Surfacelost CopySurface()\n"); lpddsSorSurface->Restore( ); lpddsDestSurface->Restore( ); return(1);	}
			else
				if( DDFailedCheck(ddReturnVal, "CopySurface(), Blt failed", cpErrorBuf ) )
				{ MessageBox( NULL, cpErrorBuf, "SurfaceFuncs", MB_ICONEXCLAMATION );   return(0); }
			
		return 1;
	}
	
	//-----------------------------------------------------------------------------
	// Name: Lock()
	// Desc: Fucntion to lock the entire surface
	//-----------------------------------------------------------------------------
	static inline int Lock( LPDIRECTDRAWSURFACE7 lpddsSurface, DDSURFACEDESC2 &ddsd )
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
	static inline int  UnLock( LPDIRECTDRAWSURFACE7 lpddsSurface )
	{
		HRESULT ddReturnVal = 0;
		static char cpErrorBuf[1000];

		ddReturnVal  = lpddsSurface->Unlock(NULL);
		if ( DDFailedCheck(ddReturnVal, "Unlock( ), failed", cpErrorBuf ) )
		{ MessageBox( NULL, cpErrorBuf, "UnLock( )", MB_ICONEXCLAMATION ); return( 0 ); }
	
		return(1);
	}

	//-----------------------------------------------
	// Stiatic inline public methods Primitive Shapes
	//-----------------------------------------------
	typedef struct PIXEL
	{
		CVECTOR velVector();
		float x;
		float y;
		float vx;
		float vy;
		float vx0;
		float vy0;
		float initvel;
		float angle;
		float gravity;
		int   fadefactor;    //duration in milliseconds or units
		int   r,g,b;
		SCCOLOR col;
		bool Active;   //is this particle dead or alive
	}PARTICLE, *PTR_PARTICLE;

	typedef struct _tParticalExplosion
	{
		PARTICLE  particle[MAX_PARTICLES]; //list of particles making up this effect
		float initVel;  //initial vel, or strength, of effect
		float fStartTime;
		int   x;        //initial x location
		int   y;        //initial y location
		bool  Active;   //is effect active or dead
	} tParticalExplotion;

	
	//-----------------------------------------------------------------------------
	// Name: DrawPixel()
	// Desc: draws a 16bit pixel to a surface
	//-----------------------------------------------------------------------------
	static inline bool DrawPixel(DDSURFACEDESC2 ddsd, int x, int y, SCCOLOR col)
	{
		unsigned short *video_buffer= (unsigned short*)ddsd.lpSurface;
			
		video_buffer[x + (y*ddsd.lPitch>>1)]= (unsigned short)BIULDCOLOR(col.r,col.g,col.b);

		return true;
	}

	//-----------------------------------------------------------------------------
	// Name: DrawPixel(DDSURFACEDESC2 ddsd, int x, int y, int r, int g, int b)
	// Desc: draws a 16bit pixel to a surface
	//-----------------------------------------------------------------------------
	static inline bool DrawPixel(DDSURFACEDESC2 ddsd, int x, int y, int r, int g, int b)
	{
		unsigned short *video_buffer= (unsigned short*)ddsd.lpSurface;
		
		video_buffer[x + (y*ddsd.lPitch>>1)]= (unsigned short)BIULDCOLOR((int)r,(int)g,(int)b);

		return true;
	}

	//-----------------------------------------------------------------------------
	// Name: DrawPixel(DDSURFACEDESC2 ddsd, int lPitch, int x, int y, int r, int g, int b)
	// Desc: draws a 16bit pixel to a surface
	//-----------------------------------------------------------------------------
	static inline bool DrawPixel(USHORT *video_buffer, int lPitch, int x, int y, int r, int g, int b)
	{
		video_buffer[x + (y*lPitch)]= (unsigned short)BIULDCOLOR((int)r,(int)g,(int)b);
		return true;
	}

	//-----------------------------------------------------------------------------
	// Name: DrawPixel32()
	// Desc: draws a 32bit pixel to a surface
	//-----------------------------------------------------------------------------
	static inline bool CUtilities::DrawPixel32(DDSURFACEDESC2 ddsd, int x, int y, SCCOLOR col)
	{
		DWORD *video_buffer= (DWORD*)ddsd.lpSurface;
			
		video_buffer[x + (y*(ddsd.lPitch>>2))]= (DWORD)BIULDCOLOR(col.r,col.g,col.b);

		return true;
	}

	//-----------------------------------------------------------------------------
	// Name: DrawPixel32(DDSURFACEDESC2 ddsd, int x, int y, int r, int g, int b)
	// Desc: draws a 32bit pixel to a surface
	//-----------------------------------------------------------------------------
	static inline bool DrawPixel32(DDSURFACEDESC2 ddsd, int x, int y, int r, int g, int b)
	{
		DWORD *video_buffer= (DWORD*)ddsd.lpSurface;
		
		video_buffer[x + (y*(ddsd.lPitch>>2))]= (DWORD)BIULDCOLOR((int)r,(int)g,(int)b);

		return true;
	}

	//-----------------------------------------------------------------------------
	// Name: DrawPixel32(DWORD *video_buffer, int lpitch, int x, int y, int r, int g, int b)
	// Desc: draws a 32bit pixel to a surface
	//-----------------------------------------------------------------------------
	static inline bool DrawPixel32(DWORD *video_buffer, int lPitch, int x, int y, int r, int g, int b)
	{	
		video_buffer[x + (y*lPitch)]= (DWORD)BIULDCOLOR((int)r,(int)g,(int)b);
		return true;
	}

	//-----------------------------------------------------------------------------
	// Name: DrawParticle()
	// Desc: draws an array of particles
	//-----------------------------------------------------------------------------
	static inline bool DrawParticle(DDSURFACEDESC2 ddsd, PARTICLE *particle, int nCurrentMaxParticles)
	{
		int i=0;

		unsigned short *video_buffer= (unsigned short*)ddsd.lpSurface;
		
		while(i <= nCurrentMaxParticles-1)
		{
			video_buffer[((int)particle[i].x) + (((int)particle[i].y)*ddsd.lPitch>>1)]= (unsigned short)BIULDCOLOR(particle[i].col.r,particle[i].col.g,particle[i].col.b);
			i++;
		}

		return true;
	}

	//-----------------------------------------------------------------------------
	// Name: DrawParticle32()
	// Desc: draws an array of particles
	//-----------------------------------------------------------------------------
	static inline bool DrawParticle32(DDSURFACEDESC2 ddsd, PARTICLE *particle, int nCurrentMaxParticles)
	{
		DWORD *video_buffer= (DWORD*)ddsd.lpSurface;
		
		for(int i=0; i<nCurrentMaxParticles; i++)
		{
			video_buffer[((int)particle[i].x) + (((int)particle[i].y)*ddsd.lPitch>>2)]= (DWORD)BIULDCOLOR(particle[i].col.r,particle[i].col.g,particle[i].col.b);
		}
		return true;
	}

	//-----------------------------------------------------------------------------
	// Name: CSurface::DrawText()
	// Desc: Draws a text string on a DirectDraw surface using hFont or the default
	//       GDI font if hFont is NULL.  
	//-----------------------------------------------------------------------------
	static inline HRESULT CUtilities::DrawTextGDI( LPDIRECTDRAWSURFACE7 lpddsSurface,
												   char cpFontName[], int nTextSize, TCHAR* strText, 
												   DWORD dwOriginX, DWORD dwOriginY,
												   SCCOLOR *scBackground, SCCOLOR scForeground )
	{
	    HDC     hDC = NULL;
	    HRESULT hr=NULL;
		HFONT hFont=NULL;
		LOGFONT lf;
		COLORREF crBackground,
				 crForeground = RGB(scForeground.r,scForeground.g,scForeground.b);
		
		if( lpddsSurface == NULL || strText == NULL )
	        return E_INVALIDARG;

	    // Make sure this surface is restored.
	    if( FAILED( hr = lpddsSurface->Restore() ) )
	        return hr;

	    if( FAILED( hr = lpddsSurface->GetDC( &hDC ) ) )
	        return hr;

	    // Set the background and foreground color
	    if( scBackground != NULL)
		{
			crBackground = RGB(scBackground->r,scBackground->g,scBackground->b);
			SetBkColor( hDC, crBackground );
		}
		else
			SetBkMode( hDC, TRANSPARENT );

	    SetTextColor( hDC, crForeground );
		
		if( cpFontName )
		{
			memset(&lf, 0, sizeof(LOGFONT));       // zero out structure
			lf.lfHeight = nTextSize;               // request a 12-pixel-height font
			strcpy(lf.lfFaceName, cpFontName);     // request a face name "Arial"
			hFont = CreateFontIndirect(&lf);       // create the font
			SelectObject( hDC, hFont );
		}
					
	    // Use GDI to draw the text on the surface
	    TextOut( hDC, dwOriginX, dwOriginY, strText,(int) _tcslen(strText) );

	    if( FAILED( hr = lpddsSurface->ReleaseDC( hDC ) ) )
	        return hr;

	    return S_OK;
	}

	//-----------------------------------------------------------------------------
	// Name: DrawStringGDI()
	// Desc: Draws a string to a surface using GDI
	//-----------------------------------------------------------------------------
	static inline int DrawStringGDI( LPDIRECTDRAWSURFACE7 lpddsSurface, int x, int y, const char *string )
	{
		HDC hDC;
		HRESULT ddReturnVal = 0;
		static char cpErrorBuf[100];
		//Write text to the Secondary surface remember to Release the DC
		ddReturnVal = lpddsSurface->GetDC( &hDC );
		if( ddReturnVal != DD_OK )
			if( ddReturnVal == DDERR_SURFACELOST )
				{ OutputDebugString("DrawStringGDI( ) Surface lost\n"); lpddsSurface->Restore( ); lpddsSurface->GetDC( &hDC ); }
			else
				if( DDFailedCheck(ddReturnVal, "GetDC() failed", cpErrorBuf ) )
					{ MessageBox( NULL, cpErrorBuf, "DrawStringGDI()", MB_ICONEXCLAMATION ); return( 0 ); }
		
			//SetBkColor(hDC,RGB(0,0,0));
			SetBkMode( hDC, TRANSPARENT );
			SetTextColor( hDC, RGB(255,255,255) );
			//sprintf(buffer, "Frames/s = %d", "l");
			TextOut( hDC, x, y, string, (int)strlen( string ) );
		
		ddReturnVal = lpddsSurface->ReleaseDC( hDC );
		if( ddReturnVal != DD_OK )
			if( DDFailedCheck( ddReturnVal, "ReleaseDC( ) failed", cpErrorBuf ) )
			{	MessageBox(NULL, cpErrorBuf, "main()", MB_ICONEXCLAMATION ); return( 0 ); }

		return( 1 );
	}

	//-----------------------------------------------------------------------------
	// Name: ClrS()
	// Desc: blts a block of the given color to the destination surface. clearing it
	//-----------------------------------------------------------------------------
	static inline int ClrS( LPDIRECTDRAWSURFACE7 lpddsSurface, RECT *destinationRec, int r, int g, int b )
	{
		HRESULT ddReturnVal = 0;
		static char cpErrorBuf[100];
		static DDBLTFX ddbltfx;
		//clear the struct
		memset(&ddbltfx,0,sizeof(DDBLTFX));
		//set the size field
		ddbltfx.dwSize = sizeof(DDBLTFX);
		//set the dwfillcolor field to the desired color; this is in 16bit mode
		ddbltfx.dwFillColor = BIULDCOLOR(r,g,b);
		
		//blt to the surface
		ddReturnVal  = lpddsSurface->Blt(destinationRec,
										 NULL, //pointer to source surface
										 NULL, //pointer to the source rectangle
										 DDBLT_COLORFILL | DDBLT_WAIT,&ddbltfx);
		if( ddReturnVal != DD_OK )
			if( ddReturnVal == DDERR_SURFACELOST )
			{ OutputDebugString("Surfacelost ClrS( )\n"); lpddsSurface->Restore( ); return( 1 ); }
			else
				if( DDFailedCheck( ddReturnVal, "Clrs( ), Blt failed", cpErrorBuf ) )
					{ MessageBox(NULL, cpErrorBuf, "ClrS( )", MB_ICONEXCLAMATION ); return( 0 ); }

		return( 1 );
	}

	//-----------------------------------------------------------------------------
	// Name: ClrS()
	// Desc: blts a black block to the destination surface clearing it
	//-----------------------------------------------------------------------------
	static inline int ClrS(LPDIRECTDRAWSURFACE7 lpddsSurface, RECT *destinationRec)
	{
		HRESULT ddReturnVal = 0;
		static char cpErrorBuf[100];
		static DDBLTFX ddbltfx;
	
		//clear the struct
		memset(&ddbltfx,0,sizeof(DDBLTFX));
		//set the size field
		ddbltfx.dwSize = sizeof(DDBLTFX);
		//set the dwfillcolor field to the desired color; this is in 16bit mode
		ddbltfx.dwFillColor = 0;//or can use this BIULDCOLOR(0,0,0)
	
		//blt to the surface
		ddReturnVal  = lpddsSurface->Blt(destinationRec,
										 NULL, //pointer to source surface
										 NULL, //pointer to the source rectangle
										 DDBLT_COLORFILL | DDBLT_WAIT,&ddbltfx);
		if( ddReturnVal != DD_OK )
			if( ddReturnVal == DDERR_SURFACELOST )
			{ OutputDebugString( "Surfacelost ClrS(RECT)\n" ); lpddsSurface->Restore( ); return( 1 );	}
			else
				if( DDFailedCheck(ddReturnVal, "ClrS(RECT), Blt failed", cpErrorBuf ))
					{ MessageBox(NULL, cpErrorBuf, "ClrS)()", MB_ICONEXCLAMATION); return( 0 ); }

		return(1);
	}

	//-----------------------------------------------------------------------------
	// Name: DrawFillRect()
	// Desc: blts a rect to a surface
	//-----------------------------------------------------------------------------
	static inline int DrawFillRect(LPDIRECTDRAWSURFACE7 lpddsSurface, int x, int y, int x2, int y2, int r, int g, int b)
	{
		RECT destRect={x,y,x2,y2};
		HRESULT ddReturnVal = 0;
		static char cpErrorBuf[100];
		static DDBLTFX ddbltfx;
		//clear the struct
		memset(&ddbltfx,0,sizeof(DDBLTFX));
		//set the size field
		ddbltfx.dwSize = sizeof(DDBLTFX);
		//set the dwfillcolor field to the desired color; this is in 16bit mode
		ddbltfx.dwFillColor = BIULDCOLOR(r,g,b);//or can use this BIULDCOLOR(0,0,0)

		//blt to the surface
		ddReturnVal  = lpddsSurface->Blt(&destRect,
										NULL, //pointer to source surface
										NULL, //pointer to the source rectangle
										DDBLT_COLORFILL | DDBLT_WAIT,&ddbltfx);
		if( ddReturnVal != DD_OK )
			if( ddReturnVal==DDERR_SURFACELOST )
			{ OutputDebugString( "Surfacelost DrawFillRect(x,y,...)\n" ); lpddsSurface->Restore( ); return( 1 ); }
			else
				if( DDFailedCheck(ddReturnVal, "DrawFillRect(), Blt failed", cpErrorBuf ) )
					{ MessageBox(NULL, cpErrorBuf, "ClrS( )", MB_ICONEXCLAMATION ); return( 0 ); }
		return ( 0 );
	}

	//-----------------------------------------------------------------------------
	// Name: DrawFillRect()
	// Desc: blts a rect to a surface
	//-----------------------------------------------------------------------------
	static inline int DrawFillRect(LPDIRECTDRAWSURFACE7 lpddsSurface, RECT destinationRect, SCCOLOR col)
	{
		HRESULT ddReturnVal = 0;
		static char cpErrorBuf[100];
		static DDBLTFX ddbltfx;

		//clear the struct
		memset(&ddbltfx,0,sizeof(DDBLTFX));
		//set the size field
		ddbltfx.dwSize = sizeof(DDBLTFX);
		//set the dwfillcolor field to the desired color; this is in 16bit mode
		ddbltfx.dwFillColor = BIULDCOLOR(col.r,col.g,col.b);//or can use this BIULDCOLOR(0,0,0)

		//blt to the surface
		ddReturnVal  = lpddsSurface->Blt(&destinationRect,
										 NULL, //pointer to source surface
										 NULL, //pointer to the source rectangle
										DDBLT_COLORFILL | DDBLT_WAIT,&ddbltfx);
		if( ddReturnVal != DD_OK )
			if( ddReturnVal == DDERR_SURFACELOST )
			{ OutputDebugString( "Surfacelost DrawFilledRect(SCCOLOR), blt failed\n" ); lpddsSurface->Restore( );	return( 1 ); }
			else
				if( DDFailedCheck( ddReturnVal, "DrawFillRect(SCCOLOR), Blt failed", cpErrorBuf ) )
					{	MessageBox( NULL, cpErrorBuf, "ClrS( )", MB_ICONEXCLAMATION ); return( 0 ); }
		return ( 0 );
	}
	
	//-----------------------------------------------------------------------------
	// Name: DrawRect()
	// Desc: Draws a rect
	//-----------------------------------------------------------------------------
	static inline int DrawRect32(DDSURFACEDESC2 ddsd, RECT rec, SCCOLOR col)
	{
		//int nWidth  = rec.right - rec.left,
		//	nHeight = rec.bottom - rec.top;

		DrawLine32(ddsd,rec.left,rec.top,rec.right,rec.top,col); //draw top line
		DrawLine32(ddsd, rec.left, rec.top, rec.left, rec.bottom,col); //draw left line
		DrawLine32(ddsd, rec.left,rec.bottom,rec.right,rec.bottom,col); // draw bottom line
		DrawLine32(ddsd, rec.right,rec.bottom, rec.right, rec.top,col); //draw right line
		
		return 1;
	}

	//-----------------------------------------------------------------------------
	// Name: DrawLine()
	// Desc: draws an unclipped line to a surface
	//-----------------------------------------------------------------------------
	static inline int DrawLine(DDSURFACEDESC2 ddsd, int xo, int yo, int x1,int y1, SCCOLOR col)
	{
		// this function draws a line from xo,yo to x1,y1 using differential error
		// terms (based on Bresenahams work)

		int dx,             // difference in x's
		dy,             // difference in y's
		x_inc,          // amount in pixel space to move during drawing
		y_inc,          // amount in pixel space to move during drawing
		error=0,        // the discriminant i.e. error i.e. decision variable
		index;          // used for looping
		USHORT color = CreateRGB(col.r,col.g,col.b);
		UCHAR *vb_start = (UCHAR*)ddsd.lpSurface;
		int lpitch = ddsd.lPitch;
		// pre-compute first pixel address in video buffer
		USHORT *vb_start16 = (USHORT *)vb_start + xo + yo*(lpitch >> 1);

		// compute horizontal and vertical deltas
		dx = x1-xo;
		dy = y1-yo;

		// test which direction the line is going in i.e. slope angle
		if (dx>=0)
		{
			x_inc = 1;

		} // end if line is moving right
		else
		{
			x_inc = -1;
			dx    = -dx;  // need absolute value

		} // end else moving left

		// test y component of slope

		if (dy>=0)
		{
			y_inc = (lpitch >> 1);
		} // end if line is moving down
		else
		{
			y_inc = -(lpitch >> 1);
			dy    = -dy;  // need absolute value

		} // end else moving up

		// now based on which delta is greater we can draw the line
		if (dx>dy)
		{
			// draw the line
			for (index=0; index<=dx; index++)
			{
				// set the pixel
				*vb_start16 = color;

		        // adjust the error term
			    error+=dy;

				// test if error has overflowed
				if (error>dx)
				{
					error-=dx;

						// move to next line
						vb_start16+=y_inc;


				} // end if error overflowed

				// move to the next pixel
				vb_start16+=x_inc;

			} // end for

		} // end if |slope| <= 1
		else
		{
			// draw the line
			for (index=0; index<=dy; index++)
			{
				// set the pixel
				*vb_start16 = color;

				// adjust the error term
				error+=dx;

				// test if error overflowed
				if (error>0)
				{
					error-=dy;
	
					// move to next line
					vb_start16+=x_inc;

				} // end if error overflowed

				// move to the next pixel
				vb_start16+=y_inc;

			} // end for

		} // end else |slope| > 1

		// return success
		return(1);

	} // end Draw_Line16

	//-----------------------------------------------------------------------------
	// Name: DrawLine()
	// Desc: draws an unclipped line to a surface
	//-----------------------------------------------------------------------------
	static inline int DrawLine32(DDSURFACEDESC2 ddsd, int xo, int yo, int x1,int y1, SCCOLOR col)
	{
		// this function draws a line from xo,yo to x1,y1 using differential error
		// terms (based on Bresenahams work)

		int dx,             // difference in x's
			dy,             // difference in y's
			x_inc,          // amount in pixel space to move during drawing
			y_inc,          // amount in pixel space to move during drawing
			error=0,        // the discriminant i.e. error i.e. decision variable
			index;          // used for looping
		unsigned int color = CreateRGB(col.r,col.g,col.b);
		UCHAR *vb_start = (UCHAR*)ddsd.lpSurface;
		int lpitch = ddsd.lPitch;
		lpitch = lpitch>>2;
		// pre-compute first pixel address in video buffer
		UINT *vb_start16 = (UINT*)vb_start + xo + yo*(lpitch); //>>2

		// compute horizontal and vertical deltas
		dx = x1-xo;
		dy = y1-yo;

		// test which direction the line is going in i.e. slope angle
		if (dx>=0)
		{
			x_inc = 1;

		} // end if line is moving right
		else
		{
			x_inc = -1;
			dx    = -dx;  // need absolute value

		} // end else moving left

		// test y component of slope

		if (dy>=0)
		{
			y_inc = (lpitch);
		} // end if line is moving down
		else
		{
			y_inc = -(lpitch);
			dy    = -dy;  // need absolute value

		} // end else moving up

		// now based on which delta is greater we can draw the line
		if (dx>dy)
		{
			// draw the line
			for (index=0; index<=dx; index++)
			{
				// set the pixel
				*vb_start16 = color;

		        // adjust the error term
			    error+=dy;

				// test if error has overflowed
				if (error>dx)
				{
					error-=dx;

						// move to next line
						vb_start16+=y_inc;


				} // end if error overflowed

				// move to the next pixel
				vb_start16+=x_inc;

			} // end for

		} // end if |slope| <= 1
		else
		{
			// draw the line
			for (index=0; index<=dy; index++)
			{
				// set the pixel
				*vb_start16 = color;

				// adjust the error term
				error+=dx;

				// test if error overflowed
				if (error>0)
				{
					error-=dy;
	
					// move to next line
					vb_start16+=x_inc;

				} // end if error overflowed

				// move to the next pixel
				vb_start16+=y_inc;

			} // end for

		} // end else |slope| > 1

		// return success
		return(1);

	} // end Draw_Line32
};

#endif