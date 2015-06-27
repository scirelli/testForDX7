//-----------------------------------------------------------------------------
// File: Bitmap.cpp
//
// Desc: Implemtation of bitmap class
//
//-----------------------------------------------------------------------------

////INCLUDES///////////////
#include "Bitmap.h"
#include <stdio.h>
#include "CDDGameApp.h"

////GLOBALS///////////////

HWND  main_window_handle = GetGameApp()->getWndHandle();

////FUNCTIONS/////////////

//-----------------------------------------------------------------------------
// Name: DisplayBitmapInfo()
// Desc: displays a bitmaps info in a message box
//-----------------------------------------------------------------------------
void DisplayBitmapInfo(CBITMAP bitmap)
{
	char temp[600];
	char temp2[1024];

	strcpy( temp2, bitmap.GetBitmapFileName());
	strcat(temp2,"\n");
	sprintf( temp, "Height= %d", bitmap.GetHeight());
	strcat(temp2,temp);strcat(temp2,"\n");
	sprintf( temp, "Width= %d", bitmap.GetWidth());
	strcat(temp2,temp);strcat(temp2,"\n");
	sprintf( temp, "Bpp= %d", bitmap.GetBitCount());
	strcat(temp2,temp);strcat(temp2,"\n");
	sprintf( temp, "Image Size= %d", bitmap.GetImageSize());
	strcat(temp2,temp);strcat(temp2,"\n");

	MessageBox(main_window_handle, temp2, "Bitmap Info", MB_ICONEXCLAMATION);
}

////MEMBER FUNCTIONS//////////////////
//-----------------------------------------------------------------------------
// Name: CBITMAP()
// Desc: default constructor
//-----------------------------------------------------------------------------
CBITMAP::CBITMAP() //constructor
{
	m_cpFileName = "Default.bmp";
	m_bitmapData = NULL;
}

//-----------------------------------------------------------------------------
// Name: CBITMAP()
// Desc: constructor that takes in the file name
//-----------------------------------------------------------------------------
CBITMAP::CBITMAP(char* filename)
{
	m_cpFileName = filename;
	m_bitmapData = NULL;
}

//-----------------------------------------------------------------------------
// Name: ~CBITMAP()
// Desc: destructor cleans up all used memory
//-----------------------------------------------------------------------------
CBITMAP::~CBITMAP() //destructor
{
	if(m_bitmapData)
	{
		free(m_bitmapData);
		m_bitmapData = NULL;
	}
}

//-----------------------------------------------------------------------------
// Name: FlipBitmap()
// Desc: Flips the bitmap because bitmaps are natural inverted
//-----------------------------------------------------------------------------
bool CBITMAP::FlipBitmap(UCHAR *image, int bytes_per_line, int height)
{
	// this function is used to flip upside down .BMP images

	UCHAR *buffer; // used to perform the image processing
	int index;     // looping index

	// allocate the temporary buffer
	if (!(buffer = (UCHAR *)malloc(bytes_per_line*height)))
	{
		MessageBox(main_window_handle,"Could not allocate temp buffer", "FlipBitmap() Error",MB_ICONEXCLAMATION);
		return(0);
	}

	// copy image to work area
	memcpy(buffer,image,bytes_per_line*height);

	// flip vertically
	for (index=0; index < height; index++)
		 memcpy(&image[((height-1) - index)*bytes_per_line], &buffer[index*bytes_per_line], bytes_per_line);

	// release the memory
	free(buffer);

	// return success
	return(1);

} // end Flip_Bitmap

