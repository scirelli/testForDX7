//-----------------------------------------------------------------------------
// File: ErrorCheck.cpp
//
// Desc: Common file for all the error checkings I do
//
//-----------------------------------------------------------------------------
#include <windows.h>
#include <ddraw.h>
#include <dinput.h>
#include <dsound.h>
#include <stdio.h>


//-----------------------------------------------------------------------------
// Name: DDErrorString()
// Desc: All the errors that DD can come back with
//-----------------------------------------------------------------------------
char *DDErrorString(HRESULT hr)
{
	switch (hr)
	{
	case DDERR_ALREADYINITIALIZED:           return "DDERR_ALREADYINITIALIZED";
	case DDERR_CANNOTATTACHSURFACE:          return "DDERR_CANNOTATTACHSURFACE";
	case DDERR_CANNOTDETACHSURFACE:          return "DDERR_CANNOTDETACHSURFACE";
	case DDERR_CURRENTLYNOTAVAIL:            return "DDERR_CURRENTLYNOTAVAIL";
	case DDERR_EXCEPTION:                    return "DDERR_EXCEPTION";
	case DDERR_GENERIC:                      return "DDERR_GENERIC";
	case DDERR_HEIGHTALIGN:                  return "DDERR_HEIGHTALIGN";
	case DDERR_INCOMPATIBLEPRIMARY:          return "DDERR_INCOMPATIBLEPRIMARY";
	case DDERR_INVALIDCAPS:                  return "DDERR_INVALIDCAPS";
	case DDERR_INVALIDCLIPLIST:              return "DDERR_INVALIDCLIPLIST";
	case DDERR_INVALIDMODE:                  return "DDERR_INVALIDMODE";
	case DDERR_INVALIDOBJECT:                return "DDERR_INVALIDOBJECT";
	case DDERR_INVALIDPARAMS:                return "DDERR_INVALIDPARAMS";
	case DDERR_INVALIDPIXELFORMAT:           return "DDERR_INVALIDPIXELFORMAT";
	case DDERR_INVALIDRECT:                  return "DDERR_INVALIDRECT";
	case DDERR_LOCKEDSURFACES:               return "DDERR_LOCKEDSURFACES";
	case DDERR_NO3D:                         return "DDERR_NO3D";
	case DDERR_NOALPHAHW:                    return "DDERR_NOALPHAHW";
	case DDERR_NOCLIPLIST:                   return "DDERR_NOCLIPLIST";
	case DDERR_NOCOLORCONVHW:                return "DDERR_NOCOLORCONVHW";
	case DDERR_NOCOOPERATIVELEVELSET:        return "DDERR_NOCOOPERATIVELEVELSET";
	case DDERR_NOCOLORKEY:                   return "DDERR_NOCOLORKEY";
	case DDERR_NOCOLORKEYHW:                 return "DDERR_NOCOLORKEYHW";
	case DDERR_NODIRECTDRAWSUPPORT:          return "DDERR_NODIRECTDRAWSUPPORT";
	case DDERR_NOEXCLUSIVEMODE:              return "DDERR_NOEXCLUSIVEMODE";
	case DDERR_NOFLIPHW:                     return "DDERR_NOFLIPHW";
	case DDERR_NOGDI:                        return "DDERR_NOGDI";
	case DDERR_NOMIRRORHW:                   return "DDERR_NOMIRRORHW";
	case DDERR_NOTFOUND:                     return "DDERR_NOTFOUND";
	case DDERR_NOOVERLAYHW:                  return "DDERR_NOOVERLAYHW";
	case DDERR_NORASTEROPHW:                 return "DDERR_NORASTEROPHW";
	case DDERR_NOROTATIONHW:                 return "DDERR_NOROTATIONHW";
	case DDERR_NOSTRETCHHW:                  return "DDERR_NOSTRETCHHW";
	case DDERR_NOT4BITCOLOR:                 return "DDERR_NOT4BITCOLOR";
	case DDERR_NOT4BITCOLORINDEX:            return "DDERR_NOT4BITCOLORINDEX";
	case DDERR_NOT8BITCOLOR:                 return "DDERR_NOT8BITCOLOR";
	case DDERR_NOTEXTUREHW:                  return "DDERR_NOTEXTUREHW";
	case DDERR_NOVSYNCHW:                    return "DDERR_NOVSYNCHW";
	case DDERR_NOZBUFFERHW:                  return "DDERR_NOZBUFFERHW";
	case DDERR_NOZOVERLAYHW:                 return "DDERR_NOZOVERLAYHW";
	case DDERR_OUTOFCAPS:                    return "DDERR_OUTOFCAPS";
	case DDERR_OUTOFMEMORY:                  return "DDERR_OUTOFMEMORY";
	case DDERR_OUTOFVIDEOMEMORY:             return "DDERR_OUTOFVIDEOMEMORY";
	case DDERR_OVERLAYCANTCLIP:              return "DDERR_OVERLAYCANTCLIP";
	case DDERR_OVERLAYCOLORKEYONLYONEACTIVE: return "DDERR_OVERLAYCOLORKEYONLYONEACTIVE";
	case DDERR_PALETTEBUSY:                  return "DDERR_PALETTEBUSY";
	case DDERR_COLORKEYNOTSET:               return "DDERR_COLORKEYNOTSET";
	case DDERR_SURFACEALREADYATTACHED:       return "DDERR_SURFACEALREADYATTACHED";
	case DDERR_SURFACEALREADYDEPENDENT:      return "DDERR_SURFACEALREADYDEPENDENT";
	case DDERR_SURFACEBUSY:                  return "DDERR_SURFACEBUSY";
	case DDERR_CANTLOCKSURFACE:              return "DDERR_CANTLOCKSURFACE";
	case DDERR_SURFACEISOBSCURED:            return "DDERR_SURFACEISOBSCURED";
	case DDERR_SURFACELOST:                  return "DDERR_SURFACELOST";
	case DDERR_SURFACENOTATTACHED:           return "DDERR_SURFACENOTATTACHED";
	case DDERR_TOOBIGHEIGHT:                 return "DDERR_TOOBIGHEIGHT";
	case DDERR_TOOBIGSIZE:                   return "DDERR_TOOBIGSIZE";
	case DDERR_TOOBIGWIDTH:                  return "DDERR_TOOBIGWIDTH";
	case DDERR_UNSUPPORTED:                  return "DDERR_UNSUPPORTED";
	case DDERR_UNSUPPORTEDFORMAT:            return "DDERR_UNSUPPORTEDFORMAT";
	case DDERR_UNSUPPORTEDMASK:              return "DDERR_UNSUPPORTEDMASK";
	case DDERR_VERTICALBLANKINPROGRESS:      return "DDERR_VERTICALBLANKINPROGRESS";
	case DDERR_WASSTILLDRAWING:              return "DDERR_WASSTILLDRAWING";
	case DDERR_XALIGN:                       return "DDERR_XALIGN";
	case DDERR_INVALIDDIRECTDRAWGUID:        return "DDERR_INVALIDDIRECTDRAWGUID";
	case DDERR_DIRECTDRAWALREADYCREATED:     return "DDERR_DIRECTDRAWALREADYCREATED";
	case DDERR_NODIRECTDRAWHW:               return "DDERR_NODIRECTDRAWHW";
	case DDERR_PRIMARYSURFACEALREADYEXISTS:  return "DDERR_PRIMARYSURFACEALREADYEXISTS";
	case DDERR_NOEMULATION:                  return "DDERR_NOEMULATION";
	case DDERR_REGIONTOOSMALL:               return "DDERR_REGIONTOOSMALL";
	case DDERR_CLIPPERISUSINGHWND:           return "DDERR_CLIPPERISUSINGHWND";
	case DDERR_NOCLIPPERATTACHED:            return "DDERR_NOCLIPPERATTACHED";
	case DDERR_NOHWND:                       return "DDERR_NOHWND";
	case DDERR_HWNDSUBCLASSED:               return "DDERR_HWNDSUBCLASSED";
	case DDERR_HWNDALREADYSET:               return "DDERR_HWNDALREADYSET";
	case DDERR_NOPALETTEATTACHED:            return "DDERR_NOPALETTEATTACHED";
	case DDERR_NOPALETTEHW:                  return "DDERR_NOPALETTEHW";
	case DDERR_BLTFASTCANTCLIP:              return "DDERR_BLTFASTCANTCLIP";
	case DDERR_NOBLTHW:                      return "DDERR_NOBLTHW";
	case DDERR_NODDROPSHW:                   return "DDERR_NODDROPSHW";
	case DDERR_OVERLAYNOTVISIBLE:            return "DDERR_OVERLAYNOTVISIBLE";
	case DDERR_NOOVERLAYDEST:                return "DDERR_NOOVERLAYDEST";
	case DDERR_INVALIDPOSITION:              return "DDERR_INVALIDPOSITION";
	case DDERR_NOTAOVERLAYSURFACE:           return "DDERR_NOTAOVERLAYSURFACE";
	case DDERR_EXCLUSIVEMODEALREADYSET:      return "DDERR_EXCLUSIVEMODEALREADYSET";
	case DDERR_NOTFLIPPABLE:                 return "DDERR_NOTFLIPPABLE";
	case DDERR_CANTDUPLICATE:                return "DDERR_CANTDUPLICATE";
	case DDERR_NOTLOCKED:                    return "DDERR_NOTLOCKED";
	case DDERR_CANTCREATEDC:                 return "DDERR_CANTCREATEDC";
	case DDERR_NODC:                         return "DDERR_NODC";
	case DDERR_WRONGMODE:                    return "DDERR_WRONGMODE";
	case DDERR_IMPLICITLYCREATED:            return "DDERR_IMPLICITLYCREATED";
	case DDERR_NOTPALETTIZED:                return "DDERR_NOTPALETTIZED";
	case DDERR_UNSUPPORTEDMODE:              return "DDERR_UNSUPPORTEDMODE";
	case DDERR_NOMIPMAPHW:                   return "DDERR_NOMIPMAPHW";
	case DDERR_INVALIDSURFACETYPE:           return "DDERR_INVALIDSURFACETYPE";
	case DDERR_DCALREADYCREATED:             return "DDERR_DCALREADYCREATED";
	case DDERR_CANTPAGELOCK:                 return "DDERR_CANTPAGELOCK";
	case DDERR_CANTPAGEUNLOCK:               return "DDERR_CANTPAGEUNLOCK";
	case DDERR_NOTPAGELOCKED:                return "DDERR_NOTPAGELOCKED";
	case DDERR_NOTINITIALIZED:               return "DDERR_NOTINITIALIZED";
	
	}
	return "Unknown Error";
}
//-----------------------------------------------------------------------------
// Name: DIErrorString()
// Desc: All the errors that DI can come back with
//-----------------------------------------------------------------------------
char *DIErrorString(HRESULT hr)
{
	switch (hr)
	{
	//----Direct Input Errors----
	case DI_BUFFEROVERFLOW:					 return "DI_BUFFEROVERFLOW";
	case DI_DOWNLOADSKIPPED:                 return "DI_DOWNLOADSKIPPED";
	case DI_EFFECTRESTARTED:				 return "DI_EFFECTRESTARTED";
//	case DI_NOEFFECT:						 return "DI_NOEFFECT";
//	case DI_NOTATTACHED:					 return "DI_NOTATTACHED";
	//case DI_OK:								 return "DI_OK";
	case DI_POLLEDDEVICE:					 return "DI_POLLEDDEVICE";
//	case DI_PROPNOEFFECT:					 return "DI_PROPNOEFFECT";
	case DI_SETTINGSNOTSAVED:				 return "DI_SETTINGSNOTSAVED";
	case DI_TRUNCATED:						 return "DI_TRUNCATED";
	case DI_TRUNCATEDANDRESTARTED:			 return "DI_TRUNCATEDANDRESTARTED";
	case DI_WRITEPROTECT:					 return "DI_WRITEPROTECT";
	case DIERR_ACQUIRED:					 return "DIERR_ACQUIRED";
	case DIERR_ALREADYINITIALIZED:			 return "DIERR_ALREADYINITIALIZED";
	case DIERR_BADDRIVERVER:				 return "DIERR_BADDRIVERVER";
	case DIERR_BETADIRECTINPUTVERSION:		 return "DIERR_BETADIRECTINPUTVERSION";
	case DIERR_DEVICEFULL:					 return "DIERR_DEVICEFULL";
	case DIERR_DEVICENOTREG:				 return "DIERR_DEVICENOTREG";
	case DIERR_EFFECTPLAYING:				 return "DIERR_EFFECTPLAYING";
	case DIERR_GENERIC:						 return "DIERR_GENERIC";
	case DIERR_HANDLEEXISTS:				 return "DIERR_HANDLEEXISTS";
	case DIERR_HASEFFECTS:					 return "DIERR_HASEFFECTS";
	case DIERR_INCOMPLETEEFFECT:			 return "DIERR_INCOMPLETEEFFECT";
	case DIERR_INPUTLOST:					 return "DIERR_INPUTLOST";
	case DIERR_INVALIDPARAM:				 return "DIERR_INVALIDPARAM";
	case DIERR_MAPFILEFAIL:					 return "DIERR_MAPFILEFAIL";
	case DIERR_MOREDATA:					 return "DIERR_MOREDATA";
	case DIERR_NOAGGREGATION:				 return "DIERR_NOAGGREGATION";
	case DIERR_NOINTERFACE:					 return "DIERR_NOINTERFACE";
	case DIERR_NOTACQUIRED:					 return "DIERR_NOTACQUIRED";
	case DIERR_NOTBUFFERED:					 return "DIERR_NOTBUFFERED";
	case DIERR_NOTDOWNLOADED:				 return "DIERR_NOTDOWNLOADED";
	case DIERR_NOTEXCLUSIVEACQUIRED:		 return "DIERR_NOTEXCLUSIVEACQUIRED";
	case DIERR_NOTFOUND:					 return "DIERR_NOTFOUND";
	case DIERR_NOTINITIALIZED:				 return "DIERR_NOTINITIALIZED";
//	case DIERR_OBJECTNOTFOUND:				 return "DIERR_OBJECTNOTFOUND";
	case DIERR_OLDDIRECTINPUTVERSION:		 return "DIERR_OLDDIRECTINPUTVERSION";
//	case DIERR_OTHERAPPHASPRIO:				 return "DIERR_OTHERAPPHASPRIO";
	case DIERR_OUTOFMEMORY:					 return "DIERR_OUTOFMEMORY";
//	case DIERR_READONLY:					 return "DIERR_READONLY";
	case DIERR_REPORTFULL:					 return "DIERR_REPORTFULL";
	case DIERR_UNPLUGGED:					 return "DIERR_UNPLUGGED";
	case DIERR_UNSUPPORTED:					 return "DIERR_UNSUPPORTED";
	case E_HANDLE:							 return "E_HANDLE";
	case E_PENDING:							 return "E_PENDING";
	case E_POINTER:							 return "E_POINTER";
	
	}
	return "Unknown Error";
}

