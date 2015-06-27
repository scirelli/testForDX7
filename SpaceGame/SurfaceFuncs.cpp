//-----------------------------------------------------------------------------
// File: SurfaceFuncs.cpp
//
// Desc: All surface manipulations, or actions between or to functions
//
//-----------------------------------------------------------------------------


////INCLUDES/////////
#include "General.h"
#include "CreateRGB.h"

////GLOBALS/////////
extern HWND                 main_window_handle;  // save the window handle
extern HDC                  hDC;  // creates a handle to a device contexts
extern BOOL                 bActiveApp; // tells whether app is active or not
extern LPDIRECTDRAW7        lpddObj;  // dd object
extern LPDIRECTDRAWSURFACE7 lpddsPrimary;  // dd primary surface
extern LPDIRECTDRAWSURFACE7 lpddsSecondary;  // dd back surface
extern LPDIRECTDRAWPALETTE  lpddpal;  // a pointer to the created dd palette
extern DDSURFACEDESC2       ddsd;                   // a direct draw surface description struct
extern DDSCAPS2             ddscaps;                // a direct draw surface capabilities struct
extern DDBLTFX              ddbltfx;                // The Blit Fx structure used for all blting
extern HRESULT              ddReturnVal;     // result back from dd calls
extern bool                 iKeyStatus[256];        // keeps track of key status
extern char                 cpErrorBuf[1024];       // Build up error messages
extern bool                 bDebugMode;


////FUNCTIONS///////

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
// Name: CreateSurface()
// Desc: creates a offscreen plain surface
//-----------------------------------------------------------------------------
LPDIRECTDRAWSURFACE7 CreateSurface(int width, int height, SCCOLOR TransparentColor)
{// this function creates an offscreen plain surface

	//DDSURFACEDESC2 ddsd;         // working description
	LPDIRECTDRAWSURFACE7 lpdds;  // temporary surface
    
	// set to access caps, width, and height
	memset(&ddsd,0,sizeof(ddsd));
	ddsd.dwSize  = sizeof(ddsd);
	ddsd.dwFlags = DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT;

	// set dimensions of the new bitmap surface
	ddsd.dwWidth  =  width;
	ddsd.dwHeight =  height;

	// set surface to offscreen plain
	ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN;//default is video memory VRAM
	
	// create the surface
	ddReturnVal  = lpddObj->CreateSurface(&ddsd,&lpdds,NULL);
	if(ddReturnVal == DDERR_OUTOFVIDEOMEMORY)//out of vram
	{
		// set surface to offscreen plain system memory
		ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_SYSTEMMEMORY;
		ddReturnVal  = lpddObj->CreateSurface(&ddsd,&lpdds,NULL);
	}
	if (DDFailedCheck(ddReturnVal, "CreateSurface() failed", cpErrorBuf ))//DDERR_OUTOFVIDEOMEMORY DDSCAPS_SYSTEMMEMORY
	{	MessageBox(main_window_handle, cpErrorBuf, "CreateSurface()", MB_ICONEXCLAMATION);   return(NULL); }
	
	// set color key to TransparentColor
	DDCOLORKEY color_key; // used to set color key
	color_key.dwColorSpaceLowValue  = BIULDCOLOR(TransparentColor.r,TransparentColor.g,TransparentColor.b);
	color_key.dwColorSpaceHighValue = BIULDCOLOR(TransparentColor.r,TransparentColor.g,TransparentColor.b);

	// now set the color key for source blitting
	ddReturnVal  = lpdds->SetColorKey(DDCKEY_SRCBLT, &color_key);
	if (DDFailedCheck(ddReturnVal, "SetColorKey() failed", cpErrorBuf ))
	{	MessageBox(main_window_handle, cpErrorBuf, "CreateSurface()", MB_ICONEXCLAMATION);   return(NULL); }

	// return surface
	return(lpdds);

} // end CreateSurface()

