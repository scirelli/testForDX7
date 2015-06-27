//-----------------------------------------------------------------------------
// File: Sprite.cpp
//
// Desc: Implemtation of sprite class
//
//-----------------------------------------------------------------------------

////INCLUDES///////////////
#include "Sprite.h"
#include "CDDGameApp.h"

////GLOBALS////////////////

//-----------------------------------------------------------------------------
// Name: CSPRITE()
// Desc: constructor
//----------------------------------------------------------------------------- 
CSPRITE::CSPRITE()
{
	m_iNumberofFrames = 0;
	m_Counter = 0;
	m_iRow = 0;
	m_iCol = 0;
	m_nWidth=0; 
	m_nHeight=0;
	m_Rect.top = m_Rect.left = 0;
	m_Rect.bottom = 0;
	m_Rect.right = 0;
	m_iCurrentFrame = 0;
	m_iCurrentAnimSequence = 0;
	m_iTimeBetweenFrames = 0;
	m_iAnimationIndex = 0;
	m_iX=0; m_iY=0;              
	physProp.VelVector.x = 0;
	physProp.VelVector.y = 0;
	m_iState= SPRITE_STATE_DEAD;                
	m_iAttr= SPRITE_ATRR_NOTLOADED;
	for(int i=0; i< MAX_SPRITE_ANIMATIONS; i++)
	{
		m_ipAnimations[i]=NULL;
		m_ipAnimAttr[i] = 0;
		m_ipAnimSeqMax[i] = 0;
	}
	for(i=0; i< MAX_SPRITE_FRAMES; i++)
		m_lpddsImage[i]=NULL;
}
//-----------------------------------------------------------------------------
// Name: CSPRITE(int width, int height, int iNumofFrames, int rows, int columns)
// Desc: constructor
//----------------------------------------------------------------------------- 
CSPRITE::CSPRITE(int width, int height, int iNumofFrames, int NumofColumns,int NumofRows)
{
	m_iNumberofFrames = iNumofFrames;
	m_Counter = 0;
	m_iRow = NumofRows;
	m_iCol = NumofColumns;
	m_nWidth = width; 
	m_nHeight = height;
	m_Rect.top = m_Rect.left = 0;
	m_Rect.bottom = height;
	m_Rect.right = width;
	m_iCurrentFrame = 0;
	m_iCurrentAnimSequence = 0;
	m_iTimeBetweenFrames = 0;
	m_iAnimationIndex = 0;
	m_iX=0; m_iY=0;              
	physProp.VelVector.x = 0;
	physProp.VelVector.y = 0;
	m_iState= SPRITE_STATE_DEAD;                
	m_iAttr= SPRITE_ATRR_NOTLOADED;
	for(int i=0; i< MAX_SPRITE_ANIMATIONS; i++)
	{
		m_ipAnimations[i]=NULL;
		m_ipAnimAttr[i] = 0;
		m_ipAnimSeqMax[i] = 0;
	}
	for(i=0; i< MAX_SPRITE_FRAMES; i++)
		m_lpddsImage[i]=NULL;
}
//-----------------------------------------------------------------------------
// Name: CSPRITE(int width, int height, int iNumofFrames, int rows, int columns,
//				 int TimeBetweenFrames)
// Desc: constructor
//----------------------------------------------------------------------------- 
CSPRITE::CSPRITE(int width, int height, int iNumofFrames, int NumofColumns,int NumofRows, int TimeBetweenFrames)
{
	m_iNumberofFrames = iNumofFrames;
	m_Counter = 0;
	m_iRow = NumofRows;
	m_iCol = NumofColumns;
	m_nWidth=width; 
	m_nHeight=height;
	m_Rect.top = m_Rect.left = 0;
	m_Rect.bottom = height;
	m_Rect.right = width;
	m_iTimeBetweenFrames = TimeBetweenFrames;
	m_iCurrentFrame = 0;
	m_iCurrentAnimSequence = 0;
	m_iAnimationIndex = 0;
	m_iX=0; m_iY=0;              
	physProp.VelVector.x = 0;
	physProp.VelVector.y = 0;
	m_iState= SPRITE_STATE_DEAD;                
	m_iAttr= SPRITE_ATRR_NOTLOADED;
	
	for(int i=0; i< MAX_SPRITE_ANIMATIONS; i++)
	{
		m_ipAnimations[i]=NULL;
		m_ipAnimAttr[i] = 0;
		m_ipAnimSeqMax[i] = 0;
	}
	for(i=0; i< MAX_SPRITE_FRAMES; i++)
		m_lpddsImage[i]=NULL;
}

//-----------------------------------------------------------------------------
// Name: CSPRITE(int width, int height, int iNumofFrames, int NumofColumns,int NumofRows, 
//        int x, int y, float vx, float vy, int TimeBetweenFrames, int state);
// Desc: constructor
//----------------------------------------------------------------------------- 
CSPRITE::CSPRITE(int width, int height, int iNumofFrames, int NumofColumns,int NumofRows, int x, int y, float vx0, float vy0, 
				 int TimeBetweenFrames, int state)
{
	m_iNumberofFrames = iNumofFrames;
	m_Counter = 0;
	m_iTimeBetweenFrames = TimeBetweenFrames;
	m_nWidth = width;
	m_nHeight = height; 
	m_iRow = NumofRows;
	m_iCol = NumofColumns;      
	m_iX = x;
	m_iY = y;          
	physProp.initVel.x=vx0; physProp.initVel.y=vy0;
	physProp.VelVector.x = vx0;
	physProp.VelVector.y = vy0;
	m_iState = state;

	m_iCurrentFrame = 0;
	m_iCurrentAnimSequence = 0;
	m_iAnimationIndex = 0;
	m_iAttr= SPRITE_ATRR_NOTLOADED;
	m_Rect.top = m_Rect.left = 0;
	m_Rect.bottom = height-1;
	m_Rect.right = width-1;
	for(int i=0; i< MAX_SPRITE_ANIMATIONS; i++)
	{
		m_ipAnimations[i]=NULL;
		m_ipAnimAttr[i] = 0;
		m_ipAnimSeqMax[i] = 0;
	}
	for(i=0; i< MAX_SPRITE_FRAMES; i++)
		m_lpddsImage[i]=NULL;
}