//-----------------------------------------------------------------------------
// Name: DSErrorString()
// Desc: All the errors that DS can come back with
//-----------------------------------------------------------------------------
char *DSErrorString(HRESULT hr)
{
	switch (hr)
	{
	//----Direct Sound return values----
	case DS_NO_VIRTUALIZATION:				 return "DS_NO_VIRTUALIZATION";
	//case DS_INCOMPLETE:						 return "DS_INCOMPLETE";
	case DSERR_ACCESSDENIED:				 return "DSERR_ACCESSDENIED";
	case DSERR_ALLOCATED:					 return "DSERR_ALLOCATED";
	case DSERR_ALREADYINITIALIZED:			 return "DSERR_ALREADYINITIALIZED";
	case DSERR_BADFORMAT:					 return "DSERR_BADFORMAT";
	case DSERR_BADSENDBUFFERGUID:			 return "DSERR_BADSENDBUFFERGUID";
	case DSERR_BUFFERLOST:					 return "DSERR_BUFFERLOST";
	case DSERR_BUFFERTOOSMALL:				 return "DSERR_BUFFERTOOSMALL";
	case DSERR_CONTROLUNAVAIL:				 return "DSERR_CONTROLUNAVAIL";
	case DSERR_DS8_REQUIRED:				 return "DSERR_DS8_REQUIRED";
	case DSERR_FXUNAVAILABLE:				 return "DSERR_FXUNAVAILABLE";
	case DSERR_GENERIC:						 return "DSERR_GENERIC";
	case DSERR_INVALIDCALL:					 return "DSERR_INVALIDCALL";
	case DSERR_INVALIDPARAM:				 return "DSERR_INVALIDPARAM";
	case DSERR_NOAGGREGATION:				 return "DSERR_NOAGGREGATION";
	case DSERR_NODRIVER:					 return "DSERR_NODRIVER";
	case DSERR_NOINTERFACE:					 return "DSERR_NOINTERFACE";
	case DSERR_OBJECTNOTFOUND:				 return "DSERR_OBJECTNOTFOUND";
	case DSERR_OTHERAPPHASPRIO:				 return "DSERR_OTHERAPPHASPRIO";
	case DSERR_OUTOFMEMORY:					 return "DSERR_OUTOFMEMORY";
	case DSERR_PRIOLEVELNEEDED:				 return "DSERR_PRIOLEVELNEEDED";
	case DSERR_SENDLOOP:					 return "DSERR_SENDLOOP";
	case DSERR_UNINITIALIZED:				 return "DSERR_UNINITIALIZED";
	case DSERR_UNSUPPORTED:					 return "DSERR_UNSUPPORTED";
	
	}
	return "Unknown Error";
}

