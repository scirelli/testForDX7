//-----------------------------------------------------------------------------
// File: DynamicStarField.h
//
// Desc: 
//
//-----------------------------------------------------------------------------

////INCLUDES/////////////////


#ifndef DynamicSpaceField_h
#define DynamicSpaceField_h

////INCLUDES///////////////////
#include "SCMath.h"
#include "CDDGameApp.h"

////DEFINES/////////////////


////PROTOTYPES//////////////
int DynamicStarField(RECT*, int nNumofStars, CVECTOR velVector, int nFrame);
void CleanUpStarField(CUtilities::PARTICLE *);

#endif