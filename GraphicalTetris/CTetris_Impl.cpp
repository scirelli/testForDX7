//--------------------------------------------------------
// File: CTetris.cpp
//
// Desc: Impl. of the CTetris class
//--------------------------------------------------------
//Note: Rows fall based on Rows. The only time a block falls is if the row below it is 
//      completly cleared. And they keey falling until the entire row below the falling row
//      is not clear. Every row above the cleared row falls. No state info is needed for all 
//      the blocks.

////INCLUDES///////////
#include "CTetris.h"

//-----------------------------------
// CTetris class Impl
//-----------------------------------
//static tetrads centered at origin
/*
 Jannet  Sally  Lauren  Michelle  Tatiana   Kate   Kim
   x      xx    x           x                 x     x
   x      xx    x           x       x        xx     xx
   x            xx         xx      xxx       x       x
   x
*/
Tetrad CTetris::m_Tetrads[ NUM_OF_TETRADS ] = { Tetrad( Block(0,-2),  Block(0,-1), Block(0,0),  Block(0,1) ),
											    Tetrad( Block(-1,0),  Block(0,-1), Block(-1,-1), Block(0,0) ),
											    Tetrad( Block(0,-1),  Block(0,0),  Block(0,1),  Block(1,1) ),
											    Tetrad( Block(0,-1),  Block(0,0),  Block(0,1),  Block(-1,1) ),
											    Tetrad( Block(0,0),   Block(-1,0), Block(0,-1), Block(1,0) ),
											    Tetrad( Block(0,0),   Block(-1,0), Block(0,-1), Block(-1,1) ),
											    Tetrad( Block(0,0),   Block(-1,0), Block(-1,-1),Block(0,1) ) };
float CTetris::m_fFallDelay[ NUM_OF_LVLS ] = { 0,0,0,0,0,0,0,0,0,0 };

//------------------------------------------------------
// Name: CTetris()
// Desc: Constructor. 
//------------------------------------------------------
CTetris::CTetris( )
{
	srand( (unsigned)time(NULL) );
	m_nLevel          = 1;
	DisplayBoard      = DefaultDisplay;
	DisplayWin        = DefualtDisplayWin;
	DisplayLose       = DefaultDisplayLose;
	m_nNumofRows      = NUM_OF_ROWS;
	m_nNumofCols      = NUM_OF_COL;
	//m_nNumofLvls      = NUM_OF_LVLS;
	//m_nBlocksPerTet   = BLOCKS_PER_TET;
	m_nSizeofBoard    = SIZE_OF_BOARD;
	//m_nNumofTetrads   = NUM_OF_TETRADS;
	m_nAdvanceNextLvl = ADVANCE_NEXT_LVL;
	m_ClearColor      = MyColor(0.f,0.f,0.f);
	m_bExit           = false;
	m_bPause          = false;
	m_nRowsCompleted  = 0;
	AnimRowCompletion = DefualtAnimRowCompl;
	m_nNextTetName    = RandomTetrad();
	m_fFallDelay[0]   = FAST_FALL;
	m_fCurrFallDelay  = m_fFallDelay[m_nLevel];
	m_nTotalRowsCompleted = 0;
	//m_NextTet      = CTetris::m_Tetrads[ m_nNextTetName ];
	//float color = RandomColor();
	//for( int j=0; j<BLOCKS_PER_TET; j++ )
	//	m_NextTet.tet[j].r = m_NextTet.tet[j].g = m_NextTet.tet[j].b = color;
	PickTetrad();
	m_npBoard = new int[SIZE_OF_BOARD];
	m_pBoard  = new Block[SIZE_OF_BOARD];
	for( int i=0; i<SIZE_OF_BOARD; i++ )
		m_npBoard[i] = BLOCK_STATE_EMPTY;
	for( i=1; i<NUM_OF_LVLS; i++ )
		m_fFallDelay[i] = 1.f/(float)i;
}

