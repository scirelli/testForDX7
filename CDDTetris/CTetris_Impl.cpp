//--------------------------------------------------------
// File: CTetris.cpp
//
// Desc: Impl. of the CTetris class
//--------------------------------------------------------

////INCLUDES///////////
#include "CTetris.h"
#include "DynamicSpaceField.h"

//---------------------------------------------------------------------------------------------------------------
// CTetris class Impl
//---------------------------------------------------------------------------------------------------------------
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
/*
//------------------------------------------------------
// Name: CTetris()
// Desc: Constructor. 
//------------------------------------------------------
CTetris::CTetris( ) : CDDGameApp( )
{
	srand( (unsigned)time(NULL) );

	m_nLevel         			= 1;
	m_nAdvanceNextLvl			= ADVANCE_NEXT_LVL;
	m_bExit          			= false;
	m_bPause         			= false;
	m_nRowsCompleted 			= 0;
	m_nScore         			= 0;
	m_nNextTetName   			= RandomTetrad();
	m_fFallDelay[0]  			= FAST_FALL;
	m_fTotalFallTime			= 0.f;
	for( int i=1; i<NUM_OF_LVLS; i++ )
		m_fFallDelay[i] = 1.f-((float)i/10.f);///(float)i; //in seconds
	m_fCurrFallDelay  = m_fFallDelay[m_nLevel];
	m_nTotalRowsCompleted       = 0;
	m_gameState                 = INTRO;
	m_playGameState             = CONT_PLAY;
	m_nRowsCompletedforClearing = 0;
	m_nArbitraryAnimCounter     = 0;

	//fill in board class
	m_Board.setBkColor( SCCOLOR(0,0,0) );
	m_Board.setBoarderColor( SCCOLOR(255,255,255) );
	m_Board.setBoardSize( NUM_OF_COL, NUM_OF_ROWS );
	m_Board.setGridColor( SCCOLOR(60,60,60) );
	m_Board.setBoarderRect(BOARD_WIDTH,BOARD_HEIGHT);
	
	//get the highscore
	LoadHighScore();
	m_ThinkScore = 0;
}
*/
//------------------------------------------------------
// Name: CTetris()
// Desc: Constructor. 
//------------------------------------------------------
CTetris::CTetris( int cols, int rows) : CDDGameApp( )
{
	srand((unsigned)time(NULL));

	m_nLevel                    = 1;
	m_nAdvanceNextLvl           = ADVANCE_NEXT_LVL;
	m_bExit                     = false;
	m_bPause                    = false;
	m_nRowsCompleted            = 0;
	m_nScore                    = 0;
	m_nNextTetName              = RandomTetrad();
	m_fTotalFallTime			= 0.f;
	m_fTotalAnimaTime			= 0.f;
	for( int i=1; i<NUM_OF_LVLS; i++ )
		m_fFallDelay[i] = 1.f-((float)i/10.f);///(float)i; //in seconds
	m_fCurrFallDelay            = m_fFallDelay[m_nLevel];
	m_fFallDelay[0]             = FAST_FALL;
	m_nTotalRowsCompleted       = 0;
	m_gameState                 = INTRO;
	m_playGameState             = CONT_PLAY;
	m_nRowsCompletedforClearing = 0;
	m_nArbitraryAnimCounter     = 0;

	//fill in board class
	m_Board.setBkColor( SCCOLOR(0,0,0) );
	m_Board.setBoarderColor( SCCOLOR(255,255,255) );
	m_Board.setBoardSize( cols, rows );
	m_Board.setGridColor( SCCOLOR(60,60,60) );
	m_Board.setBoarderRect(BOARD_WIDTH,BOARD_HEIGHT);
	m_Board.InitBoard();
	
	//get the highscore
	LoadHighScore();
	m_ThinkScore = 0;
}

//------------------------------------------------------
// Name: ~CTetris()
// Desc: destructor. 
//------------------------------------------------------
CTetris::~CTetris ( ) 
{
	SaveHighScore();
	CDDGameApp::~CDDGameApp(); 
}

//-----------------------------------------------------------------------------
// Name : InitInstance ()
// Desc : Initialises the entire Engine here.
//-----------------------------------------------------------------------------
bool CTetris::InitInstance( HANDLE hInstance, LPCTSTR lpCmdLine, int iCmdShow )
{
    // Create the primary display device
    if (!CreateDisplay()) { ShutDown(); return false; }

    // Build Objects
    if (!BuildSprites()) { ShutDown(); return false; }

	//Set the textures for each static tetrad
	InitTetradTextures( );
    
	// Set up all required game states
    //SetupGameState();

    // Setup our rendering environment
    //SetupRenderStates();

    // Success!
	return true;
}

