//--------------------------------------------------------
// File: replay.h
//
// Desc: class definitions for replays
//--------------------------------------------------------

#ifndef replay_h
#define replay_h	

////INCLUDES////////////
#include "SCTimer.h"
#include <fstream.h>

////FORWARD/////////////


////CONST///////////////
const int KEY_LOG_SIZE = 10000;

////STRUCTS/////////////
struct KeyStroke
{
	int nKey;
	float fTime;
	int nTetrad;
	KeyStroke *pNext;

	KeyStroke() : nKey(0), fTime(0.f), nTetrad(0),pNext(NULL)
	{ }
	KeyStroke( int key, float fT, int tet )
	{
		nKey = key; fTime = fT;nTetrad = tet;
		pNext=NULL;
	}
	KeyStroke( KeyStroke& key)
	{
		nKey = key.nKey;
		fTime = key.fTime;
		nTetrad = key.nTetrad;
		pNext = key.pNext;
	}
};

////CLASSES/////////////

//------------------------------------------------------
// Name: CReplay
//------------------------------------------------------
class CReplay
{
protected:
	//----Protected member variables----
	KeyStroke    *m_pKeyLogHead;         // keeps  track of user keystrokes
	KeyStroke    *m_pCurrentPos;
	KeyStroke    *m_pKeyLogTail;
	//int          m_nSizeofKeyLog;
	int          m_nKeyStrokeCount;

	CTIMER3 m_TimerBetweenKeyPresses;    //Keeps track of time between keystrokes
	//float   m_fLastKeyPress;
	//float   m_fCurrentKeyPress;
	

	//----Protected methods----

public:
	//----Public methods----
	CReplay()
	{
		//m_nSizeofKeyLog    = 0;
		m_nKeyStrokeCount  = 0;
		//m_fLastKeyPress    = 0.f;
	    //m_fCurrentKeyPress = 0.f;
		m_pKeyLogHead = NULL;
		m_pCurrentPos = NULL;
		m_pKeyLogTail = NULL;
	}

	virtual ~CReplay()
	{
		KeyStroke *pConductor = NULL;
		for( pConductor = m_pKeyLogHead; pConductor != NULL; pConductor = m_pKeyLogHead )
		{
			m_pKeyLogHead = m_pKeyLogHead->pNext;
			delete pConductor;
		}
		
		m_pKeyLogHead = m_pCurrentPos = m_pKeyLogTail = NULL;
		m_nKeyStrokeCount = 0; //m_nSizeofKeyLog = 0;
	}

	void  KeyStrokeLogger( int, int );
	void  KeyLogtoFile( char[], bool );
	void  Clear( );

	//int  getKeyLogSize( )         { return m_nSizeofKeyLog; }
	bool getNextKey( KeyStroke& );
	int  getKeyStrokeCount( )     { return m_nKeyStrokeCount; }
};
#endif