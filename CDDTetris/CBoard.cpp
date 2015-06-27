//--------------------------------------------------------
// File: CBoard.cpp
//
// Desc: Impl. of the CBoard class
//--------------------------------------------------------

#include "CTetris.h"

//-----------------------------------
// CBoard class Impl
//-----------------------------------

//------------------------------------------------------
// Name: RenderBoard()
// Desc: renders a board to a ddraw surface 
//       drawGrid: says whether to draw in a grid
//------------------------------------------------------
int CBoard::RenderBoard( LPDIRECTDRAWSURFACE7 lpddsSurface )
{
	static HDC hDC = NULL;
	HPEN   hrectPen   = CreatePen(PS_SOLID,NULL,RGB(m_boarderColor.r,m_boarderColor.g,m_boarderColor.b));
	HPEN   hgridPen = CreatePen(PS_SOLID,NULL,RGB(m_gridColor.r,m_gridColor.g,m_gridColor.b));
	HPEN   hTempPen = NULL;
	HBRUSH hTempBrush = NULL;
	//static HBRUSH hrectBrush = CreateSolidBrush(RGB(m_bkColor.r,m_bkColor.g,m_bkColor.b));
		
	for( int nIndex=0; nIndex<getBoardSize(); nIndex++ )
		m_pBoard[nIndex].RenderBlock( lpddsSurface );
	
	lpddsSurface->GetDC(&hDC);
		
		if( m_bDrawGrid )
		{
			hTempPen = (HPEN)SelectObject(hDC, hgridPen);
			DrawGrid( hDC, m_boarderRectPlusOffset );
		}
		hTempBrush = (HBRUSH)SelectObject(hDC, GetStockObject(HOLLOW_BRUSH));
		SelectObject(hDC, hrectPen);
		Rectangle(hDC,m_boarderRectPlusOffset.left, m_boarderRectPlusOffset.top, m_boarderRectPlusOffset.right,m_boarderRectPlusOffset.bottom);
		Rectangle(hDC,m_nextBlockRect.left, m_nextBlockRect.top, m_nextBlockRect.right, m_nextBlockRect.bottom);
		DeleteObject(hrectPen);
		DeleteObject(hgridPen);

		SelectObject(hDC, hTempPen);
		SelectObject(hDC, hTempBrush);
	lpddsSurface->ReleaseDC(hDC);
		
	//Draw the blocks
	return 1;
}

//-----------------------------------------------------------------
// Name: InitBoard()
// Desc: Inits the board vars.
//          any parameter set to null, 0, or -1 will use the params
//          set at creation or whatever they were before.
//       To set a new pBoard u have to send in the cols and rows
//          for the new board
//-----------------------------------------------------------------
void CBoard::InitBoard(  RECT *rcBoarder, 
						 int offsetX, int offsetY, int cols, int rows,
						 Block *pBoard, 
					     SCCOLOR *gridColor, SCCOLOR *bkColor, SCCOLOR *boarderColor )
{
	if( offsetX != -1 )
		m_nOffsetX = offsetX;
	if( offsetY != -1 ) 
		m_nOffsetY = offsetY+GetSystemMetrics(SM_CYMENU);
	if( rows != 0 )
		setRows( rows );
	if( cols != 0 )
		setCols( cols );

	if( pBoard != NULL && cols && rows )
		m_pBoard = pBoard;
	
	//colors
	if( bkColor != NULL )
	{ m_bkColor.r = bkColor->r; m_bkColor.g = bkColor->g; m_bkColor.b = bkColor->b; }
	if( gridColor != NULL )
	{ m_gridColor.r = gridColor->r; m_gridColor.g = gridColor->g; m_gridColor.b = gridColor->b; }
	if( boarderColor != NULL )
	{ m_boarderColor.r = boarderColor->r; m_boarderColor.g = boarderColor->g; m_boarderColor.b = boarderColor->b; }
	
	if( rcBoarder != NULL )
	{ m_boarderRect.top = rcBoarder->top; m_boarderRect.left = rcBoarder->left; m_boarderRect.bottom = rcBoarder->bottom; m_boarderRect.right = rcBoarder->right; }
	
	m_boarderRectPlusOffset.left  = m_boarderRect.left  + m_nOffsetX; m_boarderRectPlusOffset.top = m_boarderRect.top + m_nOffsetY;
	m_boarderRectPlusOffset.right = m_boarderRect.right + m_nOffsetX; m_boarderRectPlusOffset.bottom = m_boarderRect.bottom + m_nOffsetY;
	
	m_nextBlockRect.top    = m_boarderRectPlusOffset.bottom - ((float)m_boarderRect.bottom/(float)getRows())*6;
	m_nextBlockRect.left   = m_boarderRectPlusOffset.right  + ((float)m_boarderRect.right/(float)getCols());
	m_nextBlockRect.bottom = m_boarderRectPlusOffset.bottom;
	m_nextBlockRect.right  = m_boarderRectPlusOffset.right  + ((float)m_boarderRect.right/(float)getCols())*6;
	
	getBlockSize(m_rectBlockSize);

	Block *blockArray = getBoard(); //alias the pointer
	//loop through each block and init it
	for( int Rowy=0; Rowy<getRows(); Rowy++ )
	{
		for( int Colx=0; Colx<getCols(); Colx++ )
		{
			int nPosition = ConvertToBoardXY(Colx,Rowy);
			
			blockArray[nPosition].nX       = ConvertBoardUnitsColtoX(Colx);
			blockArray[nPosition].nY       = ConvertBoardUnitsRowtoY(Rowy);
			blockArray[nPosition].nState   = BLOCK_STATE_EMPTY;
			blockArray[nPosition].pTexture = NULL;
			blockArray[nPosition].color    = SCCOLOR(0,0,0);
			blockArray[nPosition].rectBlockSize = m_rectBlockSize;
		}
	}

	for( int nIndex=0; nIndex<NUM_OF_TETRADS; nIndex++ )
		for( int k=0; k<BLOCKS_PER_TET; k++ )
		{
			CTetris::getTetrads()[nIndex].tet[k].rectBlockSize = m_rectBlockSize;
		}
}