//-----------------------------------------------------------------------------
// Name: BiuldSprites()
// Desc: This is where you do all Initialization of the sprites. 
//       animations can be stored in m_nppAnimationList
// Example:
//		Sprites[2].InitSprite(23,19,32,16,2,120);
//		Sprites[2].SetAnimationIndex(0);
//		Sprites[2].SetAnimations(array,1,SPRITE_ANIM_ONCE,0);
//		Sprites[2].LoadSpriteImage24("./bitmaps\\Blaze16.bmp");
//		Sprites[2].SetState(SPRITE_STATE_ALIVE);
//		Sprites[2].SetXPos(100);
//		Sprites[2].SetYPos(100);
//------------------------------------------------------------------------------
bool CTetris::BuildSprites( )
{
	//Make sure nothing is created already if so release it all
	ReleaseSprites();
	//Create the space
	m_nppAnimationList = new int*[MAX_SPRITE_ANIMATIONS];
	for( int i=0; i<MAX_SPRITE_ANIMATIONS; i++ ) m_nppAnimationList[i] = NULL;
	m_pSprites = new CSPRITE[MAX_SPRITES];
	//Update the sprites utility classes
	UpdateUtilitiesClasses();
	
	//Set the animation arrays
	//blocks
	m_nppAnimationList[0] = new int[2]; m_nppAnimationList[0][0] = 0; m_nppAnimationList[0][1] = 0;
	//Empty Block
	m_nppAnimationList[1] = new int[2]; m_nppAnimationList[1][0] = 0; m_nppAnimationList[1][1] = 1;
	//My Face
	m_nppAnimationList[2] = new int[8]; m_nppAnimationList[2][0] = 0; m_nppAnimationList[2][1] = 1;
										m_nppAnimationList[2][2] = 2; m_nppAnimationList[2][3] = 3;
										m_nppAnimationList[2][4] = 4; m_nppAnimationList[2][5] = 5;
										m_nppAnimationList[2][6] = 6; m_nppAnimationList[2][7] = 7;

	//Fill in all the sprites for blocks
/*
	m_nppAnimationList[0] = new int[2]; m_nppAnimationList[0][0] = 0; m_nppAnimationList[0][1] = 8;
	m_pSprites[0].InitSprite(24,24,32,8,4,130);
	m_pSprites[0].SetAnimations(m_nppAnimationList[0],2,SPRITE_ANIM_CONT); 
	m_pSprites[0].SetCurrentAnim(0);
	m_pSprites[0].LoadSpriteImage("./bitmaps\\Blaze16b.bmp");
	m_pSprites[0].SetState(SPRITE_STATE_ALIVE);
*/
   	//Block 1
	m_pSprites[0].InitSprite(52,52,2,2,1,0);
	m_pSprites[0].SetAnimations(m_nppAnimationList[0],1,SPRITE_ANIM_ONCE);
	m_pSprites[0].SetAnimations(m_nppAnimationList[1],2,SPRITE_ANIM_CONT);
	m_pSprites[0].SetCurrentAnim(0);
	if( FAILED(m_pSprites[0].LoadSpriteImage("./bitmaps\\Brick1.bmp")))
		MessageBox(NULL,"FAILED","FIRED",NULL);
	m_pSprites[0].SetState(SPRITE_STATE_ALIVE);
/*
	m_pSprites[1].InitSprite(52,52,1,1,1,0);
	m_pSprites[1].SetAnimationIndex(0);
	m_pSprites[1].SetAnimations(m_nppAnimationList[0],1,SPRITE_ANIM_ONCE,0);
	m_pSprites[1].LoadSpriteImage("./bitmaps\\Brick2.bmp");
	m_pSprites[1].SetState(SPRITE_STATE_ALIVE);
*/
	//My Face
	m_pSprites[1].InitSprite(64,64,8,8,1,120);
	m_pSprites[1].SetAnimations(m_nppAnimationList[2],8,SPRITE_ANIM_CONT); 
	m_pSprites[1].SetCurrentAnim(0);
	m_pSprites[1].LoadSpriteImage("./bitmaps\\myfaceRotate.bmp");
	m_pSprites[1].SetState(SPRITE_STATE_ALIVE);

	m_pSprites[2].InitSprite(52,52,2,2,1,0);
	m_pSprites[2].SetAnimations(m_nppAnimationList[0],1,SPRITE_ANIM_ONCE);
	m_pSprites[2].SetCurrentAnim(0);
	m_pSprites[2].LoadSpriteImage("./bitmaps\\Brick3.bmp");
	m_pSprites[2].SetState(SPRITE_STATE_ALIVE);

	m_pSprites[3].InitSprite(52,52,2,2,1,0);
	m_pSprites[3].SetAnimations(m_nppAnimationList[0],1,SPRITE_ANIM_ONCE);
	m_pSprites[3].SetCurrentAnim(0);
	m_pSprites[3].LoadSpriteImage("./bitmaps\\Brick4.bmp");
	m_pSprites[3].SetState(SPRITE_STATE_ALIVE);

	m_pSprites[4].InitSprite(52,52,2,2,1,0);
	m_pSprites[4].SetAnimations(m_nppAnimationList[0],1,SPRITE_ANIM_ONCE);
	m_pSprites[4].SetCurrentAnim(0);
	m_pSprites[4].LoadSpriteImage("./bitmaps\\Brick5.bmp");
	m_pSprites[4].SetState(SPRITE_STATE_ALIVE);

	m_pSprites[5].InitSprite(52,52,2,2,1,0);
	m_pSprites[5].SetAnimations(m_nppAnimationList[0],1,SPRITE_ANIM_ONCE);
	m_pSprites[5].SetCurrentAnim(0);
	m_pSprites[5].LoadSpriteImage("./bitmaps\\Brick6.bmp");
	m_pSprites[5].SetState(SPRITE_STATE_ALIVE);

	m_pSprites[6].InitSprite(52,52,2,2,1,0);
	m_pSprites[6].SetAnimations(m_nppAnimationList[0],1,SPRITE_ANIM_ONCE);
	m_pSprites[6].SetCurrentAnim(0);
	m_pSprites[6].LoadSpriteImage("./bitmaps\\Brick7.bmp");
	m_pSprites[6].SetState(SPRITE_STATE_ALIVE);

	//Paused sprite
	m_pSprites[7].InitSprite(132,32,1,1,1,0);
	m_pSprites[7].SetAnimations(m_nppAnimationList[0],1,SPRITE_ANIM_ONCE);
	m_pSprites[7].SetCurrentAnim(0);
	m_pSprites[7].LoadSpriteImage("./bitmaps\\paused.bmp");
	m_pSprites[7].SetState(SPRITE_STATE_ALIVE);

	return true;
}

