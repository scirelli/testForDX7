//--------------------------------------------------------
// File: CTetris.h
//
// Desc: The logic and implementation for a tetris game
//       takes care of everything about the tetris game 
//       but the display
// Note: mxn is row x col
//
// Fixes: Sprite class lot of work. Have to use internal
//           rects for des rect drawing. Redo alot of the
//           code
//        Problem with DrawTextGDI() going to have to 
//        write a font class
//--------------------------------------------------------

#ifndef CTetris_h
#define CTetris_h

////INCLUDES////////////
#include "SCTimer.h"
#include <memory.h>
#include <time.h>
#include "replay.h"
#include "CDDGameApp.h"
#include <fstream>
#include <string>
//#include <windows.h>

//// FORWARD DECLARATIONS/////////////
class CTetris;

////TYPEDEFs////////////

////CONST///////////////
const int   BLOCK_STATE_EMPTY = 0;
const int   BLOCK_STATE_FULL  = 1;
const int   CW                = 90;
const int   CCW               = -90;
const float FAST_FALL         = 0;
const int   FPS               = 15;
const int   SEED_COUNT        = 5;
const int   BK_COLOR          = 0;
const bool  DRAW_GRID         = true;

const int   NUM_OF_ROWS       = 18;//15 18
const int   NUM_OF_COL        = 10;//10 10
const float RATIO_X           = (float)WINDOW_WIDTH/(float)WINDOW_HEIGHT;
const float RATIO_Y           = (float)WINDOW_HEIGHT/(float)WINDOW_WIDTH;
const int   BLOCKS_PER_TET    = 4;//
const int   NUM_OF_TETRADS    = 7;//
const float SCALE_FACTOR_X    = 100; //100 0
const float SCALE_FACTOR_Y    = 150;//150 48
const int   BOARD_WIDTH       = (WINDOW_WIDTH/4)+SCALE_FACTOR_X;//(int)(((float)WINDOW_WIDTH/4.f)+ (float)SCALE_FACTOR_X);
const int   BOARD_HEIGHT      = (WINDOW_HEIGHT/2)+SCALE_FACTOR_Y;//(int)(((float)(WINDOW_HEIGHT)/2.f)+ (float)SCALE_FACTOR_Y);

const float ANIMATION_DELAY = 0.25f;

const int NUM_OF_LVLS       = 10;//
const int ADVANCE_NEXT_LVL  = 20;
const int WINNING_SCORE     = 100;
const int HS_TEXT_BUFFER_X  = 30;
const int HS_TEXT_BUFFER_Y  = 20;
const int HS_TEXT_SIZE      = 20;
const int HS_NUM_SIZE       = 18;
const int S_TEXT_SIZE       = HS_TEXT_SIZE;
const int S_NUM_SIZE        = HS_NUM_SIZE;
const int S_TEXT_BUFFER_X   = HS_TEXT_BUFFER_X;
const int S_TEXT_BUFFER_Y   = HS_TEXT_BUFFER_Y + 44;
const int LVL_TEXT_SIZE     = HS_TEXT_SIZE;
const int LVL_NUM_SIZE      = HS_NUM_SIZE;
const int LVL_TEXT_BUFFER_X = S_TEXT_BUFFER_X;
const int LVL_TEXT_BUFFER_Y = S_TEXT_BUFFER_Y + 44;
const int ROW_WORTH         = NUM_OF_COL+BLOCKS_PER_TET;
const int THOUGHT_WORTH     = 12;

const int SALLY    = 1,
          LAUREN   = 2,
          MICHELLE = 3,
          TATIANA  = 4,
          JANNET   = 0,
          KIM      = 6,
          KATE     = 5;

////MACROS/////////////
//#define CONVERTBOARDPOS(x, y) ( (x+(y*NUM_OF_COL)) )

////STRUCTS/////////////
//---------------------------------------
//  Struct: Block
//---------------------------------------
struct Block
{
	int      nX, nY;
	SCCOLOR  color;
	int      nState;
	CSPRITE *pTexture;
	RECT     rectBlockSize;

