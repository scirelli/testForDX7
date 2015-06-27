//-----------------------------------------------------------------------------
// File: Main.cpp
//
// Desc: Where everything is done
//
//-----------------------------------------------------------------------------
////INCLUDES///////////////////
#include "General.h"
#include "CreateRGB.h"
#include "PrimitiveShapes.h"
#include "Bitmap.h"
#include "SCTimer.h"
#include "Sprite.h"

////GLOBALS////////////////////
extern LPDIRECTDRAWSURFACE7 lpddsPrimary;    // dd primary surface
extern LPDIRECTDRAWSURFACE7 lpddsSecondary;  // dd back surface
extern DDSURFACEDESC2       ddsd;            // a direct draw surface description struct
extern HRESULT              ddReturnVal;     // result back from dd calls
extern DDBLTFX              ddbltfx;
extern char                 cpErrorBuf[1024];
extern RECT                 wndRect; 
extern HWND					main_window_handle;
extern CTIMER               gametimer;

extern CBITMAP bitmap1,bitmap2;
extern CSPRITE sprite,myface,ship,recttest,ship2;
extern LPDIRECTDRAWSURFACE7 Temp;
////PROTOTYPES/////////////////

////FUNCTIONS//////////////////
//-----------------------------------------------------------------------------
// Name: Main()
// Desc: Whwere all work is done
//-----------------------------------------------------------------------------
bool main()
{
	static SCCOLOR col;
	static UCHAR* back_buf;
	static HDC hDC;
	static int iFrames=0;
	static int x=0;

	col.b = col.r = col.g = 255;

	if(GetKeyStatus(VK_ESCAPE) == true)
		return(0);
	
	//Clear the surface
	if(!ClrS(lpddsSecondary,wndRect))//can't be within a lock-Unlock block or u can't blt
		return(0);
	
	//Write text to the Secondary surface remember to Release the DC
	ddReturnVal = lpddsSecondary->GetDC(&hDC);
	if (DDFailedCheck(ddReturnVal, "GetDC() failed", cpErrorBuf ))
	{	
		if(ddReturnVal == DDERR_SURFACELOST)
		{
			lpddsSecondary->Restore();
			lpddsSecondary->GetDC(&hDC);
		}
		else
		{
			MessageBox(main_window_handle, cpErrorBuf, "main()", MB_ICONEXCLAMATION);   
			return(0); 
		}
	}
	
	static char buffer[1024];
	SetBkColor(hDC,RGB(0,0,0));
	SetBkMode(hDC,OPAQUE);
	sprintf(buffer, "Time elapsed(s)= %d", (int)(gametimer.GetStartElapsedTime()*0.001) );//0.00001666666f
	static DWORD starttime=GetTickCount();
	if( gametimer.Elapsed(starttime,(60/0.001)) )
		sprintf(buffer,"TIME!"," 1 minute");
	SetTextColor(hDC,RGB(255,255,255));
	iFrames++;
	static int FramesPerSec=0;
	FramesPerSec = iFrames/( ((int)(gametimer.GetStartElapsedTime()*0.001)+1) );
	sprintf(buffer, "Frames/s = %d", FramesPerSec);
	TextOut(hDC, 100,100, buffer, strlen(buffer));
	
	sprintf(buffer, "X= %d", x);
	TextOut(hDC, 100,140, buffer, strlen(buffer));
	ddReturnVal = lpddsSecondary->ReleaseDC(hDC);
	if (DDFailedCheck(ddReturnVal, "ReleaseDC() failed", cpErrorBuf ))
	{	MessageBox(main_window_handle, cpErrorBuf, "main()", MB_ICONEXCLAMATION);  return(0); }
	
	//Lock the surface for drawing
	if(!Lock(lpddsSecondary))
		return(0);

	back_buf = (UCHAR*)ddsd.lpSurface;

	//test drawing pixels
	DrawPixel(ddsd,x+200,100,col); DrawPixel(ddsd,x+200,101,col); DrawPixel(ddsd,x+200,102,col);
	DrawPixel(ddsd,x+200,103,col); DrawPixel(ddsd,x+200,104,col); DrawPixel(ddsd,x+200,105,col);
	DrawPixel(ddsd,x+200,106,col); DrawPixel(ddsd,x+200,107,col); DrawPixel(ddsd,x+200,108,col);
	x++;
	if(x>780)
		x=0;
	
	//test bitmap class
	for (int y=0; y < bitmap1.GetHeight(); y++)
    {
		// copy the line
		memcpy(&back_buf[100+(y+200)*ddsd.lPitch], // dest address
			   &bitmap1.GetImageData()[y*bitmap1.GetWidth()*2],   // src address
			   bitmap1.GetWidth()*2);                         // bytes to copy
    } // end for y
	
	//Unlock the surface
	if(!UnLock(lpddsSecondary))
		return(0);
	
	if(!Lock(Temp))
		return(0);
	back_buf = (UCHAR*)ddsd.lpSurface;
	for (int y=0; y < bitmap2.GetHeight(); y++)
    {
		// copy the line
		memcpy(&back_buf[y*ddsd.lPitch], // dest address
			   &bitmap2.GetImageData()[y*bitmap2.GetWidth()*2],   // src address
			   bitmap2.GetWidth()*2);                         // bytes to copy
    } // end for y
	if(!UnLock(Temp))
		return(0);
	static RECT temprect={200,200,520,400};
	static RECT sorc_rect={0,0,320,200};
	ddReturnVal = lpddsSecondary->Blt(&temprect/*dest rect*/,Temp, //pointer to source surface
							  &sorc_rect, //pointer to the source rectangle
							  DDBLT_WAIT | DDBLT_KEYSRC,NULL);
	if (DDFailedCheck(ddReturnVal, "Blt failed", cpErrorBuf ))
	{	MessageBox(main_window_handle, cpErrorBuf, "Main()", MB_ICONEXCLAMATION);   return(0); }
	
	
	static RECT temprect2;
	temprect2.top=200; temprect2.left=500; temprect2.right=sprite.GetWidth()+500; temprect2.bottom=sprite.GetHeight()+200;
	static int i=0;
	ddReturnVal = lpddsSecondary->Blt(&temprect2/*dest rect*/,sprite.GetFrame(i), //pointer to source surface
							          NULL, //pointer to the source rectangle
							          DDBLT_WAIT | DDBLT_KEYSRC,NULL/*ddbltfx struct*/);
	if (DDFailedCheck(ddReturnVal, "Blt failed", cpErrorBuf ))
	{	MessageBox(main_window_handle, cpErrorBuf, "Main()", MB_ICONEXCLAMATION);   return(0); }
	static DWORD start = GetTickCount();
	if(gametimer.Elapsed(start,120))
	{i++;  start = GetTickCount();}
	if(i>13)
		i=0;
	
	static int size=0;
	temprect2.top=500+size;temprect2.left=300+size;temprect2.bottom=500+myface.GetWidth()+size;temprect2.right=300+myface.GetWidth()+size;
	ddReturnVal = lpddsSecondary->Blt(&temprect2/*dest rect*/,myface.GetFrame(0), //pointer to source surface
							          NULL, //pointer to the source rectangle
							          DDBLT_WAIT | DDBLT_KEYSRC,NULL/*ddbltfx struct*/);
	if (DDFailedCheck(ddReturnVal, "Blt failed", cpErrorBuf ))
	{	MessageBox(main_window_handle, cpErrorBuf, "Main()", MB_ICONEXCLAMATION);   return(0); }
	
	static int what=0;
	what++;
	size= sin(what)+cos(what+5);
	
	static int j=0;static DWORD start2=GetTickCount();
	if(gametimer.Elapsed(start2,50))
	{j++;start2=GetTickCount();}
	if(j>=3)j=0;
	temprect2.top=500;temprect2.left=100;temprect2.bottom=500+ship.GetHeight();temprect2.right=100+ship.GetWidth();
	ddReturnVal = lpddsSecondary->Blt(&temprect2/*dest rect*/,ship.GetFrame(j), //pointer to source surface
							          NULL, //pointer to the source rectangle
							          DDBLT_WAIT | DDBLT_KEYSRC,NULL/*ddbltfx struct*/);
	if (DDFailedCheck(ddReturnVal, "Blt failed", cpErrorBuf ))
	{	MessageBox(main_window_handle, cpErrorBuf, "Main()", MB_ICONEXCLAMATION);   return(0); }
	
	temprect2.top=400;
	temprect2.left=500;
	temprect2.bottom=400+recttest.GetHeight()+50;
	temprect2.right=500+recttest.GetWidth()+50;
	
	ddReturnVal = lpddsSecondary->Blt(&temprect2/*dest rect*/,recttest.GetFrame(0), //pointer to source surface
							          &recttest.GetRect(), //pointer to the source rectangle
							          DDBLT_WAIT | DDBLT_KEYSRC,NULL/*ddbltfx struct*/);
	if (DDFailedCheck(ddReturnVal, "Blt failed", cpErrorBuf ))
	{	MessageBox(main_window_handle, cpErrorBuf, "Main()", MB_ICONEXCLAMATION);   return(0); }
	
	temprect2.top=485;
	temprect2.left=160;
	temprect2.bottom=485+ship2.GetHeight();
	temprect2.right=160+ship2.GetWidth();
	static int k=0; static DWORD start3;
	if(gametimer.Elapsed(start3,0))
	{k++; start3=GetTickCount();}
	if(k>=11)k=0;
	
	ddReturnVal = lpddsSecondary->Blt(&temprect2/*dest rect*/,ship2.GetFrame(0), //pointer to source surface
							          &ship2.GetRect(), //pointer to the source rectangle
							          DDBLT_WAIT | DDBLT_KEYSRC,NULL/*ddbltfx struct*/);
	if (DDFailedCheck(ddReturnVal, "Blt failed", cpErrorBuf ))
	{	MessageBox(main_window_handle, cpErrorBuf, "Main()", MB_ICONEXCLAMATION);   return(0); }
	
	Flip();
	
	return(1);
}