//----------------------------------------------------------------------------
// Name: InitTetradTextures( )
// Desc: sets the textures of all static tetrads
//----------------------------------------------------------------------------
bool CTetris::InitTetradTextures( )
{
	for( int i=0; i<NUM_OF_TETRADS; i++ )
	{
		CTetris::m_Tetrads[i].pTetradsTexture = &m_pSprites[i];
		for( int j=0; j<BLOCKS_PER_TET; j++ )
			CTetris::m_Tetrads[j].tet[j].nState = BLOCK_STATE_FULL;
	}
	return true;
}

//-----------------------------------------------------------------------------
// Name : BeginGame ()
// Desc : Signals the beginning of the physical post-initialisation stage.
//        From here on, the game engine has control over processing.
//-----------------------------------------------------------------------------
int CTetris::BeginGame( )
{
    MSG		msg;

    // Start main loop
	while (1) 
    {
        // Did we recieve a message, or are we idling ?
		if ( PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) ) 
        {
			if (msg.message == WM_QUIT) break;
			TranslateMessage( &msg );
			DispatchMessage ( &msg );
		} 
        else 
        {
			switch( m_gameState )
			{
			case INTRO: 
				{
					m_Timer.Tick();
					m_gameState = INIT;
					break;
				}
			case INIT:
				{
					//Pick the first tetrad
					PickTetrad();

					//--INIT THE BOARD--
					m_Board.setOffset((WINDOW_WIDTH/2)-(BOARD_WIDTH/2), (WINDOW_HEIGHT/2)-(BOARD_HEIGHT/2) + GetSystemMetrics(SM_CYMENU) );
					m_Board.InitBoard();
					
					//Set the next state
					m_gameState = PLAY_GAME;
				}
				break;
			case PLAY_GAME:
				// Advance Game Frame.
				FrameAdvance();
				break;
			case CREDITS:
				m_gameState = GAME_EXIT;
				break;
			case GAME_EXIT:
				PostQuitMessage(0);
				break;
			}
		} // End If messages waiting
	
    } // Until quit message is receieved

    return 0;
}

//-----------------------------------------------------------------
// Name: FrameAdvance()
// Desc: Called to signal that we are now rendering the next frame.
//-----------------------------------------------------------------
void CTetris::FrameAdvance( )
{
	m_FrameTimer.Tick((float)FPS);
	m_FallTimer.Tick();
	m_fTotalFallTime += m_FallTimer.GetTimeElapsed();

	switch( m_playGameState )
	{
	case CONT_PLAY:
		{
			if( m_bPause ) { m_playGameState = PAUSED; break; }
			if( m_bExit )  { m_playGameState = EXIT;   break; }

			m_Utils.ClrS(m_lpddsSecondary,NULL); //clear the back buffer
			
			ClearOldBlockFromBoard( );//clears the block from the board
			
			if( m_fTotalFallTime >= m_fCurrFallDelay ) 
			{ //if it's time to fall
				m_fTotalFallTime = 0.f; //reset the timer

				if( !MoveTetrad( ) ) //meaning it can't move down
				{
					m_playGameState = ADVANCE_TETRAD;
					RenderBlockintoBoard(); //Puts block on the board
					RenderNextBlock( );
					Scoreing( true );
					PrintScoring( );
					m_Board.RenderBoard( m_lpddsSecondary );
					
					break;
				}
				else
					m_playGameState = CONT_PLAY;
			}

			ProcessInput( );        //Gets user input and orientates block
			RenderBlockintoBoard(); //Puts block on the board
			RenderNextBlock( );
			PrintScoring( );
			m_Board.RenderBoard( m_lpddsSecondary );
			
			break;
		}

	case ADVANCE_TETRAD :
		{	
			//m_Utils.ClrS(m_lpddsSecondary,NULL);

			if( CheckRowCompletion() )
				m_playGameState = ANIMATE_CLEAR;
			else
			{
				m_Utils.ClrS(m_lpddsSecondary,NULL);
				m_playGameState = (PLAY_GAME_STATE)WinLose();
				if( m_playGameState != LOSE )
					PickTetrad( ); //Picks new block and sets up possition
				RenderBlockintoBoard(); //Puts block on the board
				RenderNextBlock( );
				PrintScoring( );
				m_Board.RenderBoard( m_lpddsSecondary );
			}
			break;
		}

	case ANIMATE_CLEAR :
		{
			//Handle row completion animations
			if( !AnimateRowCompletion( ) )
			{//if animation is not completed
				m_Utils.ClrS(m_lpddsSecondary,NULL);
				m_playGameState = ANIMATE_CLEAR;
				RenderNextBlock( );
				PrintScoring( );
				m_Board.RenderBoard( m_lpddsSecondary );
				m_fTotalAnimaTime = 0;
			}
			else
			{//else it animation is completed
				m_Utils.ClrS(m_lpddsSecondary,NULL);
				for( int y=0; y<m_nRowsCompleted; y++ )
					ClearRow(m_nWhichRowCompleted[y]); //clear the row from the board
				m_nRowsCompleted=0;

				m_playGameState = (PLAY_GAME_STATE)WinLose();
				if( m_playGameState != LOSE )
						PickTetrad( ); //Picks new block and sets up possition
				RenderBlockintoBoard(); //Puts block on the board
				RenderNextBlock( );
				PrintScoring( );
				m_Board.RenderBoard( m_lpddsSecondary );
				m_fTotalAnimaTime = 0;
			}
			break;
		}

	case PAUSED:
		{
			m_Utils.ClrS(m_lpddsSecondary,NULL);
			//Display paused
			ProcessInput( ); //Gets user input and orientates block
			RenderNextBlock();
			PrintScoring( );
			m_Board.RenderBoard( m_lpddsSecondary );
			m_pSprites[7].DrawSprite( m_lpddsSecondary, (WINDOW_WIDTH/2)-(m_pSprites[7].GetWidth()/2), WINDOW_HEIGHT/2 );
			Sleep(10);
			if( !m_bPause ) { m_playGameState = CONT_PLAY; break; }
			
			break;
		}

	case WIN :
	case LOSE :
	case EXIT :
		{
			//do any animations
			m_gameState = CREDITS;
			break;
		}
	}//End switch( m_playGameState )

	if( m_DDSettings.Windowed )
		m_Utils.FlipToClient( m_lpddsSecondary );
	else
		m_Utils.Flip();
}

