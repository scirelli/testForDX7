//-----------------------------------------------------------------------------
// File: CreateRGB.h
//
// Desc: Header file for createRGB.cpp
//
//-----------------------------------------------------------------------------
#ifndef SurfaceFunc_h
#define SurfaceFunc_h

////PROTOTYPES///////////
LPDIRECTDRAWCLIPPER AttachClipper(LPDIRECTDRAWSURFACE7 lpddsSurface, HWND hWnd);
LPDIRECTDRAWCLIPPER AttachClipper(LPDIRECTDRAWSURFACE7 lpddsSurface, int iNumRects, LPRECT cliplist);
int Flip();
int FlipToClient(LPDIRECTDRAWSURFACE7);
int FlipToWindow(LPDIRECTDRAWSURFACE7);
int Flip(LPDIRECTDRAWSURFACE7 lpddsSurface, RECT destrect);
int Flip(LPDIRECTDRAWSURFACE7 lpddsSurface, int x, int y, int x2, int y2);
int CopySurface(LPDIRECTDRAWSURFACE7 lpddsSurface1, RECT *destRect,LPDIRECTDRAWSURFACE7 lpddsSurface2, RECT *sourRect);
LPDIRECTDRAWSURFACE7 CreateSurface(int width, int height,SCCOLOR TransparentColor);
int Lock(LPDIRECTDRAWSURFACE7);
int UnLock(LPDIRECTDRAWSURFACE7);

#endif