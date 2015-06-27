//-----------------------------------------------------------------------------
// File: SCMath.cpp
//
// Desc: Implemt. of Math funtcions, classes and stuff
//
//-----------------------------------------------------------------------------
#include "SCMath.h"

////GLOBALS////////////////////
float const tol = 0.0001f;

////FUNCTIONS//////////////////

//-----------------------------------------------------------------------------
// Name: CVECTOR()
// Desc: constructor
//-----------------------------------------------------------------------------
inline CVECTOR::CVECTOR()
{
	x=y=z=0;
}

//-----------------------------------------------------------------------------
// Name: CVECTOR(...)
// Desc: constructor
//-----------------------------------------------------------------------------
inline CVECTOR::CVECTOR(float xi, float yi, float zi)
{
	x = xi;
	y = yi;
	z = zi;
}

//-----------------------------------------------------------------------------
// Name: Magnitude()
// Desc: returns the magnitude of a vector
//-----------------------------------------------------------------------------
inline float CVECTOR::Magnitude()
{
	return (float) sqrt(x*x + y*y + z*z);
}

//-----------------------------------------------------------------------------
// Name: Normalize()
// Desc: normalizes or turn in to a unit vector
//-----------------------------------------------------------------------------
inline  void CVECTOR::Normalize()
{
	float m = (float) sqrt(x*x + y*y + z*z);

	if(m <= tol)
		m = 1;

	x/=m;
	y/=m;
	z/=m;

	if(fabs(z) < tol)
		z= 0.0f;
	if(fabs(z) < tol)
		z= 0.0f;
	if(fabs(z) < tol)
		z= 0.0f;
}

//-----------------------------------------------------------------------------
// Name: Reverse()
// Desc: negates a vector, makes it point the oposite way
//-----------------------------------------------------------------------------
inline void CVECTOR::Reverse()
{
	x=-x;
	y=-y;
	z=-z;
}

//-----------------------------------------------------------------------------
// Name: +=(CVECTOR u)
// Desc: addes the to another vector
//-----------------------------------------------------------------------------
inline CVECTOR& CVECTOR::operator +=(CVECTOR u)
{
	x+= u.x;
	y+= u.y;
	z+= u.z;
	return *this;
}

//-----------------------------------------------------------------------------
// Name: -=(CVECTOR u)
// Desc: subtracts the vector from another one
//-----------------------------------------------------------------------------
inline CVECTOR& CVECTOR::operator -=(CVECTOR u)
{
	x -= u.x;
	y -= u.y;
	z -= u.z;
	return *this;
}

//-----------------------------------------------------------------------------
// Name: 
// Desc: Multiplies the vector and another vector
//-----------------------------------------------------------------------------
inline CVECTOR& CVECTOR::operator *=(float s)
{
	x*=s;
	y*=s;
	z*=s;
	return *this;
}

//-----------------------------------------------------------------------------
// Name: 
// Desc: divides the vector by another vector
//-----------------------------------------------------------------------------
inline CVECTOR& CVECTOR::operator /=(float s)
{
	x/=s;
	y/=s;
	z/=s;
	return *this;
}

//-----------------------------------------------------------------------------
// Name: -()
// Desc: negates the vector
//-----------------------------------------------------------------------------
inline CVECTOR CVECTOR::operator -()
{
	return CVECTOR(-x,-y,-z);
}

//-----------------------------------------------------------------------------
// Name:
// Desc: add vectors
//-----------------------------------------------------------------------------
inline CVECTOR operator+(CVECTOR u, CVECTOR v)
{
	return CVECTOR(u.x + v.x, u.y + v.y , u.z + v.z);
}

//-----------------------------------------------------------------------------
// Name:
// Desc: subtract vectors
//-----------------------------------------------------------------------------
inline CVECTOR operator -(CVECTOR u, CVECTOR v)
{
	return CVECTOR(u.x - v.x, u.y - v.y , u.z - v.z);
}

//-----------------------------------------------------------------------------
// Name:
// Desc: cross product
//-----------------------------------------------------------------------------
inline CVECTOR operator ^(CVECTOR u, CVECTOR v)
{
	return CVECTOR(u.y*v.z - u.z*v.y, -u.x*v.z + u.z*v.x, u.x*v.y - u.y*v.x);
}

//-----------------------------------------------------------------------------
// Name:
// Desc: dot product of two vectors
//-----------------------------------------------------------------------------
inline float operator*(CVECTOR u, CVECTOR v)
{
	return (u.x*v.x + u.y*v.y + u.z*v.z);
}

//-----------------------------------------------------------------------------
// Name:
// Desc: dot of a scaler and a vector
//-----------------------------------------------------------------------------
inline CVECTOR operator*(float s, CVECTOR u)
{
	return CVECTOR(u.x*s,u.y*s,u.z*s);
}

//-----------------------------------------------------------------------------
// Name:
// Desc: dot product of a vector and a scaler
//-----------------------------------------------------------------------------
inline CVECTOR operator*(CVECTOR u, float s)
{
	return CVECTOR(u.x*s, u.y*s,u.z*s);
}

//-----------------------------------------------------------------------------
// Name:
// Desc: divide a vector by a scaler
//-----------------------------------------------------------------------------
inline CVECTOR operator/(CVECTOR u, float s)
{
	return CVECTOR(u.x/s,u.y/s,u.z/s);
}

//-----------------------------------------------------------------------------
// Name:
// Desc: triple scalar product s=u *(v^w)
//-----------------------------------------------------------------------------
inline float TripleScalarProduct(CVECTOR u, CVECTOR v, CVECTOR w)
{
	return float( (u.x*(v.y*w.z - v.z*w.y)) +
				  (u.y*(-v.x*w.z + v.z*w.x)) +
				  (u.z*(v.x*w.y - v.y*w.x)) );
}

//-----------------------------------------------------------------------------
// Name:Rand()
// Desc: randomly pics a number between to ints
//-----------------------------------------------------------------------------
int Rand(int s1, int s2)
{
	return ( (int)(((float)(s2-s1))*((float)rand()/32768.0f)) )+s1;
}

//-----------------------------------------------------------------------------
// Name:Rand()
// Desc: randomly pics a number between to floats
//-----------------------------------------------------------------------------
float Rand(float s1, float s2)
{
	return ( (s2-s1)*((float)rand()/32768.0f) ) + s1;
}