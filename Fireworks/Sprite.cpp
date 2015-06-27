//-----------------------------------------------------------------------------
// File: Sprite.cpp
//
// Desc: Implemtation of sprite class
//
//-----------------------------------------------------------------------------

////INCLUDES///////////////
#include "Sprite.h"
#include "General.h"
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
	color_key.dwColorSpaceLowValue  = _RGB16BIT565(TransparentColor.r,TransparentColor.g,TransparentColor.b);
	color_key.dwColorSpaceHighValue = _RGB16BIT565(TransparentColor.r,TransparentColor.g,TransparentColor.b);

	// now set the color key for source blitting
	ddReturnVal  = lpdds->SetColorKey(DDCKEY_SRCBLT, &color_key);
	if (DDFailedCheck(ddReturnVal, "SetColorKey() failed", cpErrorBuf ))
	{	MessageBox(main_window_handle, cpErrorBuf, "CreateSurface()", MB_ICONEXCLAMATION);   return(NULL); }

	// return surface
	return(lpdds);

} // end CreateSurface()

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
	m_iWidth=0; 
	m_iHeight=0;
	m_Rect.top = m_Rect.left = 0;
	m_Rect.bottom = 0;
	m_Rect.right = 0;
	m_iCurrentFrame = 0;
	m_iCurrentAnimSequence = 0;
	m_iTimeBetweenFrames = 0;
	m_iAnimationIndex = 0;
	m_iX=0; m_iY=0;              
	m_fVX=0; m_fVY=0;
	m_iState= SPRITE_STATE_DEAD;                
	m_iAttr= SPRITE_ATRR_NOTLOADED;
	for(int i=0; i<= MAX_SPRITE_ANIMATIONS; i++)
	{
		m_ipAnimations[i]=NULL;
		m_ipAnimAttr[i] = 0;
		m_ipAnimSeqMax[i] = 0;
	}
	for(i=0; i<= MAX_SPRITE_FRAMES; i++)
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
	m_iWidth = width-1; 
	m_iHeight = height-1;
	m_Rect.top = m_Rect.left = 0;
	m_Rect.bottom = height-1;
	m_Rect.right = width-1;
	m_iCurrentFrame = 0;
	m_iCurrentAnimSequence = 0;
	m_iTimeBetweenFrames = 0;
	m_iAnimationIndex = 0;
	m_iX=0; m_iY=0;              
	m_fVX=0; m_fVY=0;
	m_iState= SPRITE_STATE_DEAD;                
	m_iAttr= SPRITE_ATRR_NOTLOADED;
	for(int i=0; i<= MAX_SPRITE_ANIMATIONS; i++)
	{
		m_ipAnimations[i]=NULL;
		m_ipAnimAttr[i] = 0;
		m_ipAnimSeqMax[i] = 0;
	}
	for(i=0; i<= MAX_SPRITE_FRAMES; i++)
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
	m_iWidth=width-1; 
	m_iHeight=height-1;
	m_Rect.top = m_Rect.left = 0;
	m_Rect.bottom = height-1;
	m_Rect.right = width-1;
	m_iTimeBetweenFrames = TimeBetweenFrames;
	m_iCurrentFrame = 0;
	m_iCurrentAnimSequence = 0;
	m_iAnimationIndex = 0;
	m_iX=0; m_iY=0;              
	m_fVX=0; m_fVY=0;
	m_iState= SPRITE_STATE_DEAD;                
	m_iAttr= SPRITE_ATRR_NOTLOADED;
	
	for(int i=0; i<= MAX_SPRITE_ANIMATIONS; i++)
	{
		m_ipAnimations[i]=NULL;
		m_ipAnimAttr[i] = 0;
		m_ipAnimSeqMax[i] = 0;
	}
	for(i=0; i<= MAX_SPRITE_FRAMES; i++)
		m_lpddsImage[i]=NULL;
}

