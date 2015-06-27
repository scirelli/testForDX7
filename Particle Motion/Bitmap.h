//-----------------------------------------------------------------------------
// File: Bitmap.h
//
// Desc: Header file for fucntion that work on bitmaps
//
//-----------------------------------------------------------------------------
#include <windows.h>
#include <windowsx.h>
#include <ddraw.h>
#include <io.h>
#include "CreateRGB.h"

#ifndef scbitmap_h
#define scbitmap_h

#define BITMAP_ID     0x4D42      // universal id for a bitmap

//-------STRUCTS-----------------
/*
typedef struct tagBITMAPFILEHEADER
{//bmfh
	WORD bfType; //type of bitmap; 0x4d42 means .BMP
	DWORD bfSize; //the total size of the file in bytes
	WORD bfReserved1; //always 0
	WORD bfReserved2; //always 0
	DWORD bfOffBits; //number of bytes from this structure to access the BITMAPINFO; basically an offset
} BITMAPFILEHEADER;

typedef struct tagRGBQUAD
{//RGBQUAD struct
	BYTE rgbBlue; //the blue component
	BYTE rgbGreen; //the green component
	BYTE rgbRed;  //the red component
	BYTE rgbReserved; //flags; must be zero
}RGBQUAD;

typedef struct tagBITMAPINFOHEADER
{
	DWORD biSize;         //number of bytes in this structure
	LONG  biWidth;        //width of bitmap
	LONG  biHeight;       //height of bitmap
	WORD  biPlanes;       //number of color planes (always 1)
	WORD  biBitCount;     //bits per pixel (1,4,8,16,24,32)
    DWORD biCompression;  //type of compression; always BI_RGB for noncompressed bitmaps
	DWORD biSizeImage;    //size of image in bytes
	LONG  biXPelsPerMeter;//X res of target display
	LONG  BiYPelsPerMeter;//Y res of target display
	DWORD biClrUsed;      //how many colors are used
	DWORD biClrImportant; //number of important colors; only for windows apps
}BITMAPINFOHEADER;
*/
typedef struct BITMAP_FILE_TAG
{
	BITMAPFILEHEADER bitmapfileheader;
	BITMAPINFOHEADER bitmapinfoheader;
	PALETTEENTRY palette[256];  //the 256 color palette
	UCHAR *bitmapData;  //pointer to data
}BITMAP_FILE, *BITMAP_FILE_PTR;

//-------CLASSES-----------------
class CBITMAP
{
private:
	
	BITMAPFILEHEADER m_bitmapfileheader;
	BITMAPINFOHEADER m_bitmapinfoheader;
	PALETTEENTRY     m_palette[256];  //the 256 color palette
	UCHAR           *m_bitmapData;  //pointer to data 
	char            *m_cpFileName;

public:
	CBITMAP(); //constructor
	CBITMAP(char* filename);
	~CBITMAP(); //destructor
	UCHAR* LoadBitmapFile();
	bool Unload_BitmapFile();
	bool FlipBitmap(UCHAR *image, int bytes_per_line, int height);
	LONG GetWidth();
	LONG GetHeight();
	WORD GetBitCount();
	DWORD GetImageSize();
	UCHAR* GetImageData();
	char* GetBitmapFileName();
	bool SetFileName(char*);
	BOOL SetPalette(LPDIRECTDRAWPALETTE palette); //set palette
};

////PROTOTYPES///////////////////
void DisplayBitmapInfo(CBITMAP);

#endif