//-----------------------------------------------------------------------------
// Name: LoadBitmapFile()
// Desc: Loads a bitmap into mem
//-----------------------------------------------------------------------------
UCHAR* CBITMAP::LoadBitmapFile()
{
	// this function opens a bitmap file and loads the data into bitmap

	HFILE file_handle,  // the file handle
		  index;        // looping index

	UCHAR *temp_buffer = NULL; // used to convert 24 bit images to 16 bit
	OFSTRUCT file_data;        // the file data information

	// open the file if it exists
	if ((file_handle = (HFILE)CreateFile(m_cpFileName,GENERIC_READ,FILE_SHARE_READ,(LPSECURITY_ATTRIBUTES)NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,(HANDLE)NULL))==-1)
	{
		MessageBox(main_window_handle,"Could not create file handle. File could not be open.", "LoadBitmapFile() File error",MB_ICONEXCLAMATION);
		return(0);
	}
	
	/*
	char h;int count=0;char buf[100];
	while( _lread(file_handle, &h,sizeof(char)) != HFILE_ERROR)
		count++;
	sprintf(buf,"Size of Image= %d", count);
	MessageBox(main_window_handle,buf, " ",MB_ICONEXCLAMATION);
	*/
	// now load the bitmap file header
	if( ( _lread(file_handle, &m_bitmapfileheader,sizeof(BITMAPFILEHEADER)) ) == HFILE_ERROR )
	{
		MessageBox(main_window_handle,"Error loading bitmap file header.", "LoadBitmapFile() File error",MB_ICONEXCLAMATION);
		return(0);
	}
	
	// test if this is a bitmap file
	if (m_bitmapfileheader.bfType!=BITMAP_ID)
	{
		// close the file
		_lclose(file_handle);
		
		MessageBox(main_window_handle,"File is not a bitmap.", "LoadBitmapFile() File error",MB_ICONEXCLAMATION);
		 // return error
		return(0);
	} // end if

	// now we know this is a bitmap, so read in all the sections

	// first the bitmap infoheader
	
	// now load the bitmap file header
	if( (_lread(file_handle, &m_bitmapinfoheader,sizeof(BITMAPINFOHEADER))) == HFILE_ERROR)
	{
		MessageBox(main_window_handle,"Load bitmap file header2 failed", "LoadBitmapFile() File error",MB_ICONEXCLAMATION);
		return(0);
	}
	
	// now load the color palette if there is one
	if (m_bitmapinfoheader.biBitCount == 8)
	{
		_lread(file_handle, &m_palette,256*sizeof(PALETTEENTRY));

		// now set all the flags in the palette correctly and fix the reversed 
		// BGR RGBQUAD data format
		 for (index=0; index < 256; index++)
		 {
			 // reverse the red and green fields
			 int temp_color = m_palette[index].peRed;
			 m_palette[index].peRed  = m_palette[index].peBlue;
			 m_palette[index].peBlue = temp_color;
       
			 // always set the flags word to this
			 m_palette[index].peFlags = PC_NOCOLLAPSE;
		 } // end for index

    } // end if
	
	// finally the image data itself
	_lseek(file_handle,-(int)(m_bitmapinfoheader.biSizeImage),SEEK_END);
	/* 
	if( (_lseek(file_handle,-(int)(m_bitmapinfoheader.biSizeImage),SEEK_END)) == -1L) //- because it starts at the end of the file and moves backwards to beginning of image
	{
		char buffer[1000], temp[100];
		
		sprintf(buffer,"Size of Image= %d", -(int)(m_bitmapinfoheader.biSizeImage));
		sprintf(temp, "\nSize of file= %d",m_bitmapinfoheader.biSize);
		strcat(buffer,temp);
		sprintf(temp,"\nWidth= %d",m_bitmapinfoheader.biWidth);
		strcat(buffer,temp);
		sprintf(temp,"\nHeight= %d",m_bitmapinfoheader.biHeight);
		strcat(buffer,temp);
		strcat(buffer,"\nSeek to image data failed");
		MessageBox(main_window_handle,buffer, "LoadBitmapFile() File error",MB_ICONEXCLAMATION);
		// close the file
    	//_lclose(file_handle);
	}
	*/
	// now read in the image, if the image is 8 or 16 bit then simply read it
	// but if its 24 bit then read it into a temporary area and then convert
	// it to a 16 bit image

	if (m_bitmapinfoheader.biBitCount==8 || m_bitmapinfoheader.biBitCount==16)
	{
		if (m_bitmapData)//free up space if it allocated
			free(m_bitmapData);
		// allocate the memory for the image
		 if (!(m_bitmapData = (UCHAR *)malloc(m_bitmapinfoheader.biSizeImage)))
		 {
			 // close the file
			  _lclose(file_handle);
				
			 MessageBox(main_window_handle,"Could not allocate mem", "LoadBitmapFile() Allocation Error",MB_ICONEXCLAMATION);
			 // return error
			return(0);
		 } // end if

		// now read it in
		if( (_lread(file_handle,m_bitmapData,m_bitmapinfoheader.biSizeImage)) == HFILE_ERROR)
		{
			MessageBox(main_window_handle,"Read image data failed (122)", "LoadBitmapFile() File error",MB_ICONEXCLAMATION);
			if (m_bitmapData)//free up space if it allocated
				free(m_bitmapData);
			// close the file
			 _lclose(file_handle);
			return(0);
		}
	} // end if
	else
	{
		// this must be a 24 bit image, load it in and convert it to 16 bit
		// printf("\nconverting 24 bit image...");
		
		// allocate temporary buffer
		if (!(temp_buffer = (UCHAR *)malloc(m_bitmapinfoheader.biSizeImage)))
		{
			// close the file
			_lclose(file_handle);

			// return error
			return(0);
		} // end if

		// allocate final 24 bit storage buffer
		if (!(m_bitmapData=(UCHAR *)malloc(2*m_bitmapinfoheader.biWidth*m_bitmapinfoheader.biHeight)))
		{
			 // close the file
			 _lclose(file_handle);

			 // release working buffer
			 free(temp_buffer);
			
			 MessageBox(main_window_handle,"Could not allocate final 16bit storage buffer", "LoadBitmapFile() Allocation Error",MB_ICONEXCLAMATION);
			// return error
			return(0);
		} // end if

		// now read it in
		if( (_lread(file_handle,temp_buffer,m_bitmapinfoheader.biSizeImage)) == HFILE_ERROR)
		{
			MessageBox(main_window_handle,"Read image data failed (123)", "LoadBitmapFile() File error",MB_ICONEXCLAMATION);
			return(0);
		}

		// now convert each 24 bit RGB value into a 16 bit value
		for (index=0; index<m_bitmapinfoheader.biWidth*m_bitmapinfoheader.biHeight; index++)
		{
			// extract RGB components (in BGR order), note the scaling
			UCHAR blue  = (temp_buffer[index*3 + 0] >> 3), green = (temp_buffer[index*3 + 1] >> 3),
														   red   = (temp_buffer[index*3 + 2] >> 3);

			// build up 16 bit color word
			USHORT color = BIULDCOLOR(red,green,blue);

			// write color to buffer
			((USHORT *)m_bitmapData)[index] = color;

		} // end for index

		// finally write out the correct number of bits
		m_bitmapinfoheader.biBitCount=16;

	} // end if

	#if 0
		// write the file info out 
			printf("\nfilename:%s \nsize=%d \nwidth=%d \nheight=%d \nbitsperpixel=%d \ncolors=%d \nimpcolors=%d",
					filename,
					m_bitmapinfoheader.biSizeImage,
					m_bitmapinfoheader.biWidth,
					m_bitmapinfoheader.biHeight,
					m_bitmapinfoheader.biBitCount,
					m_bitmapinfoheader.biClrUsed,
					m_bitmapinfoheader.biClrImportant);
	#endif

	// close the file
	_lclose(file_handle);

	// flip the bitmap
	FlipBitmap(m_bitmapData, m_bitmapinfoheader.biWidth*(m_bitmapinfoheader.biBitCount/8), 
							  m_bitmapinfoheader.biHeight);

	// return success
	return m_bitmapData;

} // end Load_Bitmap_File