//--------------------------------------------------------------------
// Name: Scoreing()
// Desc: Handles scoreing and side affects of scoring
//--------------------------------------------------------------------
void CTetris::Scoreing( bool collision )
{
	if( collision && KEY_DOWN(VK_DOWN) )
		m_ThinkScore += THOUGHT_WORTH;

	m_nScore = m_ThinkScore + m_nTotalRowsCompleted*ROW_WORTH;
	
	if( m_nScore > m_nHighScore[0] )
		m_nHighScore[0] = m_nScore;
	if( m_nRowsCompletedforClearing >= m_nAdvanceNextLvl )
	{
		m_nRowsCompletedforClearing=0;
		if( m_nLevel <= NUM_OF_LVLS )
		{
			m_nLevel++;
			m_fCurrFallDelay = m_fFallDelay[m_nLevel];
		}
	}
}

//--------------------------------------------------------------------
// Name: PrintScoreing()
// Desc: Prints everything pertaining to the score. Highscore, score
//       Blocks completed.
//--------------------------------------------------------------------
void CTetris::PrintScoring( )
{//Comic Sans MS Impact 
	char highscore[100];
	
	//High score
	sprintf( highscore,"%d", m_nHighScore[0] );
	m_Utils.DrawTextGDI(m_lpddsSecondary, "Impact",HS_TEXT_SIZE,"High Score",m_Board.getBoardRcPlusOffset().right+HS_TEXT_BUFFER_X,m_Board.getBoardRcPlusOffset().top+HS_TEXT_BUFFER_Y,NULL,SCCOLOR(255,255,255));
	m_Utils.DrawTextGDI(m_lpddsSecondary, "Impact",HS_NUM_SIZE,highscore,m_Board.getBoardRcPlusOffset().right+HS_TEXT_BUFFER_X,m_Board.getBoardRcPlusOffset().top+HS_TEXT_BUFFER_Y+20,NULL,SCCOLOR(255,255,255));
	//score
	sprintf( highscore,"%d", m_nScore );
	m_Utils.DrawTextGDI(m_lpddsSecondary, "Impact",S_TEXT_SIZE,"Score",m_Board.getBoardRcPlusOffset().right+S_TEXT_BUFFER_X,m_Board.getBoardRcPlusOffset().top+S_TEXT_BUFFER_Y,NULL,SCCOLOR(255,255,255));
	m_Utils.DrawTextGDI(m_lpddsSecondary, "Impact",S_NUM_SIZE,highscore,m_Board.getBoardRcPlusOffset().right+S_TEXT_BUFFER_X,m_Board.getBoardRcPlusOffset().top+S_TEXT_BUFFER_Y+20,NULL,SCCOLOR(255,255,255));
	//Level
	sprintf( highscore,"%d", m_nLevel );
	m_Utils.DrawTextGDI(m_lpddsSecondary, "Impact",LVL_TEXT_SIZE,"Level",m_Board.getBoardRcPlusOffset().right+LVL_TEXT_BUFFER_X,m_Board.getBoardRcPlusOffset().top+LVL_TEXT_BUFFER_Y,NULL,SCCOLOR(255,255,255));
	m_Utils.DrawTextGDI(m_lpddsSecondary, "Impact",LVL_NUM_SIZE,highscore,m_Board.getBoardRcPlusOffset().right+LVL_TEXT_BUFFER_X,m_Board.getBoardRcPlusOffset().top+LVL_TEXT_BUFFER_Y+20,NULL,SCCOLOR(255,255,255));
}

