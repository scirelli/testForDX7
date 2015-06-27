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
		return ((r/8)<<11) | ((g/4)<<5) | (b/8);//((b&31)+((g&63)<<5)+((r&31)<<11))
	case 24:
	case 32:
		return (r<<16) | (g<<8) | (b);
	}
	return 0;
}