//-----------------------------------------------------------------------------
// Name: LoadBitmapFile24()
// Desc: Loads a bitmap into mem and converts 24bits to 32
//-----------------------------------------------------------------------------
UCHAR* CBITMAP::LoadBitmapFile24()
{
	// this function opens a bitmap file and loads the data into bitmap

	HFILE file_handle;  // the file handle
	int	  index;        // looping index
	UCHAR *temp_buffer = NULL; // used to convert 24 bit images to 32 bit
	OFSTRUCT file_data;        // the file data information

	// open the file if it exists
	if ((file_handle = (HFILE)CreateFile(m_cpFileName,GENERIC_READ,FILE_SHARE_READ,(LPSECURITY_ATTRIBUTES)NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,(HANDLE)NULL))==-1)
	{
		MessageBox(main_window_handle,"Could not create file handle. File could not be open.", "LoadBitmapFile24() File error",MB_ICONEXCLAMATION);
		return(0);
	}

	// now load the bitmap file header
	if( (_lread(file_handle, &m_bitmapfileheader,sizeof(BITMAPFILEHEADER))) == HFILE_ERROR)
	{
		MessageBox(main_window_handle,"Read Bitmap Header failed (1)", "LoadBitmapFile24() Read error",MB_ICONEXCLAMATION);
		return(0);
	}


	// test if this is a bitmap file
	if (m_bitmapfileheader.bfType!=BITMAP_ID)
	{
		// close the file
		_lclose(file_handle);
		
		MessageBox(main_window_handle,"File is not a bitmap.", "LoadBitmapFile24() File error",MB_ICONEXCLAMATION);
		 // return error
		return(0);
	} // end if

	// now we know this is a bitmap, so read in all the sections

	// first the bitmap infoheader

	// now load the bitmap file header
	if((_lread(file_handle, &m_bitmapinfoheader,sizeof(BITMAPINFOHEADER))) == HFILE_ERROR)
	{
		MessageBox(main_window_handle,"Read Bitmap header failed (2)", "LoadBitmapFile24() File error",MB_ICONEXCLAMATION);
		return(0);
	}


	// finally the image data itself
	_lseek(file_handle,-(int)(m_bitmapinfoheader.biSizeImage),SEEK_END);
	
	/*
	if( (_lseek(file_handle,-(int)(m_bitmapinfoheader.biSizeImage),SEEK_END)) == -1L) //- because it starts at the end of the file and moves backwards to beginning of image
	{
		char buffer[1000], temp[100];
		
		sprintf(buffer,"Size of Image= %d", (int)(m_bitmapinfoheader.biSizeImage));
		sprintf(temp, "\nSize of file= %d",m_bitmapinfoheader.biSize);
		strcat(buffer,temp);
		sprintf(temp,"\nWidth= %d",m_bitmapinfoheader.biWidth);
		strcat(buffer,temp);
		sprintf(temp,"\nHeight= %d",m_bitmapinfoheader.biHeight);
		strcat(buffer,temp);
		strcat(buffer,"\nSeek to image data failed");
		MessageBox(main_window_handle,buffer, "LoadBitmapFile() File error",MB_ICONEXCLAMATION);
		// close the file
    	//_lclose(file_handle);
	}
	*/

	// now read in the image.

	if (m_bitmapinfoheader.biBitCount==24)
	{
		// allocate temporary buffer
		if (!(temp_buffer = (UCHAR *)malloc(m_bitmapinfoheader.biSizeImage)))
		{
			// close the file
			_lclose(file_handle);

			// return error
			return(0);
		} // end if
		
		if (m_bitmapData)//free up space if it allocated
			free(m_bitmapData);
		// allocate the memory for the image
		 if ( !(m_bitmapData=(UCHAR *)malloc(4*m_bitmapinfoheader.biWidth*m_bitmapinfoheader.biHeight)) )
		 {
			 // close the file
			  _lclose(file_handle);
				
			 MessageBox(main_window_handle,"Could not allocate mem", "LoadBitmapFile24() Allocation Error",MB_ICONEXCLAMATION);
			 // return error
			return(0);
		 } // end if

		// now read it in
		if((_lread(file_handle,temp_buffer,m_bitmapinfoheader.biSizeImage)) == HFILE_ERROR)
		{
			MessageBox(main_window_handle,"Read image data failed (3)", "LoadBitmapFile24() File error",MB_ICONEXCLAMATION);
			return(0);
		}
		
		// now convert each 24 bit RGB value into a 32 bit value
		for (index=0; index<m_bitmapinfoheader.biWidth*m_bitmapinfoheader.biHeight; index++)
		{
			// extract RGB components (in BGR order), note5the scaling
			UCHAR blue  = (temp_buffer[index*3 + 0] ), 
				  green = (temp_buffer[index*3 + 1] ),
				  red   = (temp_buffer[index*3 + 2] );

			// build up 16 bit color word
			DWORD color = _ARGB24BIT(0,red,green,blue);

			// write color to buffer
			((DWORD *)m_bitmapData)[index] = color;

		} // end for index

	} // end if
	else
		MessageBox(main_window_handle, "Not a 24bit bitmap", "LoadBitmapFile24 Error",MB_ICONEXCLAMATION);

	// close the file
	_lclose(file_handle);

	// flip the bitmap
	FlipBitmap(m_bitmapData, m_bitmapinfoheader.biWidth*(m_bitmapinfoheader.biBitCount/6), 
							  m_bitmapinfoheader.biHeight);
	free(temp_buffer);
	// return success
	return m_bitmapData;

} // end Load_Bitmap_File 24