//--------------------------------------------------------------------
// Name: CheckRowCompletion()
// Desc: Checks to see if any rows are complete. If they are, the row
//       is cleared, and all rows above it are moved down.
//--------------------------------------------------------------------
bool CTetris::CheckRowCompletion( )
{
	int nCount = 0;
	bool bRetVal = false;
	
	//Clear which row was completed var
	for(int nIndex=0; nIndex<BLOCKS_PER_TET; nIndex++)
		m_nWhichRowCompleted[nIndex] = 0;
	nIndex=0;//set index back to 0 for reuse

	for( int y=0; y<m_Board.getRows(); y++ )
	{
		for( int x=0; x<m_Board.getCols(); x++ )
		{   //if it's full keep track
			if( m_Board.getBoard()[ m_Board.ConvertToBoardXY(x,y) ].nState == BLOCK_STATE_FULL )
				nCount++;
		}//End for( int x=0; x<m_nNumofCols; x++ )

		if( nCount == m_Board.getCols() )
		{//the row is complete 
			//Take care of updating all row information
			m_nRowsCompleted++;
			m_nRowsCompletedforClearing++;
			m_nTotalRowsCompleted++;
			m_nWhichRowCompleted[nIndex++] = y;
			if( nIndex >= BLOCKS_PER_TET ) //cap the nIndex
				nIndex = BLOCKS_PER_TET;
			//Update scores
			Scoreing();
			
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
	for( int k=0; k<m_Board.getCols(); k++ )
	{
		m_Board.getBoard()[ m_Board.ConvertToBoardXY(k,y) ].nState = BLOCK_STATE_EMPTY;
		m_Board.getBoard()[ m_Board.ConvertToBoardXY(k,y) ].color = m_Board.getBkColor();
		m_Board.getBoard()[ m_Board.ConvertToBoardXY(k,y) ].pTexture = NULL;
	}//End for( int k=0; k<m_nNumofCols; k++ )
	//move everything down a row
	for( k=y; k>0; k-- )
	{
		for( int l=0; l<m_Board.getCols(); l++ )
		{
			m_Board.getBoard()[ m_Board.ConvertToBoardXY(l,k) ].nState   = m_Board.getBoard()[ m_Board.ConvertToBoardXY(l,k-1) ].nState;
			m_Board.getBoard()[ m_Board.ConvertToBoardXY(l,k) ].color    = m_Board.getBoard()[ m_Board.ConvertToBoardXY(l,k-1) ].color;
			m_Board.getBoard()[ m_Board.ConvertToBoardXY(l,k) ].pTexture = m_Board.getBoard()[ m_Board.ConvertToBoardXY(l,k-1) ].pTexture;
		}
	}//End for( k=y; k>0; k-- )
	return true;
}

//--------------------------------------------------------------------
// Name: AnimateRowCompletion()
// Desc: 
//--------------------------------------------------------------------
bool CTetris::AnimateRowCompletion( )
{ 
	//m_nRowsCompleted;//how many rows were completed
	//m_nWhichRowCompleted[y]; //what row was completed
	if( m_nArbitraryAnimCounter < NUM_OF_COL/2 )
	{
		for( int k=0; k<m_nRowsCompleted; k++ )
		{
			//Left to right clear
			m_Board.getBoard()[ m_Board.ConvertToBoardXY(m_nArbitraryAnimCounter,m_nWhichRowCompleted[k]) ].nState = BLOCK_STATE_EMPTY;
			//right to left clear
			m_Board.getBoard()[ m_Board.ConvertToBoardXY((NUM_OF_COL-1)-m_nArbitraryAnimCounter,m_nWhichRowCompleted[k]) ].nState = BLOCK_STATE_EMPTY;
		}
		m_nArbitraryAnimCounter++;
		return false;
	}
	
	m_nArbitraryAnimCounter = 0;
	return true;
}

//--------------------------------------------------------------------
// Name: WinLose()
// Desc: Checks to see if a win or lose as accured and returns apprope
//       value.
//--------------------------------------------------------------------
int CTetris::WinLose( )
{
	for( int x=0; x<m_Board.getCols(); x++ )
	{ int de = m_Board.getBoard()[m_Board.ConvertToBoardXY(x,0)].nState;
		if ( m_Board.getBoard()[m_Board.ConvertToBoardXY(x,0)].nState == BLOCK_STATE_FULL )
			return LOSE;
	}
	if( CTetris::m_nRowsCompleted >= WINNING_SCORE && CTetris::m_nLevel >= NUM_OF_LVLS )
		return WIN;

	return CONT_PLAY;
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
	//set color
	for(int i=0; i<BLOCKS_PER_TET; i++)
		m_CurrentTet.tet[i].color.r = m_CurrentTet.tet[i].color.r = m_CurrentTet.tet[i].color.r = (int)color;
	TranslateCurrTet(m_Board.getCols()/2,1);
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
		nPos = m_Board.ConvertToBoardXY(m_CurrentTet.tet[i].nX, m_CurrentTet.tet[i].nY);
		if( nPos >= 0 && nPos < m_Board.getBoardSize()) //check to make sure the block stays with in the array
		{
			m_Board.getBoard()[ nPos ].nState   = BLOCK_STATE_FULL;
			m_Board.getBoard()[ nPos ].color    = m_CurrentTet.tet[i].color;
			m_Board.getBoard()[ nPos ].pTexture = m_CurrentTet.pTetradsTexture;
		}
	}
}

//--------------------------------------------------------------------
// Name: RenderNextBlock()
// Desc: Renders the next block into the next block rect.
// Note to self: as a side thought maybe instead of using the actual
//               size of a block, base the size on the size of the
//               next block rect.
//--------------------------------------------------------------------
void  CTetris::RenderNextBlock()
{
	Tetrad tet = CTetris::m_Tetrads[m_nNextTetName];
	float centerX = ((float)CTetris::getTetradWidth(m_nNextTetName) * (float)m_Board.getBlockSizeRect().right )*0.5f;
	float centerY = ((float)CTetris::getTetradHeight(m_nNextTetName)* (float)m_Board.getBlockSizeRect().bottom)*0.5f;
	float leftOffset= getLeftestPos(m_nNextTetName)*(int)m_Board.getBlockSizeRect().right;
	float bottomOffset= getLowestPos(m_nNextTetName)*(int)m_Board.getBlockSizeRect().bottom;

	for( int i=0; i<BLOCKS_PER_TET; i++ )
	{
		float boardUX=0,boardUY=0,modelSpaceX0=0, modelSpaceY0=0;

		tet.tet[i].pTexture = tet.pTetradsTexture; 
		tet.tet[i].nState = BLOCK_STATE_FULL;

		boardUX = m_Board.ConvertBoardUnitsColtoX(tet.tet[i].nX)-(int)m_Board.getBoardRcPlusOffset().left+(int)m_Board.getNextBlockRect()->left;//
		boardUY = m_Board.ConvertBoardUnitsRowtoY(tet.tet[i].nY)-(int)m_Board.getBoardRcPlusOffset().top+(int)m_Board.getNextBlockRect()->top;//
		
		modelSpaceX0 = boardUX+(leftOffset)-centerX;
		modelSpaceY0 = boardUY-(bottomOffset)+centerY;
		tet.tet[i].nX = modelSpaceX0 + (float)(m_Board.getNextBlockRect()->right-m_Board.getNextBlockRect()->left)*0.5f;
		tet.tet[i].nY = modelSpaceY0 + (float)(m_Board.getNextBlockRect()->bottom-m_Board.getNextBlockRect()->top)*0.5f;
		
		tet.tet[i].RenderBlockAbsolute( m_lpddsSecondary, tet.tet[i].nX, tet.tet[i].nY );
	}
}

//-------------------------------------------------------------------
// Name: LoadHighScore()
// Desc:
//-------------------------------------------------------------------
void CTetris::LoadHighScore( char * cpFileName )
{
	ifstream infile;
	if( cpFileName == NULL )
		infile.open( "./hs.dat");
	else
		infile.open( cpFileName );

	infile >> m_nHighScore[0];

	infile.close();
}

//-------------------------------------------------------------------
// Name: SaveHighScore()
// Desc:
//-------------------------------------------------------------------
void CTetris::SaveHighScore( char *cpFileName )
{
	ofstream outfile;

	if( cpFileName == NULL )
		outfile.open( "./hs.dat");
	else
		outfile.open( cpFileName );
	
	outfile.clear();
	outfile << m_nHighScore[0] << flush;
	
	outfile.close();
}

//--------------------------------------------------------------------
// Name: getLowestPos()
// Desc: 
//--------------------------------------------------------------------
int CTetris::getLowestPos( int ntetName )
{
	int lowestY = 0,count=0;;
	Tetrad tet = CTetris::m_Tetrads[ ntetName ];

	//find highest y quard, most negative
	for( int y=0; y<BLOCKS_PER_TET; y++ )
	{
		if( tet.tet[y].nY > lowestY )
		{count++;lowestY=tet.tet[y].nY;}
	}
	return count+1;
}

//--------------------------------------------------------------------
// Name: getLeftestPos()
// Desc: gets the distance in blocks from tet[0] to left most tet
//--------------------------------------------------------------------
int CTetris::getLeftestPos( int ntetName )
{
	int lowestX = 0, pos=0;
	Tetrad tet = CTetris::m_Tetrads[ntetName];

	for( int y=0; y<BLOCKS_PER_TET; y++ )
	{
		if( tet.tet[y].nX < lowestX )
		{ lowestX=tet.tet[y].nX; pos++; }
	}
	return pos;
}

//--------------------------------------------------------------------
// Name: getTetradHeight()
// Desc: returns the height in block units
//--------------------------------------------------------------------
int CTetris::getTetradHeight( int nTetName )
{
	int highestY = 0, lowestY = 0;
	Tetrad tet = CTetris::m_Tetrads[nTetName];
	
	//find highest y quard, most negative
	for( int y=0; y<BLOCKS_PER_TET; y++ )
	{
		if( tet.tet[y].nY < highestY )
			highestY = tet.tet[y].nY;
		if( tet.tet[y].nY > lowestY )
			lowestY = tet.tet[y].nY;
	}

	return abs(lowestY-highestY)+1;
}

//--------------------------------------------------------------------
// Name: getTetradWidth()
// Desc: returns the width in block units
//--------------------------------------------------------------------
int CTetris::getTetradWidth(int nTetName )
{
	int highestX = 0, lowestX=0;
	Tetrad tet = CTetris::m_Tetrads[nTetName];

	for( int y=0; y<BLOCKS_PER_TET; y++ )
	{
		if( tet.tet[y].nX > highestX )
			highestX = tet.tet[y].nX;
		if( tet.tet[y].nX < lowestX )
			lowestX = tet.tet[y].nX;
	}
	return abs(highestX-lowestX)+1;
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
		nPos = m_Board.ConvertToBoardXY(m_CurrentTet.tet[i].nX, m_CurrentTet.tet[i].nY);
		if( nPos >= 0 && nPos < m_Board.getBoardSize()) //check to make sure the block stays with in the array
		{
			m_Board.getBoard()[ nPos ].nState   = BLOCK_STATE_EMPTY;
			m_Board.getBoard()[ nPos ].color.r  = 0;
			m_Board.getBoard()[ nPos ].color.g  = 0;
			m_Board.getBoard()[ nPos ].color.b  = 0;
			m_Board.getBoard()[ nPos ].pTexture = NULL;
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
		nCurrentPos = m_CurrentTet.tet[i].nX+((m_CurrentTet.tet[i].nY+1)*m_Board.getCols());//CONVERTBOARDPOS(m_CurrentTet.tet[i].nX, m_CurrentTet.tet[i].nY+1);
		if( m_Board.getBoard()[ nCurrentPos ].nState == BLOCK_STATE_FULL || m_CurrentTet.tet[i].nY+1 >= m_Board.getRows() ) //check to see if a collision will happen if block is moved down
			return false;//collision will occur so dont move
	}
	//we've gone through all the blocks with no collision, so move the block down
	TranslateCurrTet(m_CurrentTet.nX, m_CurrentTet.nY+1);
	
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
	if( m_Board.getBoard()[ m_Board.ConvertToBoardXY(x,y) ].nState == BLOCK_STATE_FULL || y >= m_Board.getRows() || x >= m_Board.getCols() || x < 0 )
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
			if( !Collision(m_CurrentTet.tet[i].nX - 1, m_CurrentTet.tet[i].nY) )
				return;
		TranslateCurrTet(m_CurrentTet.nX - 1, m_CurrentTet.nY);
		//m_Replay.KeyStrokeLogger( VK_LEFT, m_nCurrentTetName );
	}
	if( KEY_DOWN(VK_RIGHT) )
	{
		for(int i=0; i<BLOCKS_PER_TET; i++)
			if( !Collision( m_CurrentTet.tet[i].nX + 1, m_CurrentTet.tet[i].nY ) )
				return;
		TranslateCurrTet(m_CurrentTet.nX + 1, m_CurrentTet.nY);
	}

	if( KEY_DOWN(VK_DOWN) )
		m_fCurrFallDelay = FAST_FALL;
	if( KEY_UP(VK_DOWN) )
		m_fCurrFallDelay = m_fFallDelay[m_nLevel];
	
	if( KEY_DOWN(VK_ESCAPE) )
	{
		if( !m_pbKeyStatus[VK_ESCAPE] )
		{ m_bExit = true; m_playGameState = EXIT; }
		m_pbKeyStatus[VK_ESCAPE] = true;
	}
	if( KEY_UP(VK_ESCAPE) )
		m_pbKeyStatus[VK_ESCAPE] = false;

	if( KEY_DOWN(VK_LSHIFT) )
	{
		if( !m_pbKeyStatus[VK_LSHIFT] )
			RotateCurrTet(CW);
		m_pbKeyStatus[VK_LSHIFT] = true;
	}
	if( KEY_UP(VK_LSHIFT) )
	{
		m_pbKeyStatus[VK_LSHIFT] = false;
	}
	
	if( KEY_DOWN(VK_UP) )
	{
		if( !m_pbKeyStatus[VK_UP] )
			RotateCurrTet(CW);
		m_pbKeyStatus[VK_UP] = true;
	}
	if( KEY_UP(VK_UP) )
	{
		m_pbKeyStatus[VK_UP] = false;
	}

	if( KEY_DOWN(VK_LCONTROL) )
	{
		if( !m_pbKeyStatus[VK_LCONTROL] )
			RotateCurrTet(CCW);
		m_pbKeyStatus[VK_LCONTROL] = true;
	}
	if( KEY_UP(VK_LCONTROL) )
		m_pbKeyStatus[VK_LCONTROL] = false;
	
	if(KEY_DOWN(VK_RETURN))
	{
		if( !m_pbKeyStatus[VK_RETURN] )
			m_bPause = !m_bPause;
		m_pbKeyStatus[VK_RETURN] = true;
	}
	if( KEY_UP(VK_RETURN) )
		m_pbKeyStatus[VK_RETURN] = false;
/*
	if( m_pbKeyStatus[VK_LEFT] )
	{
		for(int i=0; i<BLOCKS_PER_TET; i++)
			if( !Collision(m_CurrentTet.tet[i].nX - 1, m_CurrentTet.tet[i].nY) )
				return;
		TranslateCurrTet(m_CurrentTet.nX - 1, m_CurrentTet.nY);
		//m_Replay.KeyStrokeLogger( VK_LEFT, m_nCurrentTetName );
	}

	if( m_pbKeyStatus[VK_RIGHT] )
	{
		for(int i=0; i<BLOCKS_PER_TET; i++)
			if( !Collision( m_CurrentTet.tet[i].nX + 1, m_CurrentTet.tet[i].nY ) )
				return;
		TranslateCurrTet(m_CurrentTet.nX + 1, m_CurrentTet.nY);
	}

	if( m_pbKeyStatus[VK_DOWN] )
	{
		m_fCurrFallDelay = FAST_FALL;
		OutputDebugString("\nVK_DOWN pressed");
	}
	else
	{
		m_fCurrFallDelay = m_fFallDelay[m_nLevel];
		OutputDebugString("\nVK_DOWN released");
	}
	
	if( m_pbKeyStatus[VK_ESCAPE] )
	{ m_bExit = true; m_playGameState = EXIT; }
	
	static bool temp[256];

	if( m_pbKeyStatus[VK_LSHIFT] )
	{
		if(!temp[VK_LSHIFT])
			RotateCurrTet(CW);
		temp[VK_LSHIFT] = true;
	}
	else
		temp[VK_LSHIFT] = false;
	
	if( m_pbKeyStatus[VK_UP] )
	{
		if(!temp[VK_UP])
			RotateCurrTet(CW);
		temp[VK_UP] = true;
	}
	else
		temp[VK_UP] = false;

	if( m_pbKeyStatus[VK_LCONTROL] )
	{
		if(!temp[VK_LCONTROL])
			RotateCurrTet(CCW);
		temp[VK_LCONTROL] = true;
	}
	else
		temp[VK_LCONTROL] = false;
			
	if( m_pbKeyStatus[VK_RETURN] )
	{
		if(!temp[VK_RETURN])
			m_bPause = !m_bPause;
		temp[VK_RETURN] = true;
	}
	else
		temp[VK_RETURN] = false;
*/
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
// Desc: Rotates the tetrad by 90 degrees CCW or CW in board space
//-----------------------------------------------------------------
void CTetris::RotateCurrTet( int angle )
{//Have to take the current position move it to the origan, rotate, then move it back
	int i=0, tempX = 0, tempY = 0,
		nOrgX = m_CurrentTet.nX, nOrgY = m_CurrentTet.nY;
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
			tempX = 0*temp.tet[i].nX + -1*(temp.tet[i].nY);
			tempY = 1*temp.tet[i].nX + 0*(temp.tet[i].nY);
			temp.tet[i].nX = tempX;
			temp.tet[i].nY = tempY;
		}
		TranslateTet(temp,nOrgX,nOrgY);
		for(i=0; i<BLOCKS_PER_TET; i++)
			if( !Collision(temp.tet[i].nX, temp.tet[i].nY) ) 
				return;
		m_CurrentTet = temp;
	}
	else if( angle == CCW )
	{
		TranslateTet(temp,0,0);
		for(i=0; i<BLOCKS_PER_TET; i++)
		{
			tempX = 0*temp.tet[i].nX + 1*temp.tet[i].nY;
			tempY = (-1*temp.tet[i].nX + 0*temp.tet[i].nY);
			temp.tet[i].nX = tempX;
			temp.tet[i].nY = tempY;
		}
		TranslateTet(temp,nOrgX,nOrgY);
		for(i=0; i<BLOCKS_PER_TET; i++)
			if( !Collision(temp.tet[i].nX, temp.tet[i].nY) ) 
				return;
		m_CurrentTet = temp;
	}
}