	Block() : nX(0), nY(0), nState(BLOCK_STATE_EMPTY), pTexture(NULL) 
	{ 
		rectBlockSize.bottom = rectBlockSize.left = rectBlockSize.right = rectBlockSize.top = 0; 
	}
	Block( int row,int col ) 
	{ 
		nX = row; nY = col; 
		pTexture = NULL; 
		rectBlockSize.bottom = rectBlockSize.left = rectBlockSize.right = rectBlockSize.top = 0; 
		nState = BLOCK_STATE_EMPTY;
	}
	Block( SCCOLOR col ) 
	{ 
		color = col; 
		nX = 0; nY = 0; 
		pTexture = NULL; 
		nState = BLOCK_STATE_EMPTY;
		rectBlockSize.bottom = rectBlockSize.left = rectBlockSize.right = rectBlockSize.top = 0; 
	}
	Block( SCCOLOR col, CSPRITE *texture ) 
	{ 
		color = col; 
		nX = 0; nY = 0;
		pTexture = texture;
		nState = BLOCK_STATE_EMPTY;
		rectBlockSize.bottom = rectBlockSize.left = rectBlockSize.right = rectBlockSize.top = 0; 
	}
	~Block() { pTexture = NULL; }
/*
	Block& operator = ( Block& b )
	{
		this->color    = b.color;
		this->nState   = b.nState;
		this->pTexture = b.pTexture;
		return *this;
	}
*/
	void RenderBlock( LPDIRECTDRAWSURFACE7 lpddsSurface )
	{
		RECT dest = {rectBlockSize.left+nX,rectBlockSize.top+nY,rectBlockSize.right+nX,rectBlockSize.bottom+nY};
		if( nState == BLOCK_STATE_FULL )
			pTexture->DrawSprite( lpddsSurface, dest );
	}
	void RenderBlockAbsolute( LPDIRECTDRAWSURFACE7 lpddsSurface, int x, int y )
	{
		RECT dest = {x, y, rectBlockSize.right+x,rectBlockSize.bottom+y};
		if( nState == BLOCK_STATE_FULL )
			pTexture->DrawSprite( lpddsSurface, dest );
	}
};

//-----------------------------------
// Struct: Tetrad
//-----------------------------------
struct Tetrad
{
	Block tet[BLOCKS_PER_TET];
	int nX, nY;
	SCCOLOR color;
	CSPRITE *pTetradsTexture;
	
	Tetrad( );
	Tetrad( Tetrad & );
	Tetrad( CSPRITE & ); 
	Tetrad( Block &, Block &, Block &, Block & );
	Tetrad( SCCOLOR & ) ;
};

////CLASSES/////////////
//------------------------------------------------------
// Name: CBoard
//------------------------------------------------------
class CBoard
{
private:
	//--------------------------
	// Private member vars
	//--------------------------
	Block  *m_pBoard;
	SCCOLOR m_bkColor;
	SCCOLOR m_gridColor;
	SCCOLOR m_boarderColor;
	RECT    m_boarderRect;
	RECT    m_boarderRectPlusOffset;
	RECT    m_nextBlockRect;
	RECT    m_rectBlockSize;
	int     m_nOffsetX;
	int     m_nOffsetY;
	int     m_nRows;
	int     m_nCols;
	bool    m_bDrawGrid;
	
	//--------------------------
	// Private methods
	//--------------------------
	void DrawGrid  ( HDC, RECT& );
		
public:
	//-------------------------
	// Public methods
	//-------------------------
	CBoard( ) : m_nOffsetX(0), m_nOffsetY(0), m_nCols(NUM_OF_COL), m_nRows(NUM_OF_ROWS), m_pBoard(NULL), m_bDrawGrid(DRAW_GRID)
	{}
	CBoard( int offsetX, int offsetY, int cols, int rows ) 
	{ 
		m_nOffsetX = offsetX; 
		m_nOffsetY = offsetY;
		m_nCols    = cols;
		m_nRows    = rows;
		m_pBoard   = new Block[rows*cols];;
	}
	CBoard( CBoard& boar)
	{
		for( int i=0; i< boar.getBoardSize(); i++ )
			m_pBoard[i] = boar.getBoard()[i];
		m_bkColor      = boar.getBkColor();
		m_gridColor    = boar.getGridColor();
		m_boarderColor = boar.getBoarderColor();
		m_boarderRect  = boar.getBoardRect();
		m_nOffsetX     = boar.getOffsetX();
		m_nOffsetY     = boar.getOffsetY();
		m_nRows        = boar.getRows();
		m_nCols        = boar.getCols();
		m_bDrawGrid    = DRAW_GRID;
	}
	~CBoard()
	{
		if( m_pBoard != NULL ) { delete [] m_pBoard; m_pBoard = NULL; }
		m_nOffsetX = 0; 
		m_nOffsetY = 0;
		m_nCols    = 0;
		m_nRows    = 0;
	}
	