//-----------------------------------------------------------------------------
// Name: InitSprite(int width, int height, int iNumofFrames, int NumofColumns,int NumofRows, 
//        int x, int y, float vx, float vy, int TimeBetweenFrames, int state);
// Desc: Inits a sprite
//-----------------------------------------------------------------------------
void CSPRITE::InitSprite(int width, int height, int iNumofFrames, int NumofColumns,int NumofRows, int x, int y, float vx0, float vy0, 
						 int TimeBetweenFrames, int state)
{
	m_iNumberofFrames = iNumofFrames;
	m_Counter = 0;
	m_iTimeBetweenFrames = TimeBetweenFrames;
	m_nWidth = width;
	m_nHeight = height; 
	m_iRow = NumofRows;
	m_iCol = NumofColumns;      
	m_iX = x;
	m_iY = y;          
	physProp.initVel.x=vx0; physProp.initVel.y=vy0;
	physProp.VelVector.x = vx0;
	physProp.VelVector.y = vy0;
	m_iState = state;

	m_iCurrentFrame = 0;
	m_iCurrentAnimSequence = 0;
	m_iAnimationIndex = 0;
	m_iAttr= SPRITE_ATRR_NOTLOADED;
	m_Rect.top = m_Rect.left = 0;
	m_Rect.bottom = height;
	m_Rect.right = width;
	for(int i=0; i< MAX_SPRITE_ANIMATIONS; i++)
	{
		m_ipAnimations[i]=NULL;
		m_ipAnimAttr[i] = 0;
		m_ipAnimSeqMax[i] = 0;
	}
	for(i=0; i< MAX_SPRITE_FRAMES; i++)
		m_lpddsImage[i]=NULL;
}

//-----------------------------------------------------------------------------
// Name: InitSprite(int width, int height, int iNumofFrames, int NumofColumns,int NumofRows)
// Desc: Inits a sprite
//----------------------------------------------------------------------------- 
void CSPRITE::InitSprite(int width, int height, int iNumofFrames, int NumofColumns,int NumofRows)
{
	m_iNumberofFrames = iNumofFrames;
	m_Counter = 0;
	m_iRow = NumofRows;
	m_iCol = NumofColumns;
	m_nWidth = width; 
	m_nHeight = height;
	m_Rect.top = m_Rect.left = 0;
	m_Rect.bottom = height;
	m_Rect.right = width;
	m_iCurrentFrame = 0;
	m_iCurrentAnimSequence = 0;
	m_iTimeBetweenFrames = 0;
	m_iAnimationIndex = 0;
	m_iX=0; m_iY=0;              
	physProp.initVel.x=0; physProp.initVel.y=0;
	physProp.VelVector.x = 0;
	physProp.VelVector.y = 0;
	m_iState= SPRITE_STATE_DEAD;                
	m_iAttr= SPRITE_ATRR_NOTLOADED;
	for(int i=0; i< MAX_SPRITE_ANIMATIONS; i++)
	{
		m_ipAnimations[i]=NULL;
		m_ipAnimAttr[i] = 0;
		m_ipAnimSeqMax[i] = 0;
	}
	for(i=0; i< MAX_SPRITE_FRAMES; i++)
		m_lpddsImage[i]=NULL;
}

//-----------------------------------------------------------------------------
// Name: InitSprite(int width, int height, int iNumofFrames, int NumofColumns,int NumofRows, int TimeBetweenFrames)
// Desc: Inits a sprite
//----------------------------------------------------------------------------- 
void CSPRITE::InitSprite(int width, int height, int iNumofFrames, int NumofColumns,int NumofRows, int TimeBetweenFrames)
{
	m_iNumberofFrames = iNumofFrames;
	m_Counter = 0;
	m_iRow = NumofRows;
	m_iCol = NumofColumns;
	m_nWidth=width; 
	m_nHeight=height;
	m_Rect.top = m_Rect.left = 0;
	m_Rect.bottom = height;
	m_Rect.right = width;
	m_iTimeBetweenFrames = TimeBetweenFrames;
	m_iCurrentFrame = 0;
	m_iCurrentAnimSequence = 0;
	m_iAnimationIndex = 0;
	m_iX=0; m_iY=0;              
	physProp.initVel.x=0; physProp.initVel.y=0;
	physProp.VelVector.x = 0;
	physProp.VelVector.y = 0;
	m_iState= SPRITE_STATE_DEAD;                
	m_iAttr= SPRITE_ATRR_NOTLOADED;
	
	for(int i=0; i< MAX_SPRITE_ANIMATIONS; i++)
	{
		m_ipAnimations[i]=NULL;
		m_ipAnimAttr[i] = 0;
		m_ipAnimSeqMax[i] = 0;
	}
	for(i=0; i< MAX_SPRITE_FRAMES; i++)
		m_lpddsImage[i]=NULL;
}

//-----------------------------------------------------------------------------
// Name: ~CSPRITE()
// Desc: destructor
//----------------------------------------------------------------------------- 
CSPRITE::~CSPRITE()
{
	for(int i=0; i<m_iNumberofFrames; i++)
	{
		if(m_lpddsImage[i])
			m_lpddsImage[i]->Release();
	}
}