//-----------------------------------------------------------------------------
// Name: Unload_BitmapFile()
// Desc: cleans up the bitmap releases all used mem
//-----------------------------------------------------------------------------
bool CBITMAP::Unload_BitmapFile()
{
	if(m_bitmapData)
	{
		free(m_bitmapData);
		m_bitmapData = NULL;
	}
	return(1);
}

//-----------------------------------------------------------------------------
// Name: GetWidth()
// Desc: 
//-----------------------------------------------------------------------------
LONG CBITMAP::GetWidth()
{
	return m_bitmapinfoheader.biWidth;
}

//-----------------------------------------------------------------------------
// Name: GetHeight()
// Desc: 
//-----------------------------------------------------------------------------
LONG CBITMAP::GetHeight()
{
	return m_bitmapinfoheader.biHeight;
}

//-----------------------------------------------------------------------------
// Name: GetBitCount()
// Desc: gets the bpp
//-----------------------------------------------------------------------------
WORD CBITMAP::GetBitCount()
{
	return m_bitmapinfoheader.biBitCount;
}

//-----------------------------------------------------------------------------
// Name: GetImageSize()
// Desc: returns the size of the image
//-----------------------------------------------------------------------------
DWORD CBITMAP::GetImageSize()
{
	return m_bitmapinfoheader.biSizeImage;
}

//-----------------------------------------------------------------------------
// Name: GetBitmapFileName()
// Desc: returns the name of the bitmap file
//-----------------------------------------------------------------------------
char* CBITMAP::GetBitmapFileName()
{
	return m_cpFileName;
}

//-----------------------------------------------------------------------------
// Name: SetPalette()
// Desc: takes the pallete from the bitmap file and copies it to a ddpallete
//-----------------------------------------------------------------------------
BOOL CBITMAP::SetPalette(LPDIRECTDRAWPALETTE palette)
{
	return (0);
}

//-----------------------------------------------------------------------------
// Name: SetFileName
// Desc: 
//-----------------------------------------------------------------------------
bool CBITMAP::SetFileName(char* filename)
{
	m_cpFileName = filename;
	return (1);
}

//-----------------------------------------------------------------------------
// Name: GetImageData()
// Desc: Returns a pointer to the image data
//-----------------------------------------------------------------------------
UCHAR* CBITMAP::GetImageData()
{
	return m_bitmapData;
}