CTetris::CTetris( DISPLAYBOARD func, int cols, int rows)
{
	srand((unsigned)time(NULL));
	m_nLevel          = 1;
	DisplayBoard      = func;
	m_nNumofRows      = rows;
	m_nNumofCols      = cols;
	//m_nNumofLvls      = NUM_OF_LVLS;
	//m_nBlocksPerTet   = BLOCKS_PER_TET;
	m_nSizeofBoard    = rows*cols;
	//m_nNumofTetrads   = NUM_OF_TETRADS;
	m_nAdvanceNextLvl = ADVANCE_NEXT_LVL;
	m_ClearColor      = MyColor(0.f,0.f,0.f);
	m_bExit           = false;
	m_bPause          = false;
	m_nRowsCompleted  = 0;
	AnimRowCompletion = DefualtAnimRowCompl;
	DisplayWin        = DefualtDisplayWin;
	DisplayLose       = DefaultDisplayLose;
	m_nNextTetName    = RandomTetrad();
	m_fCurrFallDelay  = m_fFallDelay[m_nLevel];
	m_fFallDelay[0]   = FAST_FALL;
	m_nTotalRowsCompleted = 0;
	//m_NextTet      = CTetris::m_Tetrads[ m_nNextTetName ];
	//float color = RandomColor();
	//for( int j=0; j<BLOCKS_PER_TET; j++ )
	//	m_NextTet.tet[j].r = m_NextTet.tet[j].g = m_NextTet.tet[j].b = color;
	PickTetrad();
	m_npBoard  = new int[m_nSizeofBoard];
	m_pBoard   = new Block[m_nSizeofBoard];
	for(int i=0; i<m_nSizeofBoard; i++)
		m_npBoard[i] = BLOCK_STATE_EMPTY;
	for( i=1; i<NUM_OF_LVLS; i++ )
		m_fFallDelay[i] = 1.f/(float)i;
}

//------------------------------------------------------
// Name: BeginGame()
// Desc: Do init here, calls frame advance.
//------------------------------------------------------
int CTetris::BeginGame( )
{
	while(!m_bExit)
		FrameAdvance( );
	return 1;
}

//-----------------------------------------------------------------
// Name: FrameAdvance()
// Desc: Called to signal that we are now rendering the next frame.
//-----------------------------------------------------------------
void CTetris::FrameAdvance( )
{
	static float fTotalFallTime = 0.f;
	
	m_FrameTimer.Tick((float)FPS);
	m_FallTimer.Tick();
	fTotalFallTime += m_FallTimer.GetTimeElapsed();
	
	ClearOldBlockFromBoard( );//clears the block from the board
	if( fTotalFallTime >= m_fCurrFallDelay )
	{   //######################### NOTE: My idea ######################################################
		//#1. Timer runs out                                                                           #
		//#2. check to see if block can move down                                                      #
		//#   a. if so move block down 1 row. done.                                                    #
		//#   b. if not check for row completion                                                       #
		//#      i. if nothing completed, done.                                                        #
		//#      ii. if completed, clear those rows                                                    #
		//#      iii. and everything above that row drops one row down.                                #
		//#           1. if more then one row is completed. Handle in order that you find compelted    #
		//#              meaning, ever row above the completion row moves down one row. and by the time# 
		//#              it reaches a non-complete row it's Y coordinate stores the amount n times that#
		//#              it has to move down                                                           #
		//#   c. start timer.                                                                          #
		//#   d. pick new block                                                                        #
		//#   e. start over.                                                                           #
		//##############################################################################################
		
		if( !MoveTetrad( ) ) //meaning it can't move down
		{
			RenderBlockintoBoard(); //Render the block into the board before you pick a new one
			CheckRowCompletion();
			switch ( WinLose() )
			{
			case WIN :  DisplayWin(*this);   break;
			case LOSE : DisplayLose(*this); m_bExit = true; break;
			}
			PickTetrad( ); //Picks new block and sets up possition
			m_fCurrFallDelay = m_fFallDelay[m_nLevel];
		}
		fTotalFallTime = 0.f;
	}
	
	while(m_bPause) { ProcessInput(); Sleep(10);}
	Scoreing();
	ProcessInput( ); //Gets user input and orientates block
	RenderBlockintoBoard(); //Puts block on the board
	DisplayBoard(*this);
}

//--------------------------------------------------------------------
// Name: Scoreing()
// Desc: Handles scoreing and side affects of scoring
//--------------------------------------------------------------------
void CTetris::Scoreing()
{
	if( m_nRowsCompleted >= m_nAdvanceNextLvl )
	{
		m_nRowsCompleted=0;
		if( m_nLevel <= NUM_OF_LVLS )
			m_nLevel++;
	}
}

