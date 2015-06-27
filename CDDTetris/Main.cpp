//-----------------------------------------------------------------------------
// File: Main.cpp
//
// Desc: Main application entry & handling source file.
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Main Module Includes
//-----------------------------------------------------------------------------
#include "Main.h"
#include "CTetris.h"
//-----------------------------------------------------------------------------
// Global Variable Definitions
//-----------------------------------------------------------------------------
CTetris    g_App;      // Core game application processing engine

//-----------------------------------------------------------------------------
// Name : WinMain() (Application Entry Point)
// Desc : Entry point for program, App flow starts here.
//-----------------------------------------------------------------------------
int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int iCmdShow )
{
    int retCode;
	
	if( (strcmp(lpCmdLine,"debug") == 0) || (strcmp(lpCmdLine,"Debug") == 0) )
		g_App.setDebugMode(true);
	// Initialise the engine.
	g_App.setDisplaySettings(true);
	if (!g_App.InitInstance( hInstance, lpCmdLine, iCmdShow )) return 0;
    
    // Begin the gameplay process. Will return when app due to exit.
    retCode = g_App.BeginGame();

    // Shut down the engine, just to be polite, before exiting.
    if ( !g_App.ShutDown() )  MessageBox( 0, _T("Failed to shut system down correctly, please check file named 'debug.txt'.\r\n\r\nIf the problem persists, please contact technical support."), _T("Non-Fatal Error"), MB_OK | MB_ICONEXCLAMATION );

    // Return the correct exit code.
    return retCode;
}

//-----------------------------------------------------------------------------
// Name : GetGameApp()
// Desc : Allows the rest of the application to retrieve the game app pointer
//-----------------------------------------------------------------------------
CDDGameApp * GetGameApp()
{
    return (CDDGameApp*)&g_App;
}