//-----------------------------------------------------------------
// Name: Translate()
// Desc: Translates the m_CurrentTetrad to a give x,y position
//       in board space
//-----------------------------------------------------------------
void CTetris::TranslateCurrTet(int x, int y)
{
	int amountX = x - m_CurrentTet.nX,
		amountY = y - m_CurrentTet.nY;

	for(int i=0; i<BLOCKS_PER_TET; i++)
	{
		m_CurrentTet.tet[i].nX += amountX;
		m_CurrentTet.tet[i].nY += amountY;
	}

	m_CurrentTet.nX = x;
	m_CurrentTet.nY = y;
}

//-----------------------------------------------------------------
// Name: Translate()
// Desc: Translates any tetrad to a give x,y position
//       in board space
//-----------------------------------------------------------------
void CTetris::TranslateTet( Tetrad &t, int x, int y )
{
	int amountX = x - t.nX,
		amountY = y - t.nY;

	for(int i=0; i<BLOCKS_PER_TET; i++)
	{
		t.tet[i].nX += amountX;
		t.tet[i].nY += amountY;
	}

	t.nX = x;
	t.nY = y;
}

#include <fstream.h>

//----------------------------------------------------------------
// Name: PrintBoardState( )
// Desc: Prints the boards state to a file
//----------------------------------------------------------------
void CTetris::PrintBoardState( char filename[])
{
	ofstream outfile(filename);
	
	for( int y=0; y<m_Board.getRows(); y++ )
	{
		for( int x=0; x<m_Board.getCols(); x++ )
		{
			outfile << "Row = " << y << " Col = " << x 
				    << " XPos = " << m_Board.getBoard()[m_Board.ConvertToBoardXY(x,y)].nX << " YPos = " << m_Board.getBoard()[m_Board.ConvertToBoardXY(x,y)].nY 
					<< " State = " << m_Board.getBoard()[m_Board.ConvertToBoardXY(x,y)].nState << " \t" << flush;

		}
		outfile << " || " << endl;
	}
	outfile.close();
}