//--------------------------------------------------------------------
// Name: CheckRowCompletion()
// Desc: Checks to see if any rows are complete. If they are, the row
//       is cleared, and all rows above it are moved down.
//--------------------------------------------------------------------
/*
bool CTetris::CheckRowCompletion( )
{
	int nCount = 0;
	bool bRetVal = false;

	//I think i can use memcpy(dest,src,count);
	for( int y=0; y<m_nNumofRows; y++ )
	{
		for( int x=0; x<m_nNumofCols; x++ )
		{
			if( m_pBoard[ ConvertToBoardXY(x,y) ].nState == BLOCK_STATE_FULL )
				nCount++;
		}
		if( nCount == m_nNumofCols )
		{//the row is complete 
			for( int k=0; k<m_nNumofCols; k++ ) //clear the row
			{
				m_pBoard[ ConvertToBoardXY(k,y) ].nState = BLOCK_STATE_EMPTY;
				m_pBoard[ ConvertToBoardXY(k,y) ].r = m_pBoard[ ConvertToBoardXY(k,y) ].g = m_pBoard[ ConvertToBoardXY(k,y) ].b = 0;
			}
			//move everything down a row
			memcpy( (void*)&m_pBoard[m_nNumofCols], (void*)m_pBoard, ConvertToBoardXY(x,y-1)*sizeof(Block) );//dest,src,count
			m_RowsCompleted++;
			m_nTotalRowsCompleted++;
			bRetVal = true;
		}
		nCount = 0;
	}
	return bRetVal;
}*/

//--------------------------------------------------------------------
// Name: CheckRowCompletion()
// Desc: Checks to see if any rows are complete. If they are, the row
//       is cleared, and all rows above it are moved down.
//--------------------------------------------------------------------
bool CTetris::CheckRowCompletion( )
{
	int nCount = 0;
	bool bRetVal = false;
	
	//Clear the row which row was completed var
	for(int nIndex=0; nIndex<BLOCKS_PER_TET; nIndex++)
		m_nWhichRowCompleted[nIndex] = 0;
	nIndex=0;//set index back to 0 for reuse
	//I think i can use memcpy(dest,src,count);
	for( int y=0; y<m_nNumofRows; y++ )
	{
		for( int x=0; x<m_nNumofCols; x++ )
		{   //if it's full keep track
			if( m_pBoard[ ConvertToBoardXY(x,y) ].nState == BLOCK_STATE_FULL )
				nCount++;
		}//End for( int x=0; x<m_nNumofCols; x++ )
		if( nCount == m_nNumofCols )
		{//the row is complete 
			//Take care of updating all row information
			m_nRowsCompleted++;
			m_nTotalRowsCompleted++;
			m_nWhichRowCompleted[nIndex++] = y;
			if( nIndex >= BLOCKS_PER_TET ) //cap the nIndex
				nIndex = BLOCKS_PER_TET;
			//Let user do any animation to the row before it is cleared
			AnimRowCompletion(*this, y);
			//Clear the row from memory
			ClearRow(y);
			bRetVal = true;
		}//End if( nCount == m_nNumofCols )
		nCount=0;
	}//End for( int y=0; y<m_nNumofRows; y++ )
	return bRetVal;
}

//--------------------------------------------------------------------
// Name: ClearRow()
// Desc: clears a row from the board starting at location y
//--------------------------------------------------------------------
bool CTetris::ClearRow( int y )
{
	//clear the row from mem
	for( int k=0; k<m_nNumofCols; k++ )
	{
		m_pBoard[ ConvertToBoardXY(k,y) ].nState = BLOCK_STATE_EMPTY;
		m_pBoard[ ConvertToBoardXY(k,y) ] = m_ClearColor;
	}//End for( int k=0; k<m_nNumofCols; k++ )
	//move everything down a row
	for( k=y; k>0; k-- )
	{
		for( int l=0; l<m_nNumofCols; l++ )
			m_pBoard[ ConvertToBoardXY(l,k) ] = m_pBoard[ ConvertToBoardXY(l,k-1) ];
	}//End for( k=y; k>0; k-- )
	return true;
}

