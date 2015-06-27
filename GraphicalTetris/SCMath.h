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
#define Cel_Kelv(cel) (cel+273.f)
#define Cel_F(cel) ( (1.8f*cel) + 32.0f )
#define Mi_Km(mi) (mi/0.6215f) //miles to kilometers
#define M_Km(m) (m/1000.f)     //meters to kilometers
#define Km_M(km) (km*1000.f)   //kilometers to meters
#define Km_Mi(km) (km/1.609f)  //kilometers to miles
#define Lb_Kg(Lb) (Lb*0.4536f)  //pounds to kilograms
#define G_Kg(g)  (g*0.001f)    //grams to kilograms
#define Kg_G(kg) (kg*1000.f)   //kilograms to grams
#define G_Lb(g)  (g*0.002205)  //grams to pounds
#define Lb_G(lb) (lb*453.6f)   //pounds to grams
#define Ton_Kg(tons) (tons*907200.f)
#define Kg_Ton(kg) (kg*0.001102f)
#define Deg_Rad(x) ( (((float)x)/180.f)*3.14159265f )

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
	float XComponent();
	float YComponent();
	float ZComponent();

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

//-----------------------------------------------------------------------------
// Name: Class PHYSPROP
// Desc: hold the physical properies of an object
//-----------------------------------------------------------------------------
class PHYSPROP
{
public:
	//Member variables
	float fMass;
	float fWeight;
	float fCenterofMassX;
	float fCenterofMassY;
	float fMomentofIneriaX;
	float fMomentofIneriaY;
	float fMomentofIneriaZ;
	float angle;
	CVECTOR VelVector;
	CVECTOR initVel;
	CVECTOR accel;
	CVECTOR force;
	CVECTOR momentum;
	float fArea;
	float fLength;
	float fWidth;
	float fHeight;
	float fX;
	float fY;
	float fZ;

	//Methods
	PHYSPROP()
	{
		fMass=0;
		fWeight=0;
		fCenterofMassX=0;
		fCenterofMassY=0;
		fMomentofIneriaX=0;
		fMomentofIneriaY=0;
		fMomentofIneriaZ=0;
		angle = 0;
		fArea=0;
		fLength=0;
		fWidth;
		fHeight=0;
		fX=0;
		fY=0;
		fZ=0;
	}
};


#endif