//-----------------------------------------------------------------------------
// Name: CSPRITE(int width, int height, int iNumofFrames, int NumofColumns,int NumofRows, 
//        int x, int y, float vx, float vy, int TimeBetweenFrames, int state);
// Desc: constructor
//----------------------------------------------------------------------------- 
CSPRITE::CSPRITE(int width, int height, int iNumofFrames, int NumofColumns,int NumofRows, int x, int y, float vx, float vy, int TimeBetweenFrames,
				 int state)
{
	m_iNumberofFrames = iNumofFrames;
	m_Counter = 0;
	m_iTimeBetweenFrames = TimeBetweenFrames;
	m_iWidth = width-1;
	m_iHeight = height-1; 
	m_iRow = NumofRows;
	m_iCol = NumofColumns;      
	m_iX = x;
	m_iY = y;          
	m_fVX = vx;
	m_fVY = vy;        
	m_iState = state;

	m_iCurrentFrame = 0;
	m_iCurrentAnimSequence = 0;
	m_iAnimationIndex = 0;
	m_iAttr= SPRITE_ATRR_NOTLOADED;
	m_Rect.top = m_Rect.left = 0;
	m_Rect.bottom = height-1;
	m_Rect.right = width-1;
	for(int i=0; i<= MAX_SPRITE_ANIMATIONS; i++)
	{
		m_ipAnimations[i]=NULL;
		m_ipAnimAttr[i] = 0;
		m_ipAnimSeqMax[i] = 0;
	}
	for(i=0; i<= MAX_SPRITE_FRAMES; i++)
		m_lpddsImage[i]=NULL;
}

//-----------------------------------------------------------------------------
// Name: InitSprite(int width, int height, int iNumofFrames, int NumofColumns,int NumofRows, 
//        int x, int y, float vx, float vy, int TimeBetweenFrames, int state);
// Desc: Inits a sprite
//-----------------------------------------------------------------------------
void CSPRITE::InitSprite(int width, int height, int iNumofFrames, int NumofColumns,int NumofRows, int x, int y, float vx, float vy, int TimeBetweenFrames,
						 int state)
{
	m_iNumberofFrames = iNumofFrames;
	m_Counter = 0;
	m_iTimeBetweenFrames = TimeBetweenFrames;
	m_iWidth = width-1;
	m_iHeight = height-1; 
	m_iRow = NumofRows;
	m_iCol = NumofColumns;      
	m_iX = x;
	m_iY = y;          
	m_fVX = vx;
	m_fVY = vy;        
	m_iState = state;

	m_iCurrentFrame = 0;
	m_iCurrentAnimSequence = 0;
	m_iAnimationIndex = 0;
	m_iAttr= SPRITE_ATRR_NOTLOADED;
	m_Rect.top = m_Rect.left = 0;
	m_Rect.bottom = height-1;
	m_Rect.right = width-1;
	for(int i=0; i<= MAX_SPRITE_ANIMATIONS; i++)
	{
		m_ipAnimations[i]=NULL;
		m_ipAnimAttr[i] = 0;
		m_ipAnimSeqMax[i] = 0;
	}
	for(i=0; i<= MAX_SPRITE_FRAMES; i++)
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
	m_iWidth = width-1; 
	m_iHeight = height-1;
	m_Rect.top = m_Rect.left = 0;
	m_Rect.bottom = height-1;
	m_Rect.right = width-1;
	m_iCurrentFrame = 0;
	m_iCurrentAnimSequence = 0;
	m_iTimeBetweenFrames = 0;
	m_iAnimationIndex = 0;
	m_iX=0; m_iY=0;              
	m_fVX=0; m_fVY=0;
	m_iState= SPRITE_STATE_DEAD;                
	m_iAttr= SPRITE_ATRR_NOTLOADED;
	for(int i=0; i<= MAX_SPRITE_ANIMATIONS; i++)
	{
		m_ipAnimations[i]=NULL;
		m_ipAnimAttr[i] = 0;
		m_ipAnimSeqMax[i] = 0;
	}
	for(i=0; i<= MAX_SPRITE_FRAMES; i++)
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
	m_iWidth=width-1; 
	m_iHeight=height-1;
	m_Rect.top = m_Rect.left = 0;
	m_Rect.bottom = height-1;
	m_Rect.right = width-1;
	m_iTimeBetweenFrames = TimeBetweenFrames;
	m_iCurrentFrame = 0;
	m_iCurrentAnimSequence = 0;
	m_iAnimationIndex = 0;
	m_iX=0; m_iY=0;              
	m_fVX=0; m_fVY=0;
	m_iState= SPRITE_STATE_DEAD;                
	m_iAttr= SPRITE_ATRR_NOTLOADED;
	
	for(int i=0; i<= MAX_SPRITE_ANIMATIONS; i++)
	{
		m_ipAnimations[i]=NULL;
		m_ipAnimAttr[i] = 0;
		m_ipAnimSeqMax[i] = 0;
	}
	for(i=0; i<= MAX_SPRITE_FRAMES; i++)
		m_lpddsImage[i]=NULL;
}