//--------------------------------------------------------------------
// Name: WinLose()
// Desc: Checks to see if a win or lose as accured and returns apprope
//       value.
//--------------------------------------------------------------------
int CTetris::WinLose( )
{
	for( int x=0; x<m_nNumofCols; x++ )
		if ( m_pBoard[ConvertToBoardXY(x,0)].nState == BLOCK_STATE_FULL )
			return LOSE;
	if( CTetris::m_nRowsCompleted >= WINNING_SCORE && CTetris::m_nLevel >= NUM_OF_LVLS )
		return WIN;

	return CONT_PLAYING;
}

//--------------------------------------------------------------------
// Name: PickTetrad()
// Desc: Picks a tetrad
//--------------------------------------------------------------------
void CTetris::PickTetrad( )
{
	float color = RandomColor();
	
	m_CurrentTet  = CTetris::m_Tetrads[m_nNextTetName];
	m_nCurrentTetName = m_nNextTetName;
	m_nNextTetName = RandomTetrad(); 
	//m_NextTet     = CTetris::m_Tetrads[m_nNextTetName];
	for(int i=0; i<BLOCKS_PER_TET; i++)
		m_CurrentTet.tet[i].r = m_CurrentTet.tet[i].g = m_CurrentTet.tet[i].b = color;
	TranslateCurrTet(m_nNumofCols/2,1);
}

//--------------------------------------------------------------------
// Name: RandomColor()
// Desc: returns 1 of 7 random colors
//--------------------------------------------------------------------
float CTetris::RandomColor( )
{
	return ( ((((float)rand())/((float)RAND_MAX)) * ((float)NUM_OF_TETRADS))+(255.f - (float)NUM_OF_TETRADS) );
}

//--------------------------------------------------------------------
// Name: RenderBlockintoBoard()
// Desc: Draws the current tetrad to the m_npBoard[]
//--------------------------------------------------------------------
void CTetris::RenderBlockintoBoard()
{
	int nPos = 0;
	for( int i=0; i<BLOCKS_PER_TET; i++ )
	{
		nPos = ConvertToBoardXY(m_CurrentTet.tet[i].iX, m_CurrentTet.tet[i].iY);
		if( nPos >= 0 && nPos < m_nSizeofBoard) //check to make sure the block stays with in the array
		{
			m_pBoard[ ConvertToBoardXY(m_CurrentTet.tet[i].iX, m_CurrentTet.tet[i].iY) ].nState = BLOCK_STATE_FULL;
			m_pBoard[ ConvertToBoardXY(m_CurrentTet.tet[i].iX, m_CurrentTet.tet[i].iY) ].r = m_CurrentTet.tet[i].r;
			m_pBoard[ ConvertToBoardXY(m_CurrentTet.tet[i].iX, m_CurrentTet.tet[i].iY) ].r = m_CurrentTet.tet[i].g;
			m_pBoard[ ConvertToBoardXY(m_CurrentTet.tet[i].iX, m_CurrentTet.tet[i].iY) ].r = m_CurrentTet.tet[i].b;
		}
	}
}

//--------------------------------------------------------------------
// Name: ClearOldBlockFromBoard()
// Desc: Clears the old block from the board so it can be repositioned
//--------------------------------------------------------------------
void CTetris::ClearOldBlockFromBoard( )
{
	int nPos = 0;
	for( int i=0; i<BLOCKS_PER_TET; i++ )
	{
		nPos = ConvertToBoardXY(m_CurrentTet.tet[i].iX, m_CurrentTet.tet[i].iY);
		if( nPos >= 0 && nPos < m_nSizeofBoard) //check to make sure the block stays with in the array
		{
			m_pBoard[ ConvertToBoardXY( m_CurrentTet.tet[i].iX, m_CurrentTet.tet[i].iY ) ].nState = BLOCK_STATE_EMPTY;
			m_pBoard[ ConvertToBoardXY(m_CurrentTet.tet[i].iX, m_CurrentTet.tet[i].iY) ].r = 0;
			m_pBoard[ ConvertToBoardXY(m_CurrentTet.tet[i].iX, m_CurrentTet.tet[i].iY) ].r = 0;
			m_pBoard[ ConvertToBoardXY(m_CurrentTet.tet[i].iX, m_CurrentTet.tet[i].iY) ].r = 0;
		}
	}
}

