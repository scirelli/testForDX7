//--------------------------------------------------------
// File: CTetris.h
//
// Desc: The logic and implementation for a tetris game
//       takes care of everything about the tetris game 
//       but the display
// Note: mxn is row x col
//
// Fixes: Color problem
//        Change probablity for certian blocks
//		  
// Add on: Add box around next block
//         Add two player
//         Add AI player
//         Add intro and credits
//         Add keystroke logger for replays make a replay
//           struct
//--------------------------------------------------------

#ifndef CTetris_h
#define CTetris_h

////INCLUDES////////////
#include "SCTimer.h"
#include <memory.h>
#include <time.h>
#include <conio.h>
#include "replay.h"

//#include <windows.h>

//// FORWARD DECLARATIONS/////////////
class CTetris;

////TYPEDEFs////////////
//for display callbacks.
typedef int  (*DISPLAYBOARD)( CTetris& );
typedef void (*DRAWROWCOMPLETION) ( CTetris&, int );
typedef int (*DISPLAYWIN_LOSE) ( CTetris& );

////CONST///////////////
const int NUM_OF_ROWS       = 15;//
const int NUM_OF_COL        = 10;//
const int NUM_OF_LVLS       = 10;//
const int BLOCKS_PER_TET    = 4;//
const int SIZE_OF_BOARD     = NUM_OF_ROWS*NUM_OF_COL;//
const int NUM_OF_TETRADS    = 7;//
const int BLOCK_STATE_EMPTY = 0;
const int BLOCK_STATE_FULL  = 1;
const int CW                = 90;
const int CCW               = -90;
const float FAST_FALL       = 0;
const int FPS               = 15;
const int ADVANCE_NEXT_LVL  = 10;
const int SEED_COUNT        = 5;
const int BK_COLOR          = 0;
const int CONT_PLAYING      = 2;
const int WIN               = 1;
const int LOSE              = 0;
const int WINNING_SCORE     = 100;

const int SALLY    = 1,
          LAUREN   = 2,
          MICHELLE = 3,
          TATIANA  = 4,
          JANNET   = 0,
          KIM      = 6,
          KATE     = 5;

////MACROS/////////////
// these read the keyboard asynchronously
#define KEY_DOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)
#define KEY_UP(vk_code)   ((GetAsyncKeyState(vk_code) & 0x8000) ? 0 : 1)
//#define CONVERTBOARDPOS(x, y) ( (x+(y*NUM_OF_COL)) )
#define Deg_Rad(x) ( (x/180)*3.14159265f )

////STRUCTS/////////////
struct MyColor
{
	float r,g,b;
	MyColor( ) : r(0),g(0),b(0) { }
	MyColor( float red, float gre, float blue ) { r=red; g=gre; b=blue; }
	MyColor( MyColor& col ) { r=col.r; b=col.b; g=col.g; }
};

struct Block
{
	int iX, iY;
	float r,g,b;
	int nState;

	Block() : iX(0), iY(0), r(0.0f), g(0.0f), b(0.0f), nState(BLOCK_STATE_EMPTY){}
	Block( int row,int col ) { iX=row; iY=col; }
	Block( MyColor& col ) { r = col.r; g = col.g; b = col.b; }
};

struct Tetrad
{
	Block tet[BLOCKS_PER_TET];
	int iX, iY;
	int iIndexCenterBlock;
	