//-----------------------------------------------------------------------------
// Name : DisplayWndProc ()
// Desc : The display devices internal WndProc function. All messages being
//        passed to this function are relative to the window it owns.
//-----------------------------------------------------------------------------
LRESULT CTetris::DisplayWndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
    PAINTSTRUCT	ps;		   // used in WM_PAINT
	HDC hDC = NULL;
	
	//find out what the message is
	switch(uMsg)
	{	
	
	case WM_CREATE: //called when window is created
		{// do initialization stuff here
			//get the device context of the client area
			//hDC= GetDC(hWnd);
			
			return(0);
		} break;
	case WM_PAINT:
         {
			// start painting
			hDC = BeginPaint(hWnd,&ps);

			// end painting
			EndPaint(hWnd,&ps);
			ReleaseDC(hWnd, hDC);
			return(0);
        } break;
	case WM_ACTIVATEAPP: 
		{
			m_bActiveApp = (bool)wParam;
			break;
		}

	case WM_SIZE:
		{
            if ( wParam == SIZE_MINIMIZED )
            {
                // App is inactive
                m_bActiveApp = false;
            
            } // App has been minimized
            else
            {
                // App is active
                m_bActiveApp = true;      
            } // End if !Minimized

			break;
		}
	case WM_SETFOCUS:
		{
			//for(int i = 0; i<MAX_SPRITES; i++)
			//	Sprites[i].RestoreSurface();
			break;
		}
	case WM_KEYDOWN:
		{
			//m_pbKeyStatus[wParam] = true;
		
			switch (wParam) 
            {
				case VK_ESCAPE:
					m_bExitApp = true;
					//PostQuitMessage(0);
					return 0;
				case 0x57: //W
					{//Switch to windowed mode
						DestroyWindow(m_hWnd);
						UnregisterClass(m_WindowTitle,(HINSTANCE)GetModuleHandle(NULL));
						m_hWnd = NULL;//(HINSTANCE)GetModuleHandle(NULL)
						setDisplaySettings(true);
						ShutDown();
						InitInstance((HINSTANCE)GetModuleHandle(NULL), NULL, 0);
						ShowCursor(TRUE);
						OutputDebugString( "W pressed window destroyed no one created" );
						return 0;
					}
				case 0x56: //V
					{
						return 0;
					}
			}
			break;
		}

	case WM_KEYUP:
		{
			//m_pbKeyStatus[wParam] = false;
			break;
		}

	case WM_CLOSE:
		m_bExitApp = true;
		break;
	case WM_COMMAND:
		{
			// Process Menu Items
			switch( LOWORD(wParam) )
			{
				case ID_VIDEO_FULLSCREEN:
					{// Signal that we want to go full screen
						DestroyWindow(m_hWnd);
						UnregisterClass(m_WindowTitle,(HINSTANCE)GetModuleHandle(NULL));
						m_hWnd = NULL;
						setDisplaySettings(false);
						ShutDown();
						InitInstance((HINSTANCE)GetModuleHandle(NULL), NULL, 0);
						ShowCursor(FALSE);
						OutputDebugString( "Full screen mode activated" );
						return 0;
					}
               
				case ID_FILE_EXIT:
					// Recieved key/menu command to exit app
					m_bExitApp = true;
					SendMessage( m_hWnd, WM_CLOSE, 0, 0 );
					return 0;

				case ID_GRID_ON :
					m_Board.setGridOn( true );
					return 0;

				case ID_GRID_OFF :
					m_Board.setGridOn( false );
					return 0;
			}//End switch( LOWORD(wParam) )
			break;	
		 }
	case WM_DESTROY: 
		{// kill the application			
			
			//close the program
			if( m_bExitApp ) PostQuitMessage(0);
			return(0);
		} break;

	default:break;

    } // end main switch

	// process any messages that we didn't take care of 
	return (DefWindowProc(hWnd, uMsg, wParam, lParam));
}