//--------------------------------------------------------------------
// Name: MoveBlock()
// Desc: Checks to see if the current tetrad can be moved down
//--------------------------------------------------------------------
bool CTetris::MoveTetrad( )
{
	Block temp(0,0);
	int nCurrentPos = 0;

	for(int i = 0; i<BLOCKS_PER_TET; i++)
	{//loop through each block
		nCurrentPos = m_CurrentTet.tet[i].iX+((m_CurrentTet.tet[i].iY+1)*m_nNumofCols);//CONVERTBOARDPOS(m_CurrentTet.tet[i].iX, m_CurrentTet.tet[i].iY+1);
		if( m_pBoard[ nCurrentPos ].nState == BLOCK_STATE_FULL || m_CurrentTet.tet[i].iY+1 >= m_nNumofRows ) //check to see if a collision will happen if block is moved down
			return false;//collision will occur so dont move
	}
	//we've gone through all the blocks with no collision, so move the block down
	TranslateCurrTet(m_CurrentTet.iX, m_CurrentTet.iY+1);
	
	return true;
}

//-----------------------------------------------------------------
// Name: Collision()
// Desc: Checks for collisions with any other block on the board
//       and checks for collision with the bounding region. 
//       returns false if block can't move. True if it can
//-----------------------------------------------------------------
bool CTetris::Collision(int x, int y)
{
	if( m_pBoard[ ConvertToBoardXY(x,y) ].nState == BLOCK_STATE_FULL || y >= m_nNumofRows || x >= m_nNumofCols || x < 0 )
			return false;//just check for this cause it's not rendered yet

	return true;
}

//-----------------------------------------------------------------
// Name: ProcessInput()
// Desc: Handles Keyboard presses
//-----------------------------------------------------------------
void CTetris::ProcessInput( )
{
	if( KEY_DOWN(VK_LEFT) )
	{
		for(int i=0; i<BLOCKS_PER_TET; i++)
			if( !Collision(m_CurrentTet.tet[i].iX - 1, m_CurrentTet.tet[i].iY) )
				return;
		TranslateCurrTet(m_CurrentTet.iX - 1, m_CurrentTet.iY);
		//m_Replay.KeyStrokeLogger( VK_LEFT, m_nCurrentTetName );
	}
	if( KEY_DOWN(VK_RIGHT) )
	{
		for(int i=0; i<BLOCKS_PER_TET; i++)
			if( !Collision( m_CurrentTet.tet[i].iX + 1, m_CurrentTet.tet[i].iY ) )
				return;
		TranslateCurrTet(m_CurrentTet.iX + 1, m_CurrentTet.iY);
	}

	if( KEY_DOWN(VK_DOWN) )
		m_fCurrFallDelay = FAST_FALL;
	if( KEY_UP(VK_DOWN) )
		m_fCurrFallDelay = m_fFallDelay[m_nLevel];
	
	if( KEY_DOWN(VK_ESCAPE) )
	{
		if( !m_bKeyState[VK_ESCAPE] )
			m_bExit = true;
		m_bKeyState[VK_ESCAPE] = true;
	}
	if( KEY_UP(VK_ESCAPE) )
		m_bKeyState[VK_ESCAPE] = false;

	if( KEY_DOWN(VK_LSHIFT) )
	{
		if( !m_bKeyState[VK_LSHIFT] )
			RotateCurrTet(CW);
		m_bKeyState[VK_LSHIFT] = true;
	}
	if( KEY_UP(VK_LSHIFT) )
	{
		m_bKeyState[VK_LSHIFT] = false;
	}
	
	if( KEY_DOWN(VK_UP) )
	{
		if( !m_bKeyState[VK_UP] )
			RotateCurrTet(CW);
		m_bKeyState[VK_UP] = true;
	}
	if( KEY_UP(VK_UP) )
	{
		m_bKeyState[VK_UP] = false;
	}

	if( KEY_DOWN(VK_LCONTROL) )
	{
		if( !m_bKeyState[VK_LCONTROL] )
			RotateCurrTet(CCW);
		m_bKeyState[VK_LCONTROL] = true;
	}
	if( KEY_UP(VK_LCONTROL) )
		m_bKeyState[VK_LCONTROL] = false;
	
	if(KEY_DOWN(VK_RETURN))
	{
		if( !m_bKeyState[VK_RETURN] )
			m_bPause = !m_bPause;
		m_bKeyState[VK_RETURN] = true;
	}
	if( KEY_UP(VK_RETURN) )
		m_bKeyState[VK_RETURN] = false;
}

