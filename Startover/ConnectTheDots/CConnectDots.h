//---------------------------------------------------------------------
// File: CConnectDots.h
//
// Desc: Definition of a class for DirectDraw setup
//---------------------------------------------------------------------

#ifndef CConnectDots_h
#define CConnectDots_h

////INCLUDES////////////////////////////////////////
#include "CDDGameApp.h"
#include <vector>

using namespace std;

//-----------------------------------------------------------------------------
// Forward Declarations
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Main Class Declarations
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// Name : (Class)
// Desc : 
//-----------------------------------------------------------------------------
class CConnectDots : public CDDGameApp
{
public:
	struct Point
	{
		Point( ) : x(0), y(0)
		{}
		Point( int _x, int _y )
		{
			x = _x; y=_y;
		}
		int x,y;
	};
	CConnectDots() : CDDGameApp()
	{
		m_vecPoints.reserve( 100 );

		//m_strFileName = NULL;
		m_nRMouseButton=m_nLMouseButton=m_nXMouse=m_nYMouse = 0;
	}
	~CConnectDots()
	{
		//if( m_strFileName ) delete [] m_strFileName;
		//m_strFileName = NULL;
	}

	LRESULT DisplayWndProc( HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam );

private:
	bool BuildSprites( );
	void FrameAdvance( );
	void DrawVector( );

	char m_strFileName[100];
	int  m_nRMouseButton, 
		 m_nLMouseButton, 
		 m_nXMouse, 
		 m_nYMouse;
	vector< Point > m_vecPoints;

	//LPDDSD ddsd;

};


#endif