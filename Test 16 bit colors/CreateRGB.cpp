//-----------------------------------------------------------------------------
// File: CreateRGB.cpp
//
// Desc: Biulds the RGB in diffrent modes
//
//-----------------------------------------------------------------------------

unsigned int CreateRGB( int r, int g, int b, int iBpp )
{
	switch (iBpp)
	{
	case 8:
		// Here you should do a palette lookup to find the closes match.
		// I'm not going to bother with that. Many modern games no
		// longer support 256-color modes, and neither should you :)
		return 0;
	case 16:
		// Break down r,g,b into 5-6-5 format.
		return ((r/8)<<11) | ((g/4)<<5) | (b/8);
	case 24:
	case 32:
		return (r<<16) | (g<<8) | (b);
	}
	return 0;
}