//------------------------------------------------------------
// Name: DrawGrid()
// Desc:
//------------------------------------------------------------
void CBoard::DrawGrid( HDC hDC, RECT &rec  )
{
	float iIncrementY = (float)m_boarderRect.bottom/(float)getRows(),
		  iIncrementX = (float)m_boarderRect.right/(float)getCols(), temp = 0.f;
	
	for(int i=0; i<getRows(); i++)
	{
		//draw horazontal lines
		MoveToEx(hDC, rec.left, rec.top + temp, NULL); 
		LineTo(hDC, rec.right, rec.top + temp); 
		temp += iIncrementY;
	}
	temp=0.f;
	for( i=0; i<getCols(); i++ )
	{	
		//draw vertical lines
		MoveToEx(hDC, rec.left + temp, rec.top, NULL); 
		LineTo  (hDC, rec.left + temp, rec.bottom); 
		temp += iIncrementX;
	}

}

//------------------------------------------------------
// Name: ConvertXtoBoardUnitsX()
// Desc:  
//------------------------------------------------------
int CBoard::ConvertXtoBoardUnitsX( int x )
{
	return (int)( ( ( (float)m_nCols/(float)m_boarderRect.right  )+ m_nOffsetX ) * (float)(x) );
}//FIELDX = offsetX GetClientX() = m_boarderRect.right

//------------------------------------------------------
// Name: ConvertYtoBoardUnitsY()
// Desc:  
//------------------------------------------------------
int CBoard::ConvertYtoBoardUnitsY( int y )
{
	return (int)( ( ( (float)m_nRows/(float)m_boarderRect.bottom ) + m_nOffsetY )* (float)(y) );
}

//------------------------------------------------------
// Name: ConvertBoardUnitsXtoX()
// Desc:  
//------------------------------------------------------
int CBoard::ConvertBoardUnitsColtoX( int x )
{
	return (int)( ( ( (float)m_boarderRect.right/(float)m_nCols ) * (float)x ) + m_nOffsetX );
}

//------------------------------------------------------
// Name: ConvertBoardUnitsYtoY()
// Desc:  
//------------------------------------------------------
int CBoard::ConvertBoardUnitsRowtoY( int y )
{
	return (int)( ( ( (float)m_boarderRect.bottom/(float)m_nRows ) * (float)y ) +  m_nOffsetY );
}

//------------------------------------------------------
// Name: BlockSize()
// Desc:  
//------------------------------------------------------
void CBoard::getBlockSize( RECT & rec)
{
	int nHeight = (m_boarderRect.bottom-m_boarderRect.top)+1;
	int nWidth  = (m_boarderRect.right-m_boarderRect.left)+1;

	rec.top  = 0; //
	rec.left = 0; //because numbers start at 0
	rec.bottom = (int)((float)nHeight/(float)m_nRows);
	rec.right  = (int)((float)nWidth/(float)m_nCols);
}

//------------------------------------------------------
// Name: setBoarderRect()
// Desc: Sets the boarder rect so as each square with in
//       the rectangle stays a square. So it will adjust
//       the width and height sent in acordingly
//------------------------------------------------------
void CBoard::setBoarderRect( int width, int height )
{
	m_boarderRect.right = width - (width%m_nCols);
	m_boarderRect.bottom = height - (height%m_nRows);
	
	if( abs((m_boarderRect.right/m_nCols) - (m_boarderRect.bottom/m_nRows)) != 0 )
	{
		m_boarderRect.right += ((m_boarderRect.bottom/m_nRows)-(m_boarderRect.right/m_nCols))*m_nCols;
	}

	m_boarderRect.left = m_boarderRect.top = 0;
}