//-----------------------------------------------------------------------------
// File: CTestBed.cpp
//
// Desc: 
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// CGameApp Specific Includes
//-----------------------------------------------------------------------------
#include "CConnectDots.h"
//-----------------------------------------------------------------------------
// CDDGameApp Member Functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Name : () (Constructor)
// Desc : Class Constructor
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------
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
//-----------------------------------------------------------------------
bool CConnectDots::BuildSprites( )
{
	//Make sure nothing is created already if so release it all
	ReleaseSprites();
	//Create the space
	if( m_nppAnimationList ) delete [] m_nppAnimationList;
	m_nppAnimationList = new int*[MAX_SPRITE_ANIMATIONS];
	
	for( int i=0; i<MAX_SPRITE_ANIMATIONS; i++ ) m_nppAnimationList[i] = NULL;
	m_nppAnimationList[0] = new int[2]; m_nppAnimationList[0][0] = 0; m_nppAnimationList[0][1] = 0;
	
	m_pSprites = new CSPRITE[MAX_SPRITES];
	//Update the sprites utility classes
	UpdateUtilitiesClasses();
	
	m_pSprites[0].InitSprite(640,480,0,1,1,0);
	m_pSprites[0].SetAnimations( m_nppAnimationList[0],1,SPRITE_ANIM_ONCE );
	m_pSprites[0].SetCurrentAnim(0);
	if( FAILED(m_pSprites[0].LoadSpriteImage("./dots.bmp")))
		MessageBox(NULL,"FAILED","FIRED",NULL);
	m_pSprites[0].SetState(SPRITE_STATE_ALIVE);
	return true;
}

//-----------------------------------------------------------------------
// Name: FrameAdvance()
// Desc: 
//-----------------------------------------------------------------------
void CConnectDots::FrameAdvance()
{
	m_Utils.ClrS(m_lpddsSecondary,NULL);
	
	if( &m_pSprites[0] )
		m_pSprites[0].DrawSprite( m_lpddsSecondary );
	
	DrawVector( );
	Point p = m_vecPoints[ m_vecPoints.size()-1 ];
	Point p2 = Point( m_nXMouse, m_nYMouse );
	m_Utils.DrawLineGDI( m_lpddsSecondary, p.x,p.y, p2.x,p2.y, SCCOLOR(255,255,255) );

	static char buffer[100];
	//mouse x
	sprintf(buffer, "x= %d", m_nXMouse);
	m_Utils.DrawStringGDI( m_lpddsSecondary, 1,30,buffer );
	//mouse y
	sprintf(buffer, "y= %d", m_nYMouse);
	m_Utils.DrawStringGDI( m_lpddsSecondary, 1,45, buffer );

	Sleep(30);
	if(m_DDSettings.Windowed)
		m_Utils.FlipToClient( m_lpddsSecondary );
	else
		m_Utils.Flip( );
}

void CConnectDots::DrawVector( )
{
	RECT rect;
	Point p;
	for( int i=0; i<(int)m_vecPoints.size()-1; i++ )
	{
		p = m_vecPoints[i];
		Point p2 = m_vecPoints[i+1];
		m_Utils.DrawLineGDI( m_lpddsSecondary, p.x,p.y, p2.x,p2.y, SCCOLOR(255,255,255) );
		
		rect.top = p.y-2; rect.left = p.x-2;
		rect.bottom = p.y+2; rect.right=p.x+2;
		m_Utils.DrawFillRect( m_lpddsSecondary, rect, SCCOLOR( 255, 0,0 ) );
	}
	p = m_vecPoints[ m_vecPoints.size()-1 ];
	rect.top = p.y-2; rect.left = p.x-2;
	rect.bottom = p.y+2; rect.right=p.x+2;
	m_Utils.DrawFillRect( m_lpddsSecondary, rect, SCCOLOR( 255, 0,0 ) );
}

