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
#include "SCMath.h"
#include "CUtilities.h"
#include "General.h"

////DEFINES////////////////////
#define MAX_SPRITE_FRAMES 200
#define MAX_SPRITE_ANIMATIONS 200
#define MAX_SPRITES 100

#define SPRITE_ATRR_NOTLOADED 2
#define SPRITE_ATTR_LOADED    3

#define SPRITE_STATE_DEAD     0x00000001   //Do not draw sprite
#define SPRITE_STATE_ALIVE    0x00000010   //Sprite being draw but no animation

#define SPRITE_ANIM_ONCE 0x00000100   //Animate sprite once then state switch to Done
#define SPRITE_ANIM_CONT 0x00001000   //Continuely animate the sprite
#define SPRITE_ANIM_DONE 0x00010000   //Sprite is done with animation and just sits on screen

////GLOBALS////////////////////

////PROTOTYPES////////////////


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
	int     m_nWidth, m_nHeight;                    //the height and width of a frame of the sprite
	RECT    m_Rect;                                 //has the bounding rect of the sprite
	int     m_iRow, m_iCol;                         //Number of rows and columns that are in the bitmap of frames
	
	int     m_iX, m_iY;                             //position of the sprite on the screen
	//float   m_fVX0, m_fVY0;                         //init velocity in x,y
	//float   m_Mass;                                 //Total mass of the sprite
	//CVECTOR m_CenterofMass;                         //Center of mass of the sprite grams
	//CVECTOR m_VelVector;                            //velocity vector
	//float   m_MomentofInertiaZ;                     //moment of inertia about the z axes g-s^2-m
		
	int     m_iState;                               //the state of the object
	int     m_iAttr;                                //attributes pertaining to the object (extra)
	int     m_ipAnimAttr[MAX_SPRITE_ANIMATIONS];    //Tells whether sprite anim seq is cont or once
	int     m_ipAnimSeqMax[MAX_SPRITE_ANIMATIONS];  //Parallel array of the max number of frames in a sequence
	int     *m_ipAnimations[MAX_SPRITE_ANIMATIONS]; //array of pointers to the order in which the frames will be displayed
	LPDIRECTDRAWSURFACE7 m_lpddsImage[MAX_SPRITE_FRAMES];

	CUtilities  m_Utils;
	CDDSettings m_Settings;

public:
	//Member variables
	CTIMER  m_Timer;
	PHYSPROP physProp;
	
	//Methods
	CSPRITE                        ( );
	CSPRITE                        (int width, int height, int iNumofFrames, int NumofColumns,int NumofRows, int x, int y, float vx0, 
		                            float vy0, int TimeBetweenFrames, int state);
	CSPRITE                        (int width, int height, int iNumofFrames, int NumofColumns,int NumofRows);
	CSPRITE                        (int width, int height, int iNumofFrames, int NumofColumns,int NumofRows, int TimeBetweenFrames);
	~CSPRITE                       ( );
	
	int  LoadSpriteImage           (LPCTSTR filename, int w=NULL, int h=NULL);
	int  LoadBMImage               ( LPDIRECTDRAWSURFACE7 &lpddsSurface, LPCTSTR strBMP, DWORD dwDesiredWidth=NULL, DWORD dwDesiredHeight=NULL );
	int  LoadSpriteImage16         (char * filename);//creates surfaces for all the cells and copies image to surface
	int  LoadSpriteImage24         (char * filename);//creates surfaces for all the cells and copies image to surface
	HRESULT DrawBitmap             ( LPDIRECTDRAWSURFACE7&, HBITMAP hBMP, DWORD dwBMPOriginX, DWORD dwBMPOriginY, DWORD dwBMPWidth, DWORD dwBMPHeight );
	int  UnLoadSpriteImage         ( );             //deletes the sprite
	int  RestoreSurface            ( ); //restores all the surfaces of the sprite 
	void InitSprite                (int width, int height, int iNumofFrames, int NumofColumns,int NumofRows, int x, int y, float vx0, 
		                            float vy0, int TimeBetweenFrames, int state);
	void InitSprite                (int width, int height, int iNumofFrames, int NumofColumns,int NumofRows);
	void InitSprite                (int width, int height, int iNumofFrames, int NumofColumns,int NumofRows, int TimeBetweenFrames);
	int  DrawSprite                ( LPDIRECTDRAWSURFACE7 );
	int  DrawSprite                (LPDIRECTDRAWSURFACE7, int x, int y);
	int  DrawSprite                (LPDIRECTDRAWSURFACE7 lpddsSurface, RECT &destRect);

	//int GetCount();
	LPDIRECTDRAWSURFACE7 GetFrame  (int frameNumber);
	int  GetWidth                ( );
	int  GetHeight               ( );
	RECT GetRect                 ( );
	int  GetNumberofFrames       ( );
	int  GetCurrentFrameNum      ( );
	int  GetCurrentAnimNum       ( );
	int  GetTimeBetweenFrames    ( );
	int  GetAnimationIndex       ( );
	int *GetAnimation            ( int );
	int  GetRows                 ( );
	int  GetColumns              ( );

	int     GetXPos              ( );
	int     GetYPos              ( );
	//float   GetVX0();
	//float   GetVY0();
	//CVECTOR GetVelVector();
	//float   GetTotalMass();
	//float   GetCenterofMass();
	//float   GetMomentofInertiaZ();

	int         GetState          ( );
	int         GetAttr           ( );
	int         GetAnimSeqMax     ( int );
	int         GetAnimAttr       ( int index );
	CUtilities* getUtilitiesClass ( )              {return &m_Utils;}
	
	//void  IncrementCounter();
	int  Equals                     (CSPRITE*);
	LPDIRECTDRAWSURFACE7* GetImages ( );
	//void SetCount(int);
	void SetSettings                ( CDDSettings Settings) { m_Settings = Settings; };
	void SetUtilityClass            ( CUtilities u )        { m_Utils = u; }
	void SetAnimations              (int * anim, int Seqmax, int attribute);
	void SetAnimations              (int * anim, int SeqMax, int attribute, int AnimsIndex);
	void SetAnimAttr                (int AnimsIndex, int attr);
	void SetAnimationIndex          (int);
	int  SetCurrentAnim             (int);
	void SetWidth                   (int);
	void SetHeight                  (int);
	void SetRect                    (RECT);
	void SetRect                    (int,int,int,int);
	void SetNumberofFrames          (int);
	void SetTimeBetweenFrames       (int);
	int  SetCurrentFrameNum         (int);
	void SetRows                    (int);
	void SetColumns                 (int);
	
	void SetXPos                    (int);
	void SetYPos                    (int);
	//void SetVX0(float);
	//void SetVY0(float);
	//void SetVelVector(float,float);
	//void SetVelVector(CVECTOR);
	//void SetTotalMass(float);
	//void SetCenterofMass(float);
	//void SetMomentofInertiaZ(float);
	
	void SetState(int);
	void SetAttr(int);
};

#endif