//-----------------------------------------------------------------------------
// Name: CopySurface()
// Desc: this function copies one surface to another
//-----------------------------------------------------------------------------   
int CopySurface(LPDIRECTDRAWSURFACE7 lpddsDestSurface, RECT *destRect,LPDIRECTDRAWSURFACE7 lpddsSorSurface, RECT *sorRect)
{// this function flips a surface onto the primary surface
	
	//Blt the frame
	ddReturnVal = lpddsDestSurface->Blt(destRect/*dest rect*/,lpddsSorSurface, //pointer to source surface
									  sorRect, //pointer to the source rectangle
									  DDBLT_WAIT | DDBLT_KEYSRC, NULL/*ddbltfx struct*/);//NULL means entire surface
	if(ddReturnVal!=DD_OK)
		if(ddReturnVal==DDERR_SURFACELOST)
		{	OutputDebugString("Surfacelost CopySurface()\n"); lpddsSorSurface->Restore(); lpddsDestSurface->Restore(); return(1);	}
		else
			if (DDFailedCheck(ddReturnVal, "CopySurface(), Blt failed", cpErrorBuf ))
			{	MessageBox(NULL, cpErrorBuf, "SurfaceFuncs", MB_ICONEXCLAMATION);   return(0); }
		
	return 1;
}

//-----------------------------------------------------------------------------
// Name: Flip()
// Desc: this function flips a surface onto the primary surface at a certain 
//       piont
//-----------------------------------------------------------------------------   
int Flip(LPDIRECTDRAWSURFACE7 lpddsSurface, int x, int y, int x2, int y2)
{// this function flips a surface onto the primary surface
	static RECT destrect;
	destrect.left = x; destrect.top = y;
	destrect.right = x2; destrect.bottom = y2;

	//Blt the frame
	ddReturnVal = lpddsPrimary->Blt(&destrect/*dest rect*/,lpddsSurface, //pointer to source surface
									  NULL, //pointer to the source rectangle
									  DDBLT_WAIT | DDBLT_KEYSRC, NULL/*ddbltfx struct*/);//NULL means entire surface
	if(ddReturnVal!=DD_OK)
		if(ddReturnVal==DDERR_SURFACELOST)
		{	OutputDebugString("Surfacelost Flip(x,y,...)\n"); lpddsSurface->Restore(); lpddsPrimary->Restore(); return(1);	}
		else
			if (DDFailedCheck(ddReturnVal, "Flip(lpddsSurface,x,y,...), Blt failed", cpErrorBuf ))
			{	MessageBox(NULL, cpErrorBuf, "SurfaceFuncs", MB_ICONEXCLAMATION);   return(0); }
		
	return 1;
}

//-----------------------------------------------------------------------------
// Name: Flip()
// Desc: this function flips a surface onto the primary surface
//-----------------------------------------------------------------------------   
int Flip(LPDIRECTDRAWSURFACE7 lpddsSurface, RECT destrect)
{// this function flips a surface onto the primary surface
	
	//Blt the frame
	ddReturnVal = lpddsPrimary->Blt(&destrect/*dest rect*/,lpddsSurface, //pointer to source surface
									  NULL, //pointer to the source rectangle
									  DDBLT_WAIT | DDBLT_KEYSRC, NULL/*ddbltfx struct*/);//NULL means entire surface
	if(ddReturnVal!=DD_OK)
		if(ddReturnVal==DDERR_SURFACELOST)
		{	OutputDebugString("Surfacelost Flip(RECT)\n"); lpddsSurface->Restore(); lpddsPrimary->Restore(); return(1);	}
		else
			if (DDFailedCheck(ddReturnVal, "Flip(lpddsSurface, RECT), Blt failed", cpErrorBuf ))
			{	MessageBox(NULL, cpErrorBuf, "SurfaceFuncs", MB_ICONEXCLAMATION);   return(0); }
		
	return 1;
}

//-----------------------------------------------------------------------------
// Name: Flip()
// Desc: this function flips a surface onto the primary surface 
//       primary surface expected to be a normal window (non-fullscreen)
//       This blts to entire window.
//-----------------------------------------------------------------------------   
int FlipToWindow(LPDIRECTDRAWSURFACE7 lpddsSurface)
{// this function flips a surface onto the primary surface
	static RECT temp;

	GetWindowRect(main_window_handle, &temp);
	
	//Blt the frame
	ddReturnVal = lpddsPrimary->Blt(&temp/*dest rect*/,lpddsSurface, //pointer to source surface
									  NULL, //pointer to the source rectangle
									  DDBLT_WAIT | DDBLT_KEYSRC, NULL/*ddbltfx struct*/);//NULL means entire surface
	if(ddReturnVal!=DD_OK)
		if(ddReturnVal==DDERR_SURFACELOST)
		{	OutputDebugString("Surfacelost FlipToWindow()\n"); lpddsSurface->Restore(); lpddsPrimary->Restore(); return(1);	}
		else
			if (DDFailedCheck(ddReturnVal, "FlipToWindow(lpddsSurface), Blt failed", cpErrorBuf ))
			{	MessageBox(NULL, cpErrorBuf, "SurfaceFuncs", MB_ICONEXCLAMATION);   return(0); }
		
	return 1;
}

