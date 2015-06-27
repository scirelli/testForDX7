//--------------------------------------------------------
// File: CReplay.cpp
//
// Desc: Impl. of the CReplay class
//--------------------------------------------------------

////INCLUDES///////////
#include "Replay.h"

//-----------------------------------------------------------------
// CReplay class Impl
//-----------------------------------------------------------------
////METHODS/////////////

//-----------------------------------------------------------------
// Name: KeyStrikeLogger()            virtual
// Desc: Used to log keys for things like replays or AI
//-----------------------------------------------------------------
void CReplay::KeyStrokeLogger( int key, int tet )
{
	//add first keystroke
	if( m_pKeyLogHead == NULL )
	{
		m_pKeyLogHead = new KeyStroke(key, 0, tet);
		m_pCurrentPos = m_pKeyLogHead;
	    m_pKeyLogTail = m_pKeyLogHead;
	    m_nKeyStrokeCount = 1;
		//m_fLastKeyPress = 0;
	    //m_fCurrentKeyPress = 0;
		m_TimerBetweenKeyPresses.Tick();
		return;
	}
	
	//add on a new Keystroke to the end of the list
	m_pKeyLogTail->pNext = new KeyStroke();
	//alias the pointer
	KeyStroke *temp = m_pKeyLogTail->pNext;
	
	temp->nKey  = key;
	temp->nTetrad = tet;
	temp->fTime = m_TimerBetweenKeyPresses.GetTimeElapsed();
	m_TimerBetweenKeyPresses.Tick(); //start the timer again

	//advance the tail to end of list
	m_pKeyLogTail = m_pKeyLogTail->pNext;
}

//-----------------------------------------------------------------
// Name: KeyLogtoFile()            
// Desc: wrties the key log to a file
//-----------------------------------------------------------------
void CReplay::KeyLogtoFile( char cpFileName[], bool nClear = true )
{
/*
	ofstream outFile(cpFileName);
	for( int i=0; i<m_nKeyStrokeCount; i++)
		outFile << m_npKeyLog[ i ] << endl;
	outFile.close();
	if( nClear )
	{
		memset((void*)m_npKeyLog,0,m_nSizeofKeyLog);
		m_nKeyStrokeCount = 0;
	}
*/
}

//-----------------------------------------------------------------
// Name: Clear()            
// Desc: clears the list of keys
//-----------------------------------------------------------------
void CReplay::Clear( )
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
//-----------------------------------------------------------------
// Name: getNextKey( )
// Desc: returns a copy of the next key in OutputKey
//       returns true if there's another key on list
//-----------------------------------------------------------------
bool CReplay::getNextKey( KeyStroke &OutputKey )
{ 
	OutputKey = *m_pCurrentPos;
	OutputKey.pNext = NULL;
	if(m_pCurrentPos->pNext)
		m_pCurrentPos = m_pCurrentPos->pNext;
	else
		return false;
	return true;
}