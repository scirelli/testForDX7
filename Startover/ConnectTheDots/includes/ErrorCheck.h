//-----------------------------------------------------------------------------
// File: ErrorCheck.h
//
// Desc: Header for the error checking file
//
//-----------------------------------------------------------------------------

#ifndef ErrorCheck_h
#define ErrorCheck_h

////INCLUDES/////////////////////
#include <windows.h>

char *DDErrorString(HRESULT hr, bool &bFatal);
bool  DDFailedCheck(HRESULT hr, char *szMessage, char * cpErrorBuf);

char *DIErrorString(HRESULT hr);
bool  DIFailedCheck(HRESULT hr, char *szMessage, char * cpErrorBuf);

char *DSErrorString(HRESULT hr);
bool  DSFailedCheck(HRESULT hr, char *szMessage, char * cpErrorBuf);

#endif