	int  RenderBoard             ( LPDIRECTDRAWSURFACE7 lpddsSurface );
	void RenderNextTetrad        ( LPDIRECTDRAWSURFACE7, Tetrad& );
	void InitBoard               ( RECT *rcBoarder = NULL,
						           int offsetX = -1, int offsetY = -1, int cols = NULL, int rows = NULL,
								   Block *pBoard = NULL, 
					               SCCOLOR *gridColor = NULL, SCCOLOR *bkColor = NULL, SCCOLOR *boarderColor = NULL );
	int  ConvertXtoBoardUnitsX   ( int x );
	int  ConvertYtoBoardUnitsY   ( int y );
	int  ConvertBoardUnitsColtoX ( int x );
	int  ConvertBoardUnitsRowtoY ( int y );
	int  ConvertToBoardXY( int x, int y ) { return( x+(y*m_nCols) ); }
	
	void setRows( int y )
	{
		if( m_pBoard != NULL ) { delete [] m_pBoard; }
		m_pBoard = new Block[y*m_nCols];
		m_nRows = y;
	}
	void setCols( int x )
	{
		if( m_pBoard != NULL ) { delete [] m_pBoard; }
		m_pBoard = new Block[x*m_nRows];
		m_nCols = x;
	}
	void setBoardSize( int x, int y )
	{
		if( m_pBoard != NULL ) { delete [] m_pBoard; }
		m_pBoard = new Block[x*y];
		m_nRows = y;
		m_nCols = x;
	}
	void setOffset       ( int x, int y ) { m_nOffsetX = x; m_nOffsetY = y; }
	void setBkColor      ( SCCOLOR col )  { m_bkColor = col; }
	void setGridColor    ( SCCOLOR col )  { m_gridColor = col; }
	void setBoarderColor ( SCCOLOR col )  { m_boarderColor = col; }
	void setGridOn       ( bool on )      { m_bDrawGrid = on; }
	void setBoarderRect  ( int width, int height );
	//getters
	int     getRows             ( ) { return m_nRows; }
	int     getCols             ( ) { return m_nCols; }
	int     getOffsetX          ( ) { return m_nOffsetX; }
	int     getOffsetY          ( ) { return m_nOffsetY; }
	SCCOLOR getBkColor          ( ) { return m_bkColor; }
	SCCOLOR getGridColor        ( ) { return m_gridColor; }
	SCCOLOR getBoarderColor     ( ) { return m_boarderColor; }
	RECT    getBoardRect        ( ) { return m_boarderRect; }
	RECT    getBoardRcPlusOffset( ) { return m_boarderRectPlusOffset; }
	RECT*   getNextBlockRect    ( ) { return &m_nextBlockRect; }
	RECT    getBlockSizeRect    ( ) { return m_rectBlockSize; }
	int     getBoardSize        ( ) { return m_nRows*m_nCols; }
	Block  *getBoard            ( ) { return m_pBoard; }
	void    getBlockSize        ( RECT & );
	bool    getGridState        ( ) { return m_bDrawGrid; }
};

//------------------------------------------------------
// Name: CTetris
//------------------------------------------------------
class CTetris : public CDDGameApp
{
protected:

	enum PLAY_GAME_STATE
	{
		CONT_PLAY,
		TIMER_ELAPSED,
		ADVANCE_TETRAD,
		ANIMATE_CLEAR,
		PAUSED,
		WIN,
		LOSE,
		EXIT
	};
	enum GAME_STATE
	{
		INIT, //Don't like this find a better way
		INTRO,
		PLAY_GAME,
		CREDITS,
		GAME_EXIT
	};
	//---------------------
	// Member Variables
	//---------------------
	CBoard      m_Board;
	