//-----------------------------------------------------------------------------
// Name: DDFailedCheck()
// Desc: Checks to see if DD gave and error
//-----------------------------------------------------------------------------
bool DDFailedCheck(HRESULT hr, char *szMessage, char * cpErrorBuf)
{
	if (FAILED(hr))
	{
		char buf[1024];
		sprintf( buf, "%s (%s)\n", szMessage, DDErrorString(hr) );
		OutputDebugString( buf );
		sprintf( cpErrorBuf, "%s (%s)\n", szMessage, DDErrorString(hr) );
		return true;
	}
	return false;
}
//-----------------------------------------------------------------------------
// Name: DIFailedCheck()
// Desc: Checks to see if DI gave and error
//-----------------------------------------------------------------------------
bool DIFailedCheck(HRESULT hr, char *szMessage, char * cpErrorBuf)
{
	if (FAILED(hr))
	{
		char buf[1024];
		sprintf( buf, "%s (%s)\n", szMessage, DIErrorString(hr) );
		OutputDebugString( buf );
		sprintf( cpErrorBuf, "%s (%s)\n", szMessage, DIErrorString(hr) );
		return true;
	}
	return false;
}
//-----------------------------------------------------------------------------
// Name: DSFailedCheck()
// Desc: Checks to see if DS gave and error
//-----------------------------------------------------------------------------
bool DSFailedCheck(HRESULT hr, char *szMessage, char * cpErrorBuf)
{
	if (FAILED(hr))
	{
		char buf[1024];
		sprintf( buf, "%s (%s)\n", szMessage, DSErrorString(hr) );
		OutputDebugString( buf );
		sprintf( cpErrorBuf, "%s (%s)\n", szMessage, DSErrorString(hr) );
		return true;
	}
	return false;
}