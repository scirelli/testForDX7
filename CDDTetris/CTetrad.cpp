//--------------------------------------------------------
// File: CTetrad.cpp
//
// Desc: Impl. of the CTetrad class
//--------------------------------------------------------

#include "CTetris.h"

//--------------------------------------------------------------------
// Name: Tetrad()
// Desc: Constructor
//--------------------------------------------------------------------
Tetrad::Tetrad()
{
	for(int i=0; i<BLOCKS_PER_TET; i++)
	{ tet[i].nX = 0; tet[i].nY = 0; tet[i].color.r = tet[i].color.g = tet[i].color.r = 0;tet[i].nState = BLOCK_STATE_EMPTY; }
	nX = 0;
	nY = 0;
	pTetradsTexture = NULL;
}

//--------------------------------------------------------------------
// Name: Tetrad()
// Desc: Constructor
//--------------------------------------------------------------------
Tetrad::Tetrad(Tetrad &c)
{
	for(int i=0; i<BLOCKS_PER_TET; i++)
	{ 
		tet[i].nX = c.tet[i].nX; 
		tet[i].nY = c.tet[i].nY;
		tet[i].color = c.tet[i].color;
		tet[i].nState  = c.tet[i].nState;
		//tet[i].pTexture= c.tet[i].pTexture;
		tet[i].rectBlockSize = c.tet[i].rectBlockSize;
	}
	nX = c.nX;
	nY = c.nY;
	pTetradsTexture = c.pTetradsTexture;
}

//--------------------------------------------------------------------
// Name: Tetrad()
// Desc: Constructor
//--------------------------------------------------------------------
Tetrad::Tetrad( Block &a, Block &b, Block &c, Block &d )
{
	tet[0] = a;
	tet[1] = b;
	tet[2] = c;
	tet[3] = d;
	nX = nY = 0;
	pTetradsTexture = NULL;
	for(int i=0; i<BLOCKS_PER_TET; i++)
	{
		tet[i].color.r=0; tet[i].color.g=0; tet[i].color.b=0;
	}
}

//--------------------------------------------------------------------
// Name: Tetrad()
// Desc: Constructor
//--------------------------------------------------------------------
Tetrad::Tetrad(SCCOLOR& col) 
{  
	for(int i=0; i<BLOCKS_PER_TET; i++)
	{
		tet[i].color.r=col.r; tet[i].color.g=col.b; tet[i].color.b=col.g;
	    tet[i].nX = 0; tet[i].nY = 0; tet[i].nState = BLOCK_STATE_EMPTY; 
	}
	nX = 0;
	nY = 0;
	pTetradsTexture = NULL;
}

//--------------------------------------------------------------------
// Name: Tetrad()
// Desc: Constructor
//--------------------------------------------------------------------
Tetrad::Tetrad(CSPRITE &textture) 
{  
	for(int i=0; i<BLOCKS_PER_TET; i++)
	{
		tet[i].color.r = tet[i].color.g = tet[i].color.b = 0;
	    tet[i].nX = 0; tet[i].nY = 0; tet[i].nState = BLOCK_STATE_EMPTY; 
	}
	nX = 0;
	nY = 0;
	pTetradsTexture = &textture;
}