//-----------------------------------------------------------------------------
// Name: ~CSPRITE()
// Desc: destructor
//----------------------------------------------------------------------------- 
CSPRITE::~CSPRITE()
{
	for(int i=0; i<=m_iNumberofFrames-1; i++)
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
int CSPRITE::LoadSpriteImage(char * filename)
{
	USHORT *source_ptr,   // working pointers
		   *dest_ptr;
	int index_y=0, column=0,row=0,cx=0,cy=0;
	CBITMAP bitmap(filename);
	
	bitmap.LoadBitmapFile();
	
	for(int frame=0; frame<= m_iNumberofFrames-1; frame++)
	{

		m_lpddsImage[frame] = CreateSurface(m_iWidth, m_iHeight,TransColor);

		//clear out the ddsd
		memset(&ddsd,0,sizeof(ddsd));
		ddsd.dwSize  = sizeof(ddsd);
	
		if(!Lock(m_lpddsImage[frame]))
			return(0);
			dest_ptr = (USHORT*) ddsd.lpSurface;
		
			cx=column; cy=row;
			//compute x,y
			cx = cx*(m_iWidth+1) + 1;
			cy = cy*(m_iHeight+1) + 1;
		
			// extract bitmap data
			source_ptr = (USHORT*)bitmap.GetImageData() + (cy*bitmap.GetWidth()+cx);

			// iterate thru each scanline and copy bitmap
			for (int index_y=0; index_y < m_iHeight; index_y++)
			{
			    // copy next line of data to destination
				memcpy(dest_ptr, source_ptr,m_iWidth*2);

				// advance pointers
				dest_ptr   += (ddsd.lPitch >> 1);
				source_ptr += bitmap.GetWidth();
			} // end for index_y

		if(!UnLock(m_lpddsImage[frame]))
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
	for(int i=0; i<=m_iNumberofFrames-1; i++)
	{
		if(m_lpddsImage[i])
			m_lpddsImage[i]->Release();
	}

	m_iNumberofFrames = 0;
	m_iRow = 0;
	m_iCol = 0;
	m_iWidth=0; 
	m_iHeight=0;
	m_Rect.top = m_Rect.left = 0;
	m_Rect.bottom = m_Rect.right = 0;
	m_iCurrentFrame = 0;
	m_iCurrentAnimSequence = 0;
	m_iTimeBetweenFrames = 0;
	m_iAnimationIndex = 0;
	m_iX=0; m_iY=0;              
	m_fVX=0; m_fVY=0;
	m_iState= SPRITE_STATE_DEAD;                
	m_iAttr= SPRITE_ATRR_NOTLOADED;
	for(int i=0; i<= MAX_SPRITE_ANIMATIONS; i++)
	{
		m_ipAnimations[i]=NULL;
		m_ipAnimAttr[i] = 0;
		m_ipAnimSeqMax[i] = 0;
	}
	for(i=0; i<= MAX_SPRITE_FRAMES; i++)
		m_lpddsImage[i]=NULL;
	
	return (1);
}
//-----------------------------------------------------------------------------
// Name: RestoreSurface()
// Desc: Restores all sprite surfaces.
//-----------------------------------------------------------------------------
int CSPRITE::RestoreSurface()
{
	for(int i=0; i<=m_iNumberofFrames-1; i++)
	{
		ddReturnVal = m_lpddsImage[i]->Restore();
		if (DDFailedCheck(ddReturnVal, "RestoreSurface() failed", cpErrorBuf ))
			{	MessageBox(main_window_handle, cpErrorBuf, "CSPRITE", MB_ICONEXCLAMATION);   return(0); }
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
	m_iWidth = sprite->GetWidth(); 
	m_iHeight = sprite->GetHeight();
	m_Rect = sprite->GetRect();
	
	m_iTimeBetweenFrames = sprite->GetTimeBetweenFrames();
	m_iCurrentFrame = sprite->GetCurrentFrameNum();
	m_iCurrentAnimSequence = sprite->GetCurrentAnimNum();
	m_iAnimationIndex = sprite->GetAnimationIndex();
	m_iX=sprite->GetXPos(); 
	m_iY=sprite->GetYPos();              
	m_fVX=sprite->GetVX(); 
	m_fVY=sprite->GetVY();
	m_iState= sprite->GetState();                
	m_iAttr= sprite->GetAttr();
	
	for(int i=0; i<= MAX_SPRITE_ANIMATIONS; i++)
	{
		m_ipAnimations[i]= sprite->GetAnimation(i);
		m_ipAnimAttr[i] = sprite->GetAnimAttr(i);
		m_ipAnimSeqMax[i] = sprite->GetAnimSeqMax(i);
	}

	for(int i=0; i<= MAX_SPRITE_FRAMES; i++)
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
	return m_iWidth;
}

//-----------------------------------------------------------------------------
// Name: GetHeight()
// Desc: Returns the height of the sprite includes the border
//-----------------------------------------------------------------------------
int CSPRITE::GetHeight()
{
	return m_iHeight;
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
// Name: ()
// Desc: 
//-----------------------------------------------------------------------------
float CSPRITE::GetVX()
{
	return m_fVX;
}

//-----------------------------------------------------------------------------
// Name: ()
// Desc: 
//-----------------------------------------------------------------------------
float CSPRITE::GetVY()
{
	return m_fVY;
}

//-----------------------------------------------------------------------------
// Name: ()
// Desc: 
//-----------------------------------------------------------------------------
int CSPRITE::GetState()
{
	return m_iState;
}

//-----------------------------------------------------------------------------
// Name: ()
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
	m_iWidth=width;
}

//-----------------------------------------------------------------------------
// Name: ()
// Desc: 
//-----------------------------------------------------------------------------
void CSPRITE::SetHeight(int height)
{
	m_iHeight=height;
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
	if(currFrame <= MAX_SPRITE_FRAMES)
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
	if(currAnim <= MAX_SPRITE_ANIMATIONS)
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
void CSPRITE::SetAnimations(int * anim, int Seqmax, int attribute)
{
	m_ipAnimations[m_iAnimationIndex]= anim;
	m_ipAnimSeqMax[m_iAnimationIndex] = Seqmax;
	m_ipAnimAttr[m_iAnimationIndex] = attribute;
	m_iAnimationIndex++;
	if(m_iAnimationIndex > MAX_SPRITE_ANIMATIONS)
		m_iAnimationIndex = MAX_SPRITE_ANIMATIONS;
}

void CSPRITE::SetAnimations(int * anim, int SeqMax, int attribute, int index)
{
	if(index > MAX_SPRITE_ANIMATIONS)
		index = MAX_SPRITE_ANIMATIONS;

	m_ipAnimations[index]= anim;
	m_ipAnimSeqMax[index] = SeqMax;
	m_ipAnimAttr[index] = attribute;
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
// Desc: moves the index pointer of m_iAnimation[]
//-----------------------------------------------------------------------------
void CSPRITE::SetAnimationIndex(int animIndex)
{
	m_iAnimationIndex = animIndex;
}

//-----------------------------------------------------------------------------
// Name: ()
// Desc: 
//-----------------------------------------------------------------------------
void CSPRITE::SetTimeBetweenFrames(int interval)
{
	m_iTimeBetweenFrames = interval;
}

//-----------------------------------------------------------------------------
// Name: ()
// Desc: 
//-----------------------------------------------------------------------------
void CSPRITE::SetXPos(int x)
{
	m_iX = x+(m_iWidth/2);
}

//-----------------------------------------------------------------------------
// Name: ()
// Desc: 
//-----------------------------------------------------------------------------
void CSPRITE::SetYPos(int y)
{
	m_iY = y+(m_iHeight/2);
}

//-----------------------------------------------------------------------------
// Name: ()
// Desc: 
//-----------------------------------------------------------------------------
void CSPRITE::SetRows(int m)
{
	m_iRow = m;
}

//-----------------------------------------------------------------------------
// Name: ()
// Desc: 
//-----------------------------------------------------------------------------
void CSPRITE::SetColumns(int n)
{
	m_iCol = n;
}

//-----------------------------------------------------------------------------
// Name: ()
// Desc: 
//-----------------------------------------------------------------------------
void CSPRITE::SetVX(float vx)
{
	m_fVX = vx;
}

//-----------------------------------------------------------------------------
// Name: ()
// Desc: 
//-----------------------------------------------------------------------------
void CSPRITE::SetVY(float vy)
{
	m_fVY = vy;
}

//-----------------------------------------------------------------------------
// Name: ()
// Desc: 
//-----------------------------------------------------------------------------
void CSPRITE::SetState(int state)
{
	m_iState = state;
}

//-----------------------------------------------------------------------------
// Name: ()
// Desc: 
//-----------------------------------------------------------------------------
void CSPRITE::SetAttr(int attr)
{
	m_iAttr = attr;
}

//-----------------------------------------------------------------------------
// Name: DrawSprite()
// Desc: Draws a list of sprites to a surface
//-----------------------------------------------------------------------------
int CSPRITE::DrawSprite(LPDIRECTDRAWSURFACE7 lpddsSurface)
{
	static RECT destRect;
	//static int j=0;
	//static DWORD start=GetTickCount();
	//static CTIMER timer;
	
	if( (m_iState != SPRITE_STATE_DEAD) && ( m_iAttr == SPRITE_ATTR_LOADED) )
	{
		//Fill in dest rect
		destRect.top = m_iY;
		destRect.left = m_iX;
		destRect.bottom = m_iY + m_iHeight;
		destRect.right = m_iX + m_iWidth;
	
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
		ddReturnVal = lpddsSecondary->Blt(&destRect/*dest rect*/,m_lpddsImage[m_iCurrentFrame], //pointer to source surface
										  &m_Rect, //pointer to the source rectangle
										  DDBLT_WAIT | DDBLT_KEYSRC, NULL/*ddbltfx struct*/);
		if (DDFailedCheck(ddReturnVal, "Blt failed", cpErrorBuf ))
		{	MessageBox(main_window_handle, cpErrorBuf, "DrawSprite()", MB_ICONEXCLAMATION);   return(0); }
	}//End if( (m_iState != SPRITE_STATE_DEAD) && ( m_iAttr == SPRITE_ATTR_LOADED) )
	
	return(1);
}

//=========================================================FUNCTIONS===========================================================================

//-----------------------------------------------------------------------------
// Name: Flip()
// Desc: this function flip the primary surface with the secondary surface
//-----------------------------------------------------------------------------   
int Flip()
{// this function flip the primary surface with the secondary surface

	// flip pages
	while( (ddReturnVal=lpddsPrimary->Flip(NULL, DDFLIP_WAIT) )!= DD_OK)
	{
		if (DDFailedCheck(ddReturnVal, "Flip() failed", cpErrorBuf ))
			{	MessageBox(main_window_handle, cpErrorBuf, "Flip()", MB_ICONEXCLAMATION);   return(0); }
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
	if (DDFailedCheck(ddReturnVal, "Lock() failed", cpErrorBuf ))
	{	
		if(ddReturnVal==DDERR_SURFACELOST)
		{
			lpddsSurface->Restore();
			return(1);
		}
		else
		{
			MessageBox(main_window_handle, cpErrorBuf, "Lock()", MB_ICONEXCLAMATION);
			return(0); 
		}
	}
	
	return (1);
}
//-----------------------------------------------------------------------------
// Name: UnLock()
// Desc: UnLocks entire surface that has been locked
//-----------------------------------------------------------------------------
int UnLock(LPDIRECTDRAWSURFACE7 lpddsSurface)
{
	ddReturnVal  = lpddsSurface->Unlock(NULL);
	if (DDFailedCheck(ddReturnVal, "Unlock() failed", cpErrorBuf ))
	{	MessageBox(main_window_handle, cpErrorBuf, "UnLock()", MB_ICONEXCLAMATION);   return(0); }

	return(1);
}