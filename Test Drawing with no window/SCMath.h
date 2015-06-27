//-----------------------------------------------------------------------------
// File: SCMath.h
//
// Desc: Math funtcions and constants
//
//-----------------------------------------------------------------------------
#ifndef SCMath_h
#define SCMath_h

#include <Math.h>
#include <stdlib.h>
#include <windows.h>
//#include <windowsx.h>

// 1ms = 1.66666667x10^-5 s
// 1s = 1000ms

////DEFINES//////////
#define PI 3.141592654f
//Constant of gravitation
#define G 0.0000000000667 //N*m^2 /kg^2 
//Velocity of light
#define c 300000000 //m/s


//----Convertion Macros----
#define Cel_Kelv(cel) (cel+273)
#define Cel_F(cel) ( (1.8f*cel) + 32.0f )
#define Mi_Km(mi) (mi/0.6215f)
#define Km_Mi(km) (km/1.609f)
#define Deg_Rad(x) ( (x/180)*3.14159265f )

////PROTOTYPES////////////////
int Rand(int, int);
float Rand(float s1, float s2);

////CLASSES///////////////////

//-----------------------------------------------------------------------------
// Name: Class VECTOR
// Desc: draws a 16bit pixel to a surface
//-----------------------------------------------------------------------------
class CVECTOR
{
public:
	float x;
	float y;
	float z;

	CVECTOR(void);
	CVECTOR(float xi, float yi, float zi);

	float Magnitude(void);
	void Normalize(void); //or unit vector
	void Reverse(void);

	CVECTOR& operator += (CVECTOR u);
	CVECTOR& operator -= (CVECTOR u);
	CVECTOR& operator *= (float s);
	CVECTOR& operator /= (float s);

	CVECTOR operator - (void);
};

CVECTOR operator+(CVECTOR u, CVECTOR v);
CVECTOR operator -(CVECTOR u, CVECTOR v);
CVECTOR operator ^(CVECTOR u, CVECTOR v);
float operator*(CVECTOR u, CVECTOR v);
inline CVECTOR operator*(float s, CVECTOR u);
inline CVECTOR operator*(CVECTOR u, float s);
inline CVECTOR operator/(CVECTOR u, float s);






#endif