	Tetrad()
	{
		for(int i=0; i<BLOCKS_PER_TET; i++)
		{ tet[i].iX = 0; tet[i].iY = 0; tet[i].r = tet[i].g = tet[i].b = 0;tet[i].nState = BLOCK_STATE_EMPTY; }
		iX = 0;
		iY = 0;
		iIndexCenterBlock = -1;
	}
	Tetrad(Tetrad &c)
	{
		for(int i=0; i<BLOCKS_PER_TET; i++)
		{ 
			tet[i].iX = c.tet[i].iX; 
			tet[i].iY = c.tet[i].iY;
			tet[i].r = c.tet[i].r;
			tet[i].g = c.tet[i].g;
			tet[i].b = c.tet[i].b;
		}
		iX = c.iX;
		iY = c.iY;
		iIndexCenterBlock = c.iIndexCenterBlock;
	}
	Tetrad( Block &a, Block &b, Block &c, Block &d )
	{
		tet[0] = a;
		tet[1] = b;
		tet[2] = c;
		tet[3] = d;
		iX = iY = 0;
		iIndexCenterBlock = -1;
		for(int i=0; i<BLOCKS_PER_TET; i++)
		{
			tet[i].r=0; tet[i].g=0; tet[i].b=0;
		}
	}
	Tetrad(MyColor& col) 
	{  
		for(int i=0; i<BLOCKS_PER_TET; i++)
		{
			tet[i].r=col.r; tet[i].g=col.b; tet[i].b=col.g;
		}
	}
};

////CLASSES/////////////
//------------------------------------------------------
// Name: CTetris
//------------------------------------------------------
class CTetris
{
protected:
	//----Member Variables----
	int     *m_npBoard;
	Block   *m_pBoard;
	MyColor m_ClearColor;

	int     m_nLevel;
	int     m_nRowsCompleted;
	int     m_nTotalRowsCompleted;
	int     m_nWhichRowCompleted[BLOCKS_PER_TET];        //tells which row was just completed
	int     m_nNumofRows;
	int     m_nNumofCols;
	//int     m_nNumofLvls;
	int     m_nSizeofBoard;
	//int     m_nNumofTetrads;
	int     m_nAdvanceNextLvl;
	float   m_fRotationAngle;
	
	bool    m_bExit;
	bool    m_bPause;
	bool    m_bKeyState[256];
	
	int     m_nCurrentTetName;          //the one and only moving tetrad
	Tetrad  m_CurrentTet;               //the one and only moving tetrad
	int     m_nNextTetName;
	//Tetrad  m_NextTet;
	
	CTIMER3 m_FallTimer;                //keeps track of time between each down ward movment
	CTIMER3 m_FrameTimer;               //keeps track of time it takes to render one frame
	float   m_fCurrFallDelay;
	static float m_fFallDelay[ NUM_OF_LVLS ]; //in sec
	
	CReplay m_Replay;

	//"Pictures" of All the tetrads centered at origan
	static Tetrad m_Tetrads[ NUM_OF_TETRADS ];
	
	//----Private Methods----
	void FrameAdvance( );
	void TranslateCurrTet( int, int );
	void TranslateTet( Tetrad&, int, int );
	void RotateCurrTet( int ); 
	void AnimateObjects( );
	void ClearOldBlockFromBoard( );
	bool Collision( int x, int y );
	bool CheckRowCompletion( );
	bool ClearRow( int row );
	void RenderBlockintoBoard();
	void PickTetrad( );
	bool MoveTetrad( );
	int  RandomTetrad( );
	float RandomColor( );
	DISPLAYBOARD DisplayBoard;
	DRAWROWCOMPLETION AnimRowCompletion;
	DISPLAYWIN_LOSE DisplayWin;
	DISPLAYWIN_LOSE DisplayLose;
	//Virtual methods
	virtual void ProcessInput( );
	virtual void Scoreing( );
	virtual int  WinLose( );
	//Static methods for init of callbacks
	static int  DefaultDisplay( CTetris& def ) { return 0; }
	static void DefualtAnimRowCompl( CTetris& defTet, int def ) { }
	static int  DefualtDisplayWin( CTetris &defTet ) { return 0; }
	static int  DefaultDisplayLose( CTetris &defTet ) { return 0; }
	
public:
	//----Public Methods----
	CTetris( );
	CTetris( DISPLAYBOARD, int cols=NUM_OF_COL, int rows=NUM_OF_ROWS );
	virtual ~CTetris( )
	{
		if(m_pBoard) delete [] m_pBoard;
		if(m_npBoard) delete [] m_npBoard;
		m_pBoard = NULL;
		m_npBoard = NULL;
	}