//-----------------------------------------------------------------------------
// Name: LoadSpriteImage(CBITMAP framesofAnim)
// Desc: creates surfaces for all the cells and copies image to surface cx and 
//       cy are absolute quards i.e. (0,0) (1,0)...
//----------------------------------------------------------------------------- 
int CSPRITE::LoadBMImage( LPDIRECTDRAWSURFACE7 &lpddsSurface,
                                           LPCTSTR strBMP,                                            
                                           DWORD dwDesiredWidth, 
                                           DWORD dwDesiredHeight )
{
    HRESULT        hr;
    HBITMAP        hBMP = NULL;
    BITMAP         bmp;
	DDSURFACEDESC2 ddsd;
	LPDIRECTDRAW7 lpddObj = GetGameApp()->getDirectDObj();

    if( strBMP == NULL ) 
        return E_INVALIDARG;

    //  Try to load the bitmap as a resource, if that fails, try it as a file
    hBMP = (HBITMAP) LoadImage( GetModuleHandle(NULL), strBMP, 
                                IMAGE_BITMAP, NULL, NULL, 
                                LR_CREATEDIBSECTION );
    if( hBMP == NULL )
    {
        hBMP = (HBITMAP) LoadImage( NULL, strBMP, 
                                    IMAGE_BITMAP, NULL, NULL, 
                                    LR_LOADFROMFILE | LR_CREATEDIBSECTION );
        if( hBMP == NULL )
            return E_FAIL;
    }

    // Get size of the bitmap
    GetObject( hBMP, sizeof(bmp), &bmp );
	
	// Create a DirectDrawSurface for this bitmap
    ZeroMemory( &ddsd, sizeof(ddsd) );
    ddsd.dwSize         = sizeof(ddsd);
    ddsd.dwFlags        = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH;
    ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN;
    ddsd.dwWidth        = bmp.bmWidth;
    ddsd.dwHeight       = bmp.bmHeight;
	
	if( FAILED( hr = lpddObj->CreateSurface(&ddsd,&lpddsSurface,NULL) ) )
    {
        DeleteObject( hBMP );
        return hr;
    }

	// Draw the bitmap on this surface
	if( FAILED( hr = CSPRITE::DrawBitmap(lpddsSurface, hBMP, 0, 0, 0, 0 ) ) )
    {
        DeleteObject( hBMP );
        return hr;
    }

    DeleteObject( hBMP );

    return S_OK;
}

//----------------------------------------------------------------------
// Name: DrawBitmap()
// Desc: Draws a bitmap on a surface
//----------------------------------------------------------------------
HRESULT CSPRITE::DrawBitmap( LPDIRECTDRAWSURFACE7 &lpddsSurface, HBITMAP hBMP, DWORD dwBMPOriginX, DWORD dwBMPOriginY, DWORD dwBMPWidth, DWORD dwBMPHeight )
{
    HDC            hDCImage;
    HDC            hDC;
    BITMAP         bmp;
    DDSURFACEDESC2 ddsd;
    HRESULT        hr;

    if( hBMP == NULL || lpddsSurface == NULL )
        return E_INVALIDARG;

    // Make sure this surface is restored.
    if( FAILED( hr = lpddsSurface->Restore() ) )
        return hr;

    // Get the surface.description
    ddsd.dwSize  = sizeof(ddsd);
    lpddsSurface->GetSurfaceDesc( &ddsd );

    if( ddsd.ddpfPixelFormat.dwFlags == DDPF_FOURCC )
        return E_NOTIMPL;

    // Select bitmap into a memoryDC so we can use it.
    hDCImage = CreateCompatibleDC( NULL );
    if( NULL == hDCImage )
        return E_FAIL;

    SelectObject( hDCImage, hBMP );

    // Get size of the bitmap
    if( !GetObject( hBMP, sizeof(bmp), &bmp ) )
		return 0;

    // Use the passed size, unless zero
    dwBMPWidth  = ( dwBMPWidth  == 0 ) ? bmp.bmWidth  : dwBMPWidth;     
    dwBMPHeight = ( dwBMPHeight == 0 ) ? bmp.bmHeight : dwBMPHeight;

    // Stretch the bitmap to cover this surface
    if( FAILED( hr = lpddsSurface->GetDC( &hDC ) ) )
        return hr;

    if( !StretchBlt( hDC, 0, 0, ddsd.dwWidth, ddsd.dwHeight, hDCImage, dwBMPOriginX, dwBMPOriginY, dwBMPWidth, dwBMPHeight, SRCCOPY ) )
		return 0;

    if( FAILED( hr = lpddsSurface->ReleaseDC( hDC ) ) )
        return hr;

    DeleteDC( hDCImage );

    return S_OK;
}

//-----------------------------------------------------------------------------
// Name: LoadSpriteImage(CBITMAP framesofAnim)
// Desc: creates surfaces for all the cells and copies image to surface cx and 
//       cy are absolute quards i.e. (0,0) (1,0)...
//----------------------------------------------------------------------------- 
int CSPRITE::LoadSpriteImage(LPCTSTR filename, int dwDesiredWidth, int dwDesiredHeight)
{
	int row, col, frame=0;
	HRESULT hr=0;
	DDSURFACEDESC2 ddsd;
	LPDIRECTDRAWSURFACE7 lpddsTemp = NULL;
	SCCOLOR TransColor( (int)m_Settings.GetSettings()->TransColorR, (int)m_Settings.GetSettings()->TransColorG, (int)m_Settings.GetSettings()->TransColorB);
	RECT sorcRect={0,0,m_nWidth-1,m_nHeight-1};
	
	hr = LoadBMImage(lpddsTemp,filename, dwDesiredWidth, dwDesiredHeight);
	if( FAILED(hr) )
		return hr;
	
	//GetGameApp()->getPrimarySurface()->Blt(NULL,lpddsTemp,NULL,DDBLT_WAIT,NULL);//debug
	//Sleep(10000);

	for( row=0; row< m_iRow; row++ )
	{
		for( col=0; col<m_iCol; col++ )
		{
			m_lpddsImage[frame] = m_Utils.CreateSurface(m_nWidth, m_nHeight,TransColor);
			hr = m_lpddsImage[frame]->Blt(NULL/*dest rect*/,lpddsTemp, //pointer to source surface
										  &sorcRect, //pointer to the source rectangle
										  DDBLT_WAIT, NULL/*ddbltfx struct*/);
			if( FAILED(hr) )
				return hr;
			frame++;
			sorcRect.left += m_nWidth;
			sorcRect.right += m_nWidth;
		}
		sorcRect.top += m_nHeight;
		sorcRect.bottom += m_nHeight;
		sorcRect.left = 0;
		sorcRect.right = m_nWidth;
	}//End for(int row=0; row< m_iRow; row++)
	frame=0;
	m_iAttr = SPRITE_ATTR_LOADED;
	
	if(lpddsTemp)
		lpddsTemp->Release();
	
	return (S_OK);
}

