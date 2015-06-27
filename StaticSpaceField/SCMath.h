//-----------------------------------------------------------------------------
// File: SCMath.h
//
// Desc: Math funtcions and constants
//
//-----------------------------------------------------------------------------
#ifndef SCMath_h
#define SCMath_h

#include <math.h>
//#include <windows.h>
//#include <windowsx.h>

// 1ms = 1.66666667x10^-5 s
// 1s = 1000ms

////DEFINES//////////
#define PI 3.141592654f
//Constant of gravitation N*m^2 /kg^2
#define G 0.0000000000667f 
//Velocity of light m/s
#define C 300000000
//Acceleration due to gravity on earth m/s^2 or 32.174 ft/^2
#define gEarth 9.81f

//----Convertion Macros----
#define Cel_Kelv(cel) (cel+273)
#define Cel_F(cel) ( (1.8f*cel) + 32.0f )
#define Mi_Km(mi) (mi/0.6215f)
#define Km_Mi(km) (km/1.609f)
#define Deg_Rad(x) ( (x/180)*3.14159265f )

////CLASSES///////////////////

//-----------------------------------------------------------------------------
// Name: Class VECTOR
// Desc: 
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
	void  Normalize(void); //or unit vector
	void  Reverse(void);
	float XComponent(float fAngleX);
	float YComponent(float fAngleY);
	float ZComponent(float fAngleZ);

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
CVECTOR operator*(float scalar, CVECTOR u);
CVECTOR operator*(CVECTOR u, float s);
CVECTOR operator/(CVECTOR u, float s);

#endif