//-----------------------------------------------------------------------------
// Name: Flip()
// Desc: this function flips a surface onto the primary surface 
//       primary surface expected to be a normal window (non-fullscreen).
//       flips to client area only
//-----------------------------------------------------------------------------   
int FlipToClient(LPDIRECTDRAWSURFACE7 lpddsSurface)
{// this function flips a surface onto the primary surface
	static RECT temp;

	GetWindowRect(main_window_handle, &temp);
	temp.top +=20;
	temp.left +=4;
	temp.right -=4;
	temp.bottom -=4;
	//Blt the frame
	ddReturnVal = lpddsPrimary->Blt(&temp/*dest rect*/,lpddsSurface, //pointer to source surface
									  NULL, //pointer to the source rectangle
									  DDBLT_WAIT | DDBLT_KEYSRC, NULL/*ddbltfx struct*/);//NULL means entire surface
	if(ddReturnVal!=DD_OK)
		if(ddReturnVal==DDERR_SURFACELOST)
		{	OutputDebugString("Surfacelost FlipToClient()\n"); lpddsSurface->Restore(); lpddsPrimary->Restore(); return(1);	}
		else
			if (DDFailedCheck(ddReturnVal, "FlipToClient(lpddsSurface), Blt failed", cpErrorBuf ))
			{	MessageBox(NULL, cpErrorBuf, "SurfaceFuncs", MB_ICONEXCLAMATION);   return(0); }
		
	return 1;
}

//-----------------------------------------------------------------------------
// Name: Flip()
// Desc: this function flip the primary surface with the secondary surface
//-----------------------------------------------------------------------------   
int Flip()
{// this function flip the primary surface with the secondary surface

	// flip pages
	while( (ddReturnVal=lpddsPrimary->Flip(NULL, DDFLIP_WAIT) )!= DD_OK)
	{
		if(ddReturnVal!=DD_OK)
			if(ddReturnVal==DDERR_SURFACELOST)
			{	OutputDebugString("Surfacelost Flip()\n"); lpddsPrimary->Restore(); return(1);	}
			else
				if (DDFailedCheck(ddReturnVal, "Flip()", cpErrorBuf ))
				{	MessageBox(NULL, cpErrorBuf, "SurfaceFuncs", MB_ICONEXCLAMATION);   return(0); }
	}

	// return success
	return(1);

} // end DD_Flip

//-----------------------------------------------------------------------------
// Name: Lock()
// Desc: Fucntion to lock the entire surface
//-----------------------------------------------------------------------------
int Lock(LPDIRECTDRAWSURFACE7 lpddsSurface)
{
	memset(&ddsd,0,sizeof(ddsd));
	ddsd.dwSize = sizeof(ddsd);
	
	ddReturnVal  = lpddsSurface->Lock(NULL,&ddsd,DDLOCK_SURFACEMEMORYPTR | DDLOCK_WAIT, NULL);
	if (DDFailedCheck(ddReturnVal, "Lock(), failed", cpErrorBuf ))
	{	MessageBox(main_window_handle, cpErrorBuf, "SurfaceFuncs, Lock()", MB_ICONEXCLAMATION); return(0); }
	
	return (1);
}
//-----------------------------------------------------------------------------
// Name: UnLock()
// Desc: UnLocks entire surface that has been locked
//-----------------------------------------------------------------------------
int UnLock(LPDIRECTDRAWSURFACE7 lpddsSurface)
{
	ddReturnVal  = lpddsSurface->Unlock(NULL);
	if (DDFailedCheck(ddReturnVal, "Unlock(), failed", cpErrorBuf ))
	{	MessageBox(main_window_handle, cpErrorBuf, "UnLock()", MB_ICONEXCLAMATION); return(0); }
	
	return(1);
}