//-----------------------------------------------------------------------------
// Name: LoadSpriteImage(CBITMAP framesofAnim)
// Desc: creates surfaces for all the cells and copies image to surface cx and 
//       cy are absolute quards i.e. (0,0) (1,0)...
//----------------------------------------------------------------------------- 
int CSPRITE::LoadSpriteImage16(char * filename)
{
	USHORT *source_ptr,   // working pointers
		   *dest_ptr;
	int index_y=0, column=0,row=0,cx=0,cy=0;
	CBITMAP bitmap(filename);
	DDSURFACEDESC2 ddsd;
	SCCOLOR TransColor( (int)m_Settings.GetSettings()->TransColorR, (int)m_Settings.GetSettings()->TransColorG, (int)m_Settings.GetSettings()->TransColorB);

	bitmap.LoadBitmapFile();
			
	for(int frame=0; frame< m_iNumberofFrames; frame++)
	{

		m_lpddsImage[frame] = m_Utils.CreateSurface(m_nWidth, m_nHeight,TransColor);

		//clear out the ddsd
		memset(&ddsd,0,sizeof(ddsd));
		ddsd.dwSize  = sizeof(ddsd);
	
		if( !CUtilities::Lock(m_lpddsImage[frame], ddsd ) )
			return(0);
			dest_ptr = (USHORT*) ddsd.lpSurface;
		
			cx=column; cy=row;
			//compute x,y
			cx = cx*(m_nWidth+1) + 1;
			cy = cy*(m_nHeight+1) + 1;
		
			// extract bitmap data
			source_ptr = (USHORT*)bitmap.GetImageData() + (cy*bitmap.GetWidth()+cx);

			// iterate thru each scanline and copy bitmap
			for (int index_y=0; index_y < m_nHeight; index_y++)
			{
			    // copy next line of data to destination
				memcpy(dest_ptr, source_ptr,m_nWidth*2);

				// advance pointers
				dest_ptr   += (ddsd.lPitch >> 1);
				source_ptr += bitmap.GetWidth();
			} // end for index_y

			if(!CUtilities::UnLock(m_lpddsImage[frame]))
			return(0);
		column++;
		if(column > m_iCol-1)
		{
			column=0; row++;
		}
	}//End for(int i=0; i<= m_iNumberofFrames; i++)
	
	m_iAttr = SPRITE_ATTR_LOADED;
	return (1);
}

//-----------------------------------------------------------------------------
// Name: LoadSpriteImage24()
// Desc: creates surfaces for all the cells and copies image to surface cx and 
//       cy are absolute quards i.e. (0,0) (1,0)...
//----------------------------------------------------------------------------- 
int CSPRITE::LoadSpriteImage24(char * filename)
{
	DWORD *source_ptr,   // working pointers
		  *dest_ptr;
	int index_y=0, column=0,row=0,cx=0,cy=0;
	CBITMAP bitmap(filename);
	DDSURFACEDESC2 ddsd;
	SCCOLOR TransColor( (int)m_Settings.GetSettings()->TransColorR, (int)m_Settings.GetSettings()->TransColorG, (int)m_Settings.GetSettings()->TransColorB);

	bitmap.LoadBitmapFile24();
			
	for(int frame=0; frame< m_iNumberofFrames; frame++)
	{

		m_lpddsImage[frame] = m_Utils.CreateSurface(m_nWidth, m_nHeight,TransColor);

		//clear out the ddsd
		memset(&ddsd,0,sizeof(ddsd));
		ddsd.dwSize  = sizeof(ddsd);
	
		if(!CUtilities::Lock(m_lpddsImage[frame], ddsd))
			return(0);
			dest_ptr = (DWORD*) ddsd.lpSurface;
		
			cx=column; cy=row;
			//compute x,y
			cx = cx*(m_nWidth+1) + 1;
			cy = cy*(m_nHeight+1) + 1;
		
			// extract bitmap data
			source_ptr = (DWORD*)bitmap.GetImageData() + (cy*bitmap.GetWidth()+cx);
			
			// iterate thru each scanline and copy bitmap
			for (int index_y=0; index_y < m_nHeight; index_y++)
			{
			    // copy next line of data to destination
				memcpy(dest_ptr, source_ptr,m_nWidth*4);//4 for four bytes
				
				// advance pointers
				dest_ptr   += ddsd.lPitch>>2; //still not sure why divide by 4
				source_ptr += bitmap.GetWidth();
			} // end for index_y

			if(!CUtilities::UnLock(m_lpddsImage[frame]))
			return(0);
		column++;
		if(column > m_iCol-1)
		{
			column=0; row++;
		}
	}//End for(int i=0; i<= m_iNumberofFrames; i++)
	
	m_iAttr = SPRITE_ATTR_LOADED;
	return (1);
}

//-----------------------------------------------------------------------------
// Name: UnLoadSpriteImage()
// Desc: Clears a sprite object for reloading
//-----------------------------------------------------------------------------
int CSPRITE::UnLoadSpriteImage()
{
	for(int i=0; i<m_iNumberofFrames; i++)
	{
		if(m_lpddsImage[i])
			m_lpddsImage[i]->Release();
	}

	m_iNumberofFrames = 0;
	m_iRow = 0;
	m_iCol = 0;
	m_nWidth=0; 
	m_nHeight=0;
	m_Rect.top = m_Rect.left = 0;
	m_Rect.bottom = m_Rect.right = 0;
	m_iCurrentFrame = 0;
	m_iCurrentAnimSequence = 0;
	m_iTimeBetweenFrames = 0;
	m_iAnimationIndex = 0;
	m_iX=0; m_iY=0;              
	physProp.initVel.x=0; physProp.initVel.y=0;
	physProp.VelVector.x = 0;
	physProp.VelVector.y = 0;
	m_iState= SPRITE_STATE_DEAD;                
	m_iAttr= SPRITE_ATRR_NOTLOADED;
	for(int i=0; i< MAX_SPRITE_ANIMATIONS; i++)
	{
		m_ipAnimations[i]=NULL;
		m_ipAnimAttr[i] = 0;
		m_ipAnimSeqMax[i] = 0;
	}
	for(i=0; i< MAX_SPRITE_FRAMES; i++)
		m_lpddsImage[i]=NULL;
	
	return (1);
}
//-----------------------------------------------------------------------------
// Name: RestoreSurface()
// Desc: Restores all sprite surfaces.
//-----------------------------------------------------------------------------
int CSPRITE::RestoreSurface()
{
	char cpErrorBuf[1000];
	HRESULT ddReturnVal = 0;

	for(int i=0; i<m_iNumberofFrames; i++)
	{
		ddReturnVal = m_lpddsImage[i]->Restore();
		if (DDFailedCheck(ddReturnVal, "CSPRITE::RestoreSurface() failed", cpErrorBuf ))
		{	MessageBox(m_Utils.getWindowHandle(), cpErrorBuf, "CSPRITE", MB_ICONEXCLAMATION);   return(0); }
	}
	
	return(1);
}