//-----------------------------------------------------------------
// Name: RandomTetrad()
// Desc: returns a random tetrad from the array of tetrads
//-----------------------------------------------------------------
int CTetris::RandomTetrad( )
{
	int iTetrad = 0;
	static int ProbabilityArray[108] = {0,1,2,3,4,5,6,0,1,2,3,4,5,6,0,1,2,3,4,5,6,0,1,2,3,4,5,6,0,1,2,3,4,5,6,0,1,2,3,4,5,6,0,1,2,3,4,5,6,0,1,2,3,4,5,6,0,1,2,3,4,5,6,0,1,2,3,4,5,6,0,1,2,3,4,5,6,0,1,2,3,4,5,6,0,1,2,3,4,5,6};
	static int count = 0;
	
	count++;
	if( count >= SEED_COUNT )
	{
		count = 0;
		srand((unsigned)time(NULL));
	}
	iTetrad = (int)((((float)rand())/((float)RAND_MAX)) * (float)(99));
	
	return ProbabilityArray[iTetrad];
}

//-----------------------------------------------------------------
// Name: RotateTet()
// Desc: Rotates the tetrad by 90 degrees CCW or CW
//-----------------------------------------------------------------
void CTetris::RotateCurrTet( int angle )
{//Have to take the current position move it to the origan, rotate, then move it back
	int i=0, tempX = 0, tempY = 0,
		nOrgX = m_CurrentTet.iX, nOrgY = m_CurrentTet.iY;
	Tetrad temp(m_CurrentTet);

	//x'= xcos(angle) - ysin(angle);
	//y'= xsin(angle) + ycos(angle);
	if( m_nCurrentTetName == SALLY ) 
		return;
	//pre do the rotation then check bounds and if it works store in currentTet
	if( angle == CW )//
	{
		TranslateTet(temp,0,0);
		for(i=0; i<BLOCKS_PER_TET; i++)
		{
			tempX = 0*temp.tet[i].iX + -1*(temp.tet[i].iY);
			tempY = 1*temp.tet[i].iX + 0*(temp.tet[i].iY);
			temp.tet[i].iX = tempX;
			temp.tet[i].iY = tempY;
		}
		TranslateTet(temp,nOrgX,nOrgY);
		for(i=0; i<BLOCKS_PER_TET; i++)
			if( !Collision(temp.tet[i].iX, temp.tet[i].iY) ) 
				return;
		m_CurrentTet = temp;
	}
	else if( angle == CCW )
	{
		TranslateTet(temp,0,0);
		for(i=0; i<BLOCKS_PER_TET; i++)
		{
			tempX = 0*temp.tet[i].iX + 1*temp.tet[i].iY;
			tempY = (-1*temp.tet[i].iX + 0*temp.tet[i].iY);
			temp.tet[i].iX = tempX;
			temp.tet[i].iY = tempY;
		}
		TranslateTet(temp,nOrgX,nOrgY);
		for(i=0; i<BLOCKS_PER_TET; i++)
			if( !Collision(temp.tet[i].iX, temp.tet[i].iY) ) 
				return;
		m_CurrentTet = temp;
	}
}

//-----------------------------------------------------------------
// Name: Translate()
// Desc: Translates the m_CurrentTetrad to a give x,y position
//-----------------------------------------------------------------
void CTetris::TranslateCurrTet(int x, int y)
{
	int amountX = x - m_CurrentTet.iX,
		amountY = y - m_CurrentTet.iY;

	for(int i=0; i<BLOCKS_PER_TET; i++)
	{
		m_CurrentTet.tet[i].iX += amountX;
		m_CurrentTet.tet[i].iY += amountY;
	}

	m_CurrentTet.iX = x;
	m_CurrentTet.iY = y;
}

//-----------------------------------------------------------------
// Name: Translate()
// Desc: Translates the any tetrad to a give x,y position
//-----------------------------------------------------------------
void CTetris::TranslateTet( Tetrad &t, int x, int y )
{
	int amountX = x - t.iX,
		amountY = y - t.iY;

	for(int i=0; i<BLOCKS_PER_TET; i++)
	{
		t.tet[i].iX += amountX;
		t.tet[i].iY += amountY;
	}

	t.iX = x;
	t.iY = y;
}