	int         m_nLevel;
	int         m_nRowsCompleted;
	int         m_nRowsCompletedforClearing;
	int         m_nTotalRowsCompleted; //for scoreing
	int         m_nHighScore[3];
	std::string m_pstrHighScoreNames[3];
	int         m_nScore;
	int         m_ThinkScore;
	int         m_nWhichRowCompleted[BLOCKS_PER_TET];        //tells which row was just completed
	int         m_nAdvanceNextLvl;
	GAME_STATE  m_gameState;
	PLAY_GAME_STATE m_playGameState;

	float      m_fRotationAngle;
	
	bool       m_bExit;
	bool       m_bPause;
		  
	int        m_nCurrentTetName;          //the one and only moving tetrad
	Tetrad     m_CurrentTet;               //the one and only moving tetrad
	int        m_nNextTetName;
		
	CTIMER3    m_FallTimer;                //keeps track of time between each down ward movment
	CTIMER3    m_FrameTimer;               //keeps track of time it takes to render one frame
	float      m_fTotalFallTime;
	float      m_fCurrFallDelay;
	static float m_fFallDelay[ NUM_OF_LVLS ]; //in sec
	
	int        m_nArbitraryAnimCounter;
	CTIMER3    m_ArbAnimationTimer;
	float      m_fTotalAnimaTime;

	CTIMER3    m_IntroTimer;

	CReplay    m_Replay;

	//"Pictures" of All the tetrads centered at origan
	static Tetrad m_Tetrads[ NUM_OF_TETRADS ];
	
	//-----------------------
	//    Private Methods
	//-----------------------
	void  FrameAdvance            ( );
	void  TranslateCurrTet        ( int, int );
	void  TranslateTet            ( Tetrad&, int, int );
	void  RotateCurrTet           ( int ); 
	void  ClearOldBlockFromBoard  ( );
	bool  Collision               ( int x, int y );
	bool  CheckRowCompletion      ( );
	bool  ClearRow                ( int row );
	void  RenderBlockintoBoard    ( );
	void  RenderNextBlock         ( );
	void  PickTetrad              ( );
	bool  MoveTetrad              ( );
	int   RandomTetrad            ( );
	float RandomColor             ( );
	void  ProcessInput            ( );
	void  PrintScoring            ( );
	void  Scoreing                ( bool = false );
	void  LoadHighScore           ( char* = NULL );
	void  SaveHighScore           ( char* = NULL );
	int   WinLose                 ( );
	bool  AnimateRowCompletion    ( );
	bool  BuildSprites            ( );
	bool  InitTetradTextures      ( );
	
	int  getTetradHeight          ( int ntetName );
	int  getTetradWidth           ( int ntetName );
	int  getLowestPos             ( int ntetName );
	int  getLeftestPos            ( int ntetName );
  
	//For debug
	void PrintBoardState( char[] );
	
public:
	//---------------------
	//   Public Methods
	//---------------------
	//CTetris  ( );
	CTetris  ( int cols = NUM_OF_COL, int rows = NUM_OF_ROWS);
	~CTetris ( );
	
	LRESULT DisplayWndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	int     BeginGame    ( );
	bool    InitInstance ( HANDLE hInstance, LPCTSTR lpCmdLine, int iCmdShow );
	bool    ShotDown     ( ){ return true; };

	//int  ConvertToBoardXY( int x, int y ) { return( x+(y*m_nNumofCols) ); }

	//gets
	float         getFallDelay         ( )  { return m_fCurrFallDelay; }
	int           getNumRowsCompleted  ( )  { return m_nTotalRowsCompleted; }
	int           getLevel             ( )  { return m_nLevel; }
	bool          getPauseState        ( )  { return m_bPause; }
	int           getNextLvlCount      ( )	{ return m_nAdvanceNextLvl; }
	const CBoard *getBlockBoard        ( )  { return &m_Board; }
	const int*    getWhichRowCompleted ( )  { return m_nWhichRowCompleted; }
	void          getNextTet           ( Tetrad &tet, int x=0, int y=0 ) { tet = CTetris::m_Tetrads[m_nNextTetName]; CTetris::TranslateTet(tet,x,y); }
	int           getRows              ( )  { return m_Board.getRows(); }
	int           getCols              ( )  { return m_Board.getCols(); }
	
	static Tetrad* getTetrads          ( )  { return CTetris::m_Tetrads; }
	
	//sets
	void setFallDelay    ( float delay )               { m_fCurrFallDelay = delay; }
	void setNextLvlCount ( int n )                     { m_nAdvanceNextLvl = n; }
};

#endif