//-----------------------------------------------------------------------------
// Name: ()
// Desc: 
//-----------------------------------------------------------------------------
int  CSPRITE::Equals(CSPRITE* sprite)
{
	m_iNumberofFrames = sprite->GetNumberofFrames();
	m_iRow = sprite->GetRows();
	m_iCol = sprite->GetColumns();
	m_nWidth = sprite->GetWidth(); 
	m_nHeight = sprite->GetHeight();
	m_Rect = sprite->GetRect();
	
	m_iTimeBetweenFrames = sprite->GetTimeBetweenFrames();
	m_iCurrentFrame = sprite->GetCurrentFrameNum();
	m_iCurrentAnimSequence = sprite->GetCurrentAnimNum();
	m_iAnimationIndex = sprite->GetAnimationIndex();
	m_iX=sprite->GetXPos(); 
	m_iY=sprite->GetYPos();              
	physProp.initVel.x = sprite->physProp.initVel.x; 
	physProp.initVel.y = sprite->physProp.initVel.y;
	m_iState= sprite->GetState();                
	m_iAttr= sprite->GetAttr();
	
	for(int i=0; i< MAX_SPRITE_ANIMATIONS; i++)
	{
		m_ipAnimations[i]= sprite->GetAnimation(i);
		m_ipAnimAttr[i] = sprite->GetAnimAttr(i);
		m_ipAnimSeqMax[i] = sprite->GetAnimSeqMax(i);
	}

	for(int i=0; i< MAX_SPRITE_FRAMES; i++)
		m_lpddsImage[i]=sprite->GetImages()[i];

	m_iAttr = SPRITE_ATTR_LOADED;

	return(1);
}

//-----------------------------------------------------------------------------
// Name: IncrementCounter()
// Desc: increments the arbitrary counter
//-----------------------------------------------------------------------------
//void CSPRITE::IncrementCounter()
//{
//	m_Counter++;
//}

//-----------------------------------------------------------------------------
// Name: GetCount()
// Desc: gets the count
//-----------------------------------------------------------------------------
//int CSPRITE::GetCount()
//{
//	return m_Counter;
//}
//-----------------------------------------------------------------------------
// Name: GetImages()
// Desc: returns a pointer to the images array
//-----------------------------------------------------------------------------
LPDIRECTDRAWSURFACE7* CSPRITE::GetImages()
{
	return m_lpddsImage;
}

//-----------------------------------------------------------------------------
// Name: GetWidth()
// Desc: Returns the width of the sprite includes the border
//-----------------------------------------------------------------------------
int CSPRITE::GetWidth()
{
	return m_nWidth;
}

//-----------------------------------------------------------------------------
// Name: GetHeight()
// Desc: Returns the height of the sprite includes the border
//-----------------------------------------------------------------------------
int CSPRITE::GetHeight()
{
	return m_nHeight;
}

//-----------------------------------------------------------------------------
// Name: GetRect()
// Desc: Returns the bounding rect of the sprite
//-----------------------------------------------------------------------------
RECT CSPRITE::GetRect()
{
	return m_Rect;
}

//-----------------------------------------------------------------------------
// Name: GetFrame()
// Desc: returns a pointer to a dd surface
//-----------------------------------------------------------------------------
LPDIRECTDRAWSURFACE7 CSPRITE::GetFrame(int frameNumber)
{
	return m_lpddsImage[frameNumber];
}

//-----------------------------------------------------------------------------
// Name: GetNumberofFrames()
// Desc: returns the total number of frames in this sprite
//-----------------------------------------------------------------------------
int CSPRITE::GetNumberofFrames()
{
	return m_iNumberofFrames;
}

//-----------------------------------------------------------------------------
// Name: GetCurrentFrameNum()
// Desc: returns the number of the current frame being displayed
//-----------------------------------------------------------------------------
int CSPRITE::GetCurrentFrameNum()
{
	return m_iCurrentFrame;
}

//-----------------------------------------------------------------------------
// Name: GetCurrentAnimNum()
// Desc: gets the index number of the animation sequence curently being displayed
//-----------------------------------------------------------------------------
int CSPRITE::GetCurrentAnimNum()
{
	return m_iCurrentAnimSequence;
}

//-----------------------------------------------------------------------------
// Name: GetAnimation(int)
// Desc: returns an animation sequence
//-----------------------------------------------------------------------------
int * CSPRITE::GetAnimation(int index)
{
	return m_ipAnimations[index];
}

//-----------------------------------------------------------------------------
// Name: GetAnimationIndex()
// Desc: returns the number of the last animation entered the index keeps track
//       of how many animations are in m_ipAnimations[]
//-----------------------------------------------------------------------------
int CSPRITE::GetAnimationIndex()
{
	return m_iAnimationIndex;
}

//-----------------------------------------------------------------------------
// Name: GetTimeBetweenFrames()
// Desc: returns the time between each frame
//-----------------------------------------------------------------------------
int CSPRITE::GetTimeBetweenFrames()
{
	return m_iTimeBetweenFrames;
}