	virtual int  BeginGame( );
	virtual bool ShotDown( ){ return true; };
	
	int ConvertToBoardXY( int x, int y ) { return( x+(y*m_nNumofCols) ); }
	//gets
	float  getFallDelay( )         { return m_fCurrFallDelay; }
	int    getNumRowsCompleted( )  { return m_nTotalRowsCompleted; }
	int    getLevel( )             { return m_nLevel; }
	bool   getPauseState( )        { return m_bPause; }
	int    getNumberofRows( )      { return m_nNumofRows; }
	int    getNumofCols( )		   { return m_nNumofCols; }
	//int    getNumofLvls( )		   { return m_nNumofLvls; }
	//int    getBlocksPerTet( )	   { return m_nBlocksPerTet; }
	int    getSizeofBoard( )	   { return m_nSizeofBoard; }
	//int    getNumofTetrads( )	   { return m_nNumofTetrads; }
	int    getNextLvlCount( )	   { return m_nAdvanceNextLvl; }
	Block* getBlockBoard( )        { return m_pBoard; }
	const int* getWhichRowCompleted( ) { return m_nWhichRowCompleted; }
	void   getNextTet(Tetrad &tet, int x=0, int y=0) { tet = CTetris::m_Tetrads[m_nNextTetName]; CTetris::TranslateTet(tet,x,y); }
	int*   getBoard( ) 
	{	
		for(int i=0; i<m_nSizeofBoard; i++) m_npBoard[i] = m_pBoard[i].nState;
		return m_npBoard; 
	}
	//sets
	void setFallDelay( float delay )     { m_fCurrFallDelay = delay; }
	void setClearColor( MyColor col )    { m_ClearColor = col; }
	//void setNumofLvls(int lvl)           { m_nNumofLvls = lvl; }
	//void setBlocksPerTet( int b )        { m_nBlocksPerTet = b; }
	//void setSizeofBoard( int sz )      { m_nSizeofBoard = sz; }
	//void setNumofTetrads( int n )        { m_nNumofTetrads = n; }
	void setNextLvlCount( int n )        { m_nAdvanceNextLvl = n; }
	void setClearColor( float r, float g, float b )        { m_ClearColor.r = r; m_ClearColor.b = b; m_ClearColor.g = g; }
	void setDisplayBoard( DISPLAYBOARD CallBackFunc )      { DisplayBoard = CallBackFunc; }
	void setAnimRowCompl( DRAWROWCOMPLETION CallBackFunc ) { AnimRowCompletion = CallBackFunc; }
	void setNumofRows( int row )         
	{ 
		m_nNumofRows = row; 
		m_nSizeofBoard = m_nNumofRows*m_nNumofCols; 
		if(m_pBoard) delete [] m_pBoard;
		if(m_npBoard) delete [] m_npBoard;
		m_pBoard = new Block[m_nSizeofBoard];
		m_npBoard = new int[m_nSizeofBoard];
	}
	void setNumofCols( int col )         
	{ 
		m_nNumofCols = col; 
		m_nSizeofBoard = m_nNumofRows*m_nNumofCols; 
		if(m_pBoard) delete [] m_pBoard;
		if(m_npBoard) delete [] m_npBoard;
		m_pBoard = new Block[m_nSizeofBoard];
		m_npBoard = new int[m_nSizeofBoard];
	}
	void setBoard( Block board[], int start = 0, int size = SIZE_OF_BOARD ) { for( int i=start; i<size; i++ )	m_pBoard[i] = board[i];	}
	void setDisplayWin( DISPLAYWIN_LOSE CallBackFunc )  { DisplayWin = CallBackFunc; }
	void setDisplayLose( DISPLAYWIN_LOSE CallBackFunc ) { DisplayLose = CallBackFunc; }
};

#endif