//-----------------------------------------------------------------------------
// Name : DisplayWndProc ()
// Desc : The display devices internal WndProc function. All messages being
//        passed to this function are relative to the window it owns.
//-----------------------------------------------------------------------------
LRESULT CConnectDots::DisplayWndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
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
	case WM_LBUTTONDOWN :
		{
			m_nLMouseButton = 1;
			m_vecPoints.push_back( Point(m_nXMouse, m_nYMouse) );
			break;
		}
	case WM_LBUTTONUP :
		{
			m_nLMouseButton = 0;
			break;
		}
	case WM_RBUTTONDOWN :
		{
			m_nRMouseButton = 2;
			m_vecPoints.pop_back();
			break;
		}
	case WM_RBUTTONUP :
		{
			m_nRMouseButton = 0;
			break;
		}
	case WM_MOUSEMOVE: // whenever the mouse moves this is sent
        {
			m_nXMouse = (int)LOWORD(lParam);
			m_nYMouse = (int)HIWORD(lParam);
			return(0);
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
						ShowCursor(TRUE);
						OutputDebugString( "Full screen mode activated" );
						return 0;
					}
				case 118:
					{
						OPENFILENAME ofn;
						char cpFilename[100];
						static TCHAR szFilter[] = TEXT ("Bitmap Files (*.bmp)\0*.dat\0")  \
											      TEXT ("All Files (*.*)\0*.*\0\0");
						static char cpFileName[1024];
						FILE *fFile;
						//----Fill in the struct for common Save/Load dialog box----
						ofn.lStructSize       = sizeof(OPENFILENAME);
						ofn.hwndOwner         = hWnd;
						ofn.hInstance         = NULL;
						ofn.lpstrFilter       = TEXT ("All Files (*.*)\0*.*\0\0");
						ofn.lpstrCustomFilter = NULL;
						ofn.nMaxCustFilter    = 0;
						ofn.nFilterIndex      = 0;
						ofn.lpstrFile         = NULL;
						ofn.nMaxFile          = MAX_PATH;
						ofn.lpstrFileTitle    = NULL;
						ofn.nMaxFileTitle     = MAX_PATH ;
						ofn.lpstrInitialDir   = NULL ;
						ofn.lpstrTitle        = NULL ;
						ofn.Flags             = 0 ;             // Set in Open and Close functions
						ofn.nFileOffset       = 0 ;
						ofn.nFileExtension    = 0 ;
						ofn.lpstrDefExt       = TEXT (".bmp") ;
						ofn.lCustData         = 0L ;
						ofn.lpfnHook          = NULL ;
						ofn.lpTemplateName    = NULL ;
						//----End fill in the struct for common Save/Load dialog box----

						//fill in the OPENFILENAME struct to open a file
						ofn.lpstrFile = cpFileName;
						ofn.lpstrFileTitle = cpFileName;
						ofn.Flags = OFN_HIDEREADONLY | OFN_CREATEPROMPT;
						//open the common dialog box
						GetOpenFileName(&ofn);
						strcpy( cpFileName, m_strFileName );
						OutputDebugString(cpFileName);
						
						//Make sure nothing is created already if so release it all
						ReleaseSprites();
						//Create the space
						if( m_nppAnimationList ) delete [] m_nppAnimationList;
						m_nppAnimationList = new int*[MAX_SPRITE_ANIMATIONS];
						
						for( int i=0; i<MAX_SPRITE_ANIMATIONS; i++ ) m_nppAnimationList[i] = NULL;
						m_nppAnimationList[0] = new int[2]; m_nppAnimationList[0][0] = 0; m_nppAnimationList[0][1] = 0;
						
						m_pSprites = new CSPRITE[MAX_SPRITES];
						//Update the sprites utility classes
						UpdateUtilitiesClasses();
						
						m_pSprites[0].InitSprite(640,480,0,1,1,0);
						m_pSprites[0].SetAnimations( m_nppAnimationList[0],1,SPRITE_ANIM_ONCE );
						m_pSprites[0].SetCurrentAnim(0);
						if( FAILED(m_pSprites[0].LoadSpriteImage(cpFileName, 640, 480)))
							MessageBox(NULL,"FAILED","FIRED",NULL);
						m_pSprites[0].SetState(SPRITE_STATE_ALIVE);

						break;
					}
				case ID_FILE_EXIT:
					// Recieved key/menu command to exit app
					m_bExitApp = true;
					SendMessage( m_hWnd, WM_CLOSE, 0, 0 );
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