//-----------------------------------------------------------------------------
// Name: ()
// Desc: 
//-----------------------------------------------------------------------------
int CSPRITE::GetXPos()
{
	return m_iX;
}

//-----------------------------------------------------------------------------
// Name: ()
// Desc: 
//-----------------------------------------------------------------------------
int CSPRITE::GetYPos()
{
	return m_iY;
}

//-----------------------------------------------------------------------------
// Name: GetRows()
// Desc: returns the total number of rows in the bitmap image of the sprite
//-----------------------------------------------------------------------------
int CSPRITE::GetRows()
{
	return m_iRow;
}

//-----------------------------------------------------------------------------
// Name: GetColumns()
// Desc: same as GetRows() but with colums
//-----------------------------------------------------------------------------
int CSPRITE::GetColumns()
{
	return m_iCol;
}

//-----------------------------------------------------------------------------
// Name: GetVX0()
// Desc: 
//-----------------------------------------------------------------------------
//float CSPRITE::GetVX0()
//{
//	return m_fVX0;
//}

//-----------------------------------------------------------------------------
// Name: GetVY0()
// Desc: 
//-----------------------------------------------------------------------------
//float CSPRITE::GetVY0()
//{
//	return m_fVY0;
//}

//-----------------------------------------------------------------------------
// Name: GetVelVector()
// Desc: 
//-----------------------------------------------------------------------------
//CVECTOR CSPRITE::GetVelVector()
//{
//	return m_VelVector;
//}

//-----------------------------------------------------------------------------
// Name: GetState()
// Desc: returns the state of a sprite alive, dead, etc.
//-----------------------------------------------------------------------------
int CSPRITE::GetState()
{
	return m_iState;
}

//-----------------------------------------------------------------------------
// Name: GetAttr()
// Desc: 
//-----------------------------------------------------------------------------
int CSPRITE::GetAttr()
{
	return m_iAttr;
}

//-----------------------------------------------------------------------------
// Name: GetAnimSeqMax()
// Desc: returns max seq number of a animation
//-----------------------------------------------------------------------------
int CSPRITE::GetAnimSeqMax(int index)
{
	return m_ipAnimSeqMax[index];
}

//-----------------------------------------------------------------------------
// Name: GetAnimAttr()
// Desc: returns the attr of an animation at index
//-----------------------------------------------------------------------------
int CSPRITE::GetAnimAttr(int index)
{
	return m_ipAnimAttr[index];
}

//-----------------------------------------------------------------------------
// Name: SetCount()
// Desc: sets the counter to count
//-----------------------------------------------------------------------------
//void CSPRITE::SetCount(int count)
//{
//	m_Counter = count;
//}
//-----------------------------------------------------------------------------
// Name: ()
// Desc: 
//-----------------------------------------------------------------------------
void CSPRITE::SetWidth(int width)
{
	m_nWidth=width;
}

//-----------------------------------------------------------------------------
// Name: ()
// Desc: 
//-----------------------------------------------------------------------------
void CSPRITE::SetHeight(int height)
{
	m_nHeight=height;
}

//-----------------------------------------------------------------------------
// Name: SetRect()
// Desc: Sets the bounding RECT of the sprite
//-----------------------------------------------------------------------------
void CSPRITE::SetRect(RECT rect)
{
	m_Rect = rect;
}
void CSPRITE::SetRect(int top, int left, int bottom,int right)
{
	m_Rect.top = top;
	m_Rect.left = left;
	m_Rect.bottom = bottom;
	m_Rect.right = right;
}

//-----------------------------------------------------------------------------
// Name: SetNumberofFrames()
// Desc: sets the total number of frames in a sprite
//-----------------------------------------------------------------------------
void CSPRITE::SetNumberofFrames(int frames)
{
	m_iNumberofFrames = frames;
}

//-----------------------------------------------------------------------------
// Name: SetCurrentFrameNum()
// Desc: Sets the current frame to be displayed
//-----------------------------------------------------------------------------
int CSPRITE::SetCurrentFrameNum(int currFrame)
{
	if(currFrame < MAX_SPRITE_FRAMES)
	{
		m_iCurrentFrame = currFrame;
		return 1;
	}
	return 0;
}

//-----------------------------------------------------------------------------
// Name: SetCurrentAnim()
// Desc: set the animation sequence to be displayed
//-----------------------------------------------------------------------------
int CSPRITE::SetCurrentAnim(int currAnim)
{
	if(currAnim < MAX_SPRITE_ANIMATIONS)
	{
		m_iCurrentAnimSequence = currAnim;
		return 1;
	}
	return 0;
}

//-----------------------------------------------------------------------------
// Name: SetAnimations()
// Desc: stores an animation sequence
//-----------------------------------------------------------------------------
void CSPRITE::SetAnimations(int *anim, int Seqmax, int attribute)
{
	m_ipAnimations[m_iAnimationIndex]= anim;
	m_ipAnimSeqMax[m_iAnimationIndex] = Seqmax-1;
	m_ipAnimAttr[m_iAnimationIndex] = attribute;
	m_iAnimationIndex++;
	if(m_iAnimationIndex > MAX_SPRITE_ANIMATIONS)
		m_iAnimationIndex = MAX_SPRITE_ANIMATIONS;
}

//-----------------------------------------------------------------------------
// Name: SetAnimations()
// Desc: over writes/stores an animation sequence AnimsIndex is the spot
//       where you want the animation seq to be stored.
//-----------------------------------------------------------------------------
void CSPRITE::SetAnimations(int *anim, int SeqMax, int attribute, int AnimsIndex)
{
	if(AnimsIndex > MAX_SPRITE_ANIMATIONS)
		AnimsIndex = MAX_SPRITE_ANIMATIONS;

	m_ipAnimations[AnimsIndex]= anim;
	m_ipAnimSeqMax[AnimsIndex] = SeqMax-1;
	m_ipAnimAttr[AnimsIndex] = attribute;
}

