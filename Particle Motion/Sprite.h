//-----------------------------------------------------------------------------
// File: Sprite.h
//
// Desc: All of sprite functions are defined here as well as the sprite class
//       A sprite is an animated bitmap
//-----------------------------------------------------------------------------

#ifndef Sprite_h
#define Sprite_h

////INCLUDES///////////////////
#include <windows.h>
#include <windowsx.h>
#include <ddraw.h>
#include "ErrorCheck.h"
#include "CreateRGB.h"
#include "Bitmap.h"
#include "SCTimer.h"

////DEFINES////////////////////
#define MAX_SPRITE_FRAMES 100
#define MAX_SPRITE_ANIMATIONS 10
#define MAX_SPRITES 100

#define SPRITE_ATRR_NOTLOADED 2
#define SPRITE_ATTR_LOADED    3

#define SPRITE_STATE_DEAD     0x00000001   //Do not draw sprite
#define SPRITE_STATE_ALIVE    0x00000010   //Sprite being draw but no animation

#define SPRITE_ANIM_ONCE 0x00000100   //Animate sprite once then state switch to Done
#define SPRITE_ANIM_CONT 0x00001000   //Continuely animate the sprite
#define SPRITE_ANIM_DONE 0x00010000   //Sprite is done with animation and just sits on screen

////GLOBALS////////////////////
extern LPDIRECTDRAW7        lpddObj;                // dd object
extern LPDIRECTDRAWSURFACE7 lpddsPrimary;           // dd primary surface
extern LPDIRECTDRAWSURFACE7 lpddsSecondary;         // dd back surface
extern LPDIRECTDRAWPALETTE  lpddpal;                // a pointer to the created dd palette
extern DDSURFACEDESC2       ddsd;                   // a direct draw surface description struct
extern DDSCAPS2             ddscaps;                // a direct draw surface capabilities struct
extern DDBLTFX              ddbltfx;                // The Blit Fx structure used for all blting
extern HRESULT              ddReturnVal;            // result back from dd calls
extern char                 cpErrorBuf[1024];       // Build up error messages
extern RECT                 wndRect;                // A rect the size of the window
extern HWND                 main_window_handle;     // save the window handle
extern SCCOLOR              TransColor;

////PROTOTYPES////////////////
int Flip();
LPDIRECTDRAWSURFACE7 CreateSurface(int width, int height,SCCOLOR TransparentColor);
int Lock(LPDIRECTDRAWSURFACE7);
int UnLock(LPDIRECTDRAWSURFACE7);

//-----SPRITE CLASS-----------
class CSPRITE
{
private:
	//CBITMAP m_allBitmapFrames();                  //the entire bitmap of all the frames of a sprite
	int     m_Counter;                              //arbitrary counter
	int     m_iNumberofFrames;
	int     m_iCurrentFrame;
	int     m_iTimeBetweenFrames;
	int     m_iCurrentAnimSequence;
	int     m_iAnimationIndex;                      //holds the last spot used for m_ipAnimations[]
	int     m_iWidth, m_iHeight;                    //the height and width of a frame of the sprite
	RECT    m_Rect;                                 //has the bounding rect of the sprite
	int     m_iRow, m_iCol;                         //Number of rows and columns that are in the bitmap of frames
	int     m_iX, m_iY;                             //position of the sprite on the screen
	float   m_fVX, m_fVY;                           //velocity in x,y
	int     m_iState;                               //the state of the object
	int     m_iAttr;                                //attributes pertaining to the object (extra)
	int     m_ipAnimAttr[MAX_SPRITE_ANIMATIONS];    //Tells whether sprite anim seq is cont or once
	int     m_ipAnimSeqMax[MAX_SPRITE_ANIMATIONS];  //Parallel array of the max number of frames in a sequence
	int     *m_ipAnimations[MAX_SPRITE_ANIMATIONS]; //array of pointers to the order in which the frames will be displayed
	LPDIRECTDRAWSURFACE7 m_lpddsImage[MAX_SPRITE_FRAMES];

public:
	CSPRITE();
	CSPRITE(int width, int height, int iNumofFrames, int NumofColumns,int NumofRows, int x, int y, float vx, float vy, int TimeBetweenFrames,
		    int state);
	CSPRITE(int width, int height, int iNumofFrames, int NumofColumns,int NumofRows);
	CSPRITE(int width, int height, int iNumofFrames, int NumofColumns,int NumofRows, int TimeBetweenFrames);
	~CSPRITE();
	int LoadSpriteImage(char * filename);//creates surfaces for all the cells and copies image to surface
	int UnLoadSpriteImage();             //deletes the sprite
	int RestoreSurface(); //restores all the surfaces of the sprite
	LPDIRECTDRAWSURFACE7 GetFrame(int frameNumber);
	void InitSprite(int width, int height, int iNumofFrames, int NumofColumns,int NumofRows, int x, int y, float vx, float vy, int TimeBetweenFrames,
					int state);
	void InitSprite(int width, int height, int iNumofFrames, int NumofColumns,int NumofRows);
	void InitSprite(int width, int height, int iNumofFrames, int NumofColumns,int NumofRows, int TimeBetweenFrames);
	int DrawSprite(LPDIRECTDRAWSURFACE7);
	CTIMER  m_Timer;
	
	//int GetCount();
	int GetWidth();
	int GetHeight();
	RECT GetRect();
	int GetNumberofFrames();
	int GetCurrentFrameNum();
	int GetCurrentAnimNum();
	int GetTimeBetweenFrames();
	int GetAnimationIndex();
	int * GetAnimation(int);
	int GetXPos();
	int GetYPos();
	int GetRows();
	int GetColumns();
	float GetVX();
	float GetVY();
	int GetState();
	int GetAttr();
	int GetAnimSeqMax(int );
	int GetAnimAttr(int index);
	
	//void  IncrementCounter();
	int  Equals(CSPRITE*);
	LPDIRECTDRAWSURFACE7* GetImages();
	//void SetCount(int);
	void SetAnimations(int * anim, int Seqmax, int attribute);
	void SetAnimations(int * anim, int SeqMax, int attribute, int index);
	void SetAnimAttr(int index, int attr);
	void SetAnimationIndex(int);
	int  SetCurrentAnim(int);
	void SetWidth(int);
	void SetHeight(int);
	void SetRect(RECT);
	void SetRect(int,int,int,int);
	void SetNumberofFrames(int);
	void SetTimeBetweenFrames(int);
	int  SetCurrentFrameNum(int);
	void SetXPos(int);
	void SetYPos(int);
	void SetRows(int);
	void SetColumns(int);
	void SetVX(float);
	void SetVY(float);
	void SetState(int);
	void SetAttr(int);
};

#endif