//-----------------------------------------------------------------------------
// Name: SetAnimAttr()
// Desc: sets the attribute of an animation sequence cont or once
//-----------------------------------------------------------------------------
void CSPRITE::SetAnimAttr(int index, int attr)
{
	m_ipAnimAttr[index] = attr;
}

//-----------------------------------------------------------------------------
// Name: SetAnimationIndex()
// Desc: moves the index pointer of m_iAnimation[]. for the next animation
//       to be placed for SetAnimations
//-----------------------------------------------------------------------------
void CSPRITE::SetAnimationIndex(int animIndex)
{
	m_iAnimationIndex = animIndex;
}

//-----------------------------------------------------------------------------
// Name: SetTimeBetweenFrames()
// Desc: 
//-----------------------------------------------------------------------------
void CSPRITE::SetTimeBetweenFrames(int interval)
{
	m_iTimeBetweenFrames = interval;
}

//-----------------------------------------------------------------------------
// Name: SetXPos()
// Desc: 
//-----------------------------------------------------------------------------
void CSPRITE::SetXPos(int x)
{
	m_iX = x+(m_nWidth/2);
}

//-----------------------------------------------------------------------------
// Name: SetYPos()
// Desc: 
//-----------------------------------------------------------------------------
void CSPRITE::SetYPos(int y)
{
	m_iY = y+(m_nHeight/2);
}

//-----------------------------------------------------------------------------
// Name: SetRows()
// Desc: 
//-----------------------------------------------------------------------------
void CSPRITE::SetRows(int m)
{
	m_iRow = m;
}

//-----------------------------------------------------------------------------
// Name: SetColumns()
// Desc: 
//-----------------------------------------------------------------------------
void CSPRITE::SetColumns(int n)
{
	m_iCol = n;
}

//-----------------------------------------------------------------------------
// Name: SetVX0()
// Desc: 
//-----------------------------------------------------------------------------
//void CSPRITE::SetVX0(float vx0)
//{
//	m_fVX0 = vx0;
//}

//-----------------------------------------------------------------------------
// Name: SetVY0()
// Desc: 
//-----------------------------------------------------------------------------
//void CSPRITE::SetVY0(float vy0)
//{
//	m_fVY0 = vy0;
//}

//-----------------------------------------------------------------------------
// Name: SetVelVector()
// Desc: sets the velocity vectors = to velVec
//-----------------------------------------------------------------------------
//void CSPRITE::SetVelVector(CVECTOR velVec)
//{
//	m_VelVector = velVec;
//}

//-----------------------------------------------------------------------------
// Name: SetVelVector()
// Desc: sets the xi and yj of the velocity vector
//-----------------------------------------------------------------------------
//void CSPRITE::SetVelVector(float x, float y)
//{
//	m_VelVector.x = x;
//	m_VelVector.y = y;
//}

//-----------------------------------------------------------------------------
// Name: SetState()
// Desc: 
//-----------------------------------------------------------------------------
void CSPRITE::SetState(int state)
{
	m_iState = state;
}

//-----------------------------------------------------------------------------
// Name: SetAttr()
// Desc: 
//-----------------------------------------------------------------------------
void CSPRITE::SetAttr(int attr)
{
	m_iAttr = attr;
}

//-----------------------------------------------------------------------------
// Name: DrawSprite()
// Desc: Draws a list of sprites to a surface at position x,y
//-----------------------------------------------------------------------------
int CSPRITE::DrawSprite(LPDIRECTDRAWSURFACE7 lpddsSurface, int nX, int nY)
{
	char cpErrorBuf[1000];
	static RECT destRect;
	HRESULT ddReturnVal = 0;
	//static int j=0;
	//static DWORD start=GetTickCount();
	//static CTIMER timer;
	
	if( (m_iState != SPRITE_STATE_DEAD) && ( m_iAttr == SPRITE_ATTR_LOADED) )
	{
		//Fill in dest rect
		destRect.top = nY;
		destRect.left = nX;
		destRect.bottom = nY + m_nHeight;
		destRect.right = nX + m_nWidth;
	
		//Setup the time between frames drawn
		if( m_Timer.Elapsed(m_iTimeBetweenFrames) && (m_ipAnimAttr[m_iCurrentAnimSequence] != SPRITE_ANIM_DONE) ) 
		{
			m_Counter++; m_Timer.StartTimer();//m_StartTime=GetTickCount();

			//Check to see if reached last frame in sequence
			if( m_Counter > m_ipAnimSeqMax[m_iCurrentAnimSequence])
				if(m_ipAnimAttr[m_iCurrentAnimSequence] == SPRITE_ANIM_CONT) //check to see if sprite is supposed to be cont animated
					m_Counter=0;
				else  //animate it once
					if( m_ipAnimAttr[m_iCurrentAnimSequence] == SPRITE_ANIM_ONCE )
					{
						m_Counter = m_ipAnimSeqMax[m_iCurrentAnimSequence];
						m_ipAnimAttr[m_iCurrentAnimSequence] = SPRITE_ANIM_DONE;
					}
		}
	
		//Get the next frame from the anim sequence
		m_iCurrentFrame = m_ipAnimations[m_iCurrentAnimSequence][m_Counter];
	
		//Blt the frame
		ddReturnVal = lpddsSurface->Blt(&destRect/*dest rect*/,m_lpddsImage[m_iCurrentFrame], //pointer to source surface
										  &m_Rect, //pointer to the source rectangle
										  DDBLT_WAIT | DDBLT_KEYSRC, NULL/*ddbltfx struct*/);
		if(ddReturnVal != DD_OK)
			if(ddReturnVal==DDERR_SURFACELOST)
			{	this->RestoreSurface(); GetGameApp()->getPrimarySurface()->Restore(); OutputDebugString("Surfacelost DrawSprite(x,y)\n"); return(1);	}
			else
				if (DDFailedCheck(ddReturnVal, "DrawSprite(nX,nY), Blt failed", cpErrorBuf ))
					{	MessageBox(m_Utils.getWindowHandle(), cpErrorBuf, "DrawSprite()", MB_ICONEXCLAMATION);   return(0); }
	}//End if( (m_iState != SPRITE_STATE_DEAD) && ( m_iAttr == SPRITE_ATTR_LOADED) )
	
	return(1);
}

//-----------------------------------------------------------------------------
// Name: DrawSprite()
// Desc: Draws a list of sprites to a surface using internal (x,y)
//-----------------------------------------------------------------------------
int CSPRITE::DrawSprite(LPDIRECTDRAWSURFACE7 lpddsSurface)
{
	char cpErrorBuf[1000];
	static RECT destRect;
	HRESULT ddReturnVal = 0;
	//static int j=0;
	//static DWORD start=GetTickCount();
	//static CTIMER timer;
	
	if( (m_iState != SPRITE_STATE_DEAD) && ( m_iAttr == SPRITE_ATTR_LOADED) )
	{
		//Fill in dest rect
		destRect.top = m_iY;
		destRect.left = m_iX;
		destRect.bottom = m_iY + m_nHeight;
		destRect.right = m_iX + m_nWidth;
	
		//Setup the time between frames drawn
		if( m_Timer.Elapsed(m_iTimeBetweenFrames) && (m_ipAnimAttr[m_iCurrentAnimSequence] != SPRITE_ANIM_DONE) ) 
		{
			m_Counter++; m_Timer.StartTimer();//m_StartTime=GetTickCount();

			//Check to see if reached last frame in sequence
			if( m_Counter > m_ipAnimSeqMax[m_iCurrentAnimSequence])
				if(m_ipAnimAttr[m_iCurrentAnimSequence] == SPRITE_ANIM_CONT) //check to see if sprite is supposed to be cont animated
					m_Counter=0;
				else  //animate it once
					if( m_ipAnimAttr[m_iCurrentAnimSequence] == SPRITE_ANIM_ONCE )
					{
						m_Counter = m_ipAnimSeqMax[m_iCurrentAnimSequence];
						m_ipAnimAttr[m_iCurrentAnimSequence] = SPRITE_ANIM_DONE;
					}
		}
	
		//Get the next frame from the anim sequence
		m_iCurrentFrame = m_ipAnimations[m_iCurrentAnimSequence][m_Counter];
	
		//Blt the frame
		ddReturnVal = lpddsSurface->Blt(&destRect/*dest rect*/,m_lpddsImage[m_iCurrentFrame], //pointer to source surface
										  &m_Rect, //pointer to the source rectangle
										  DDBLT_WAIT | DDBLT_KEYSRC, NULL/*ddbltfx struct*/);
		if(ddReturnVal != DD_OK)
			if(ddReturnVal==DDERR_SURFACELOST)
			{	this->RestoreSurface(); GetGameApp()->getPrimarySurface()->Restore();OutputDebugString("Surfacelost DrawSprite()\n"); return(1);	}
			else
				if (DDFailedCheck(ddReturnVal, "DrawSprite(), Blt failed", cpErrorBuf ))
					{	MessageBox(m_Utils.getWindowHandle(), cpErrorBuf, "DrawSprite()", MB_ICONEXCLAMATION);   return(0); }
	}//End if( (m_iState != SPRITE_STATE_DEAD) && ( m_iAttr == SPRITE_ATTR_LOADED) )
	
	return(1);
}

//-----------------------------------------------------------------------------
// Name: DrawSprite()
// Desc: Draws a list of sprites to a surface at position destRect
//-----------------------------------------------------------------------------
int CSPRITE::DrawSprite(LPDIRECTDRAWSURFACE7 lpddsSurface, RECT &destRect)
{
	char cpErrorBuf[1000];
	HRESULT ddReturnVal = 0;
	//static int j=0;
	//static DWORD start=GetTickCount();
	//static CTIMER timer;
	
	if( (m_iState != SPRITE_STATE_DEAD) && ( m_iAttr == SPRITE_ATTR_LOADED) )
	{	
		//Setup the time between frames drawn
		if( m_Timer.Elapsed(m_iTimeBetweenFrames) && (m_ipAnimAttr[m_iCurrentAnimSequence] != SPRITE_ANIM_DONE) ) 
		{
			m_Counter++; m_Timer.StartTimer();//m_StartTime=GetTickCount();

			//Check to see if reached last frame in sequence
			if( m_Counter > m_ipAnimSeqMax[m_iCurrentAnimSequence])
				if(m_ipAnimAttr[m_iCurrentAnimSequence] == SPRITE_ANIM_CONT) //check to see if sprite is supposed to be cont animated
					m_Counter=0;
				else  //animate it once
					if( m_ipAnimAttr[m_iCurrentAnimSequence] == SPRITE_ANIM_ONCE )
					{
						m_Counter = m_ipAnimSeqMax[m_iCurrentAnimSequence];
						m_ipAnimAttr[m_iCurrentAnimSequence] = SPRITE_ANIM_DONE;
					}
		}
	
		//Get the next frame from the anim sequence
		m_iCurrentFrame = m_ipAnimations[m_iCurrentAnimSequence][m_Counter];
	
		//Blt the frame
		ddReturnVal = lpddsSurface->Blt(&destRect/*dest rect*/,m_lpddsImage[m_iCurrentFrame], //pointer to source surface
										  &m_Rect, //pointer to the source rectangle
										  DDBLT_WAIT | DDBLT_KEYSRC, NULL/*ddbltfx struct*/);
		if(ddReturnVal != DD_OK)
			if(ddReturnVal==DDERR_SURFACELOST)
			{	this->RestoreSurface(); GetGameApp()->getPrimarySurface()->Restore(); OutputDebugString("Surfacelost DrawSprite(x,y)\n"); return(1);	}
			else
				if (DDFailedCheck(ddReturnVal, "DrawSprite(nX,nY), Blt failed", cpErrorBuf ))
					{	MessageBox(m_Utils.getWindowHandle(), cpErrorBuf, "DrawSprite()", MB_ICONEXCLAMATION);   return(0); }
	}//End if( (m_iState != SPRITE_STATE_DEAD) && ( m_iAttr == SPRITE_ATTR_LOADED) )
	
	return(1);
}

//=========================================================FUNCTIONS===========================================================================


