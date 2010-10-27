//////////////////////////////////////////////////////////////////////////////////////////////////////
// File: “CFont.cpp”
//
// Author: Anthony Muccio(AM)
//
// Purpose: Holds the funciton bodies for the CFont , CLetter, CDelay, and CScrolling classes.
//////////////////////////////////////////////////////////////////////////////////////////////////////
#include "PrecompiledHeader.h"

// My Includes
#include "CFont.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////
// Function: “CScrolling”
//////////////////////////////////////////////////////////////////////////////////////////////////////	
CScrolling::CScrolling( int nXScrolling, int nYScrolling  )
{
	m_nXOffset			= 0;			// Holds the amount to offset the X by, for DrawScrolling and DrawScrollingWithDelay
	m_nYOffset			= 0;			// Holds the amount to offset the Y by, for DrawScrolling and DrawScrollingWithDelay
	m_nXScrolling		= nXScrolling;	// X Scrolling direction (-) left (+) right
	m_nYScrolling		= nYScrolling;	// Y Scrolling direction (-) up (+) up
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
// Function: “CDelay”
//////////////////////////////////////////////////////////////////////////////////////////////////////	
CDelay::CDelay( void )
{	
	m_nCounter			= 0;			// The counter to increment for a delay call
	m_fTimer			= 0.0f;			// The timer to increment in update
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
// Function: “CLetter”
//////////////////////////////////////////////////////////////////////////////////////////////////////	
CLetter::CLetter( void )
{	
	m_nX				= 0;			// The starting X position for each letter
	m_nWidth			= 0;			// The width of each letter
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
// Function: “CFont”
//////////////////////////////////////////////////////////////////////////////////////////////////////
CFont::CFont( void )
{	
	m_nImageID			= -1;			// ID for the spritesheet

	// Font - Constants(  m_cLetters will change per spritesheet, but is constant for the rest )
	m_nStartingChar		= 33;			// The starting character value
	m_nCharHeight		= 32;			// The line height so all characters are on the same line

	// Font - Updates
	m_vElapsedTime.clear();				// Holds the time progressed, for DrawWithDelay and DrawScrollingWithDelay	
	m_vScrolling.clear();			// Holds the offset for each scrolling call
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
// Function: “~CFont”
//////////////////////////////////////////////////////////////////////////////////////////////////////
CFont::~CFont( void )
{
	// Release the texture
	ShutdownFont( );
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
// Function: “AddDelay”
//////////////////////////////////////////////////////////////////////////////////////////////////////
int CFont::AddDelay( void )
{
	// Create a delay variable for use in a delay call
	CDelay delay;
	m_vElapsedTime.push_back(delay);

	// Return the delay ID
	return (int)(m_vElapsedTime.size()-1);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
// Function: “AddScrolling”
//////////////////////////////////////////////////////////////////////////////////////////////////////
int CFont::AddScrolling( int nXScrolling, int nYScrolling )
{
	// Create and initialize the direction of a scrolling variable for a scrolling call
	CScrolling scrolling( nXScrolling, nYScrolling );
	m_vScrolling.push_back(scrolling);

	// Return the scrolling ID
	return (int)(m_vScrolling.size()-1);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
// Function: “InitFont”
//////////////////////////////////////////////////////////////////////////////////////////////////////
bool CFont::InitFont( const char* szSpriteSheetFilename, const char* szBinaryFilename )
{

	// Unload the image if there is already an image
	if(m_nImageID != -1 )
	{
		CSGD_TextureManager::GetInstance()->UnloadTexture(m_nImageID);
		m_nImageID =	-1;
	}

	// Load in the sprite sheet
	m_nImageID = CSGD_TextureManager::GetInstance()->LoadTexture( szSpriteSheetFilename );

	// Load in the letter details
	std::ifstream in;
	in.clear();
	
	in.open( szBinaryFilename, std::ios_base::in | std::ios_base::binary);

	if(in.is_open())
	{
		in.read( (char*)&m_cLetters, sizeof(CLetter)*90);
		in.close();

		return true;
	}

	return false;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
// Function: “ShutdownFont”
//////////////////////////////////////////////////////////////////////////////////////////////////////
void CFont::ShutdownFont( void )
{	
	// Unload the image if there is an image
	if(m_nImageID != -1 )
	{
		CSGD_TextureManager::GetInstance()->UnloadTexture(m_nImageID);
		m_nImageID =	-1;
	}

	// Clear the delays and scrollings
	m_vElapsedTime.clear();
	m_vScrolling.clear();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
// Function: “Input”
//////////////////////////////////////////////////////////////////////////////////////////////////////
bool CFont::Input( unsigned char ucKey, float fDeltaTime, float fDeltaDelaySpeed, float fDeltaScrollingSpeed )
{
	CSGD_DirectInput::GetInstance()->ReadDevices();

	if( CSGD_DirectInput::GetInstance()->KeyDown( ucKey ) )
	{
		// Update each delay variable to the newly elapsed time while also multiplying in the percentage increase in speed
		for( unsigned int i = 0; i < m_vElapsedTime.size(); ++i )
			m_vElapsedTime[i].m_fTimer += (fDeltaTime * fDeltaDelaySpeed);

		// Update each scrolling call by the amount of scrolling * the change in time while also multiplying in the percentage increase in speed
		for( unsigned int i = 0; i < m_vScrolling.size(); ++i )
		{
			m_vScrolling[i].m_nXOffset += (int)( m_vScrolling[i].m_nXScrolling * (fDeltaTime * fDeltaScrollingSpeed) );
			m_vScrolling[i].m_nYOffset += (int)( m_vScrolling[i].m_nYScrolling * (fDeltaTime * fDeltaScrollingSpeed) );
		}
	}

	return true;
}
		
//////////////////////////////////////////////////////////////////////////////////////////////////////
// Function: “ChangeScrolling”
//////////////////////////////////////////////////////////////////////////////////////////////////////
void CFont::ChangeScrolling( int nXScrolling, int nYScrolling, int nScrollingID )
{
	m_vScrolling[nScrollingID].m_nXScrolling = nXScrolling;
	m_vScrolling[nScrollingID].m_nYScrolling = nYScrolling;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
// Function: “Update”
//////////////////////////////////////////////////////////////////////////////////////////////////////
void CFont::Update( float fDeltaTime )
{
	// Update each delay variable to the newly elapsed time
	for( unsigned int i = 0; i < m_vElapsedTime.size(); ++i )
		m_vElapsedTime[i].m_fTimer += fDeltaTime;

	// Update each scrolling call by the amount of scrolling * the change in time
	for( unsigned int i = 0; i < m_vScrolling.size(); ++i )
	{
		m_vScrolling[i].m_nXOffset += (int)( m_vScrolling[i].m_nXScrolling * fDeltaTime );
		m_vScrolling[i].m_nYOffset += (int)( m_vScrolling[i].m_nYScrolling * fDeltaTime );
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
// Function: “Draw”
//////////////////////////////////////////////////////////////////////////////////////////////////////
void CFont::Draw( const char* szText, int nX, int nY, float fScale, DWORD dwColor )
{
	// The start of each line
	int nOrigX = nX;

	//	Go through the string 1 char at a time
	int nLength = strlen(szText);

	for ( int i = 0; i < nLength; i++ )
	{
		// 	Get the char
		char ch = szText[i];

		//	check for special chars
		if ( ch == ' ' )
		{
			// Increment the x position for the next letter
			nX += (int)( 6 * fScale );
			continue;
		}
		else if ( ch == '\n' )
		{
			// Reset the x to the start of the line and increment the y position for the next line
			nX = nOrigX;
			nY += (int)(m_nCharHeight * fScale);
			continue;
		}

		//	Make the char an ID
		int nId = ch - m_nStartingChar;

		// Rect on the sprite sheet
		RECT rLetter = { m_cLetters[nId].m_nX, 0, rLetter.left + m_cLetters[nId].m_nWidth, rLetter.top + m_nCharHeight };

		// Render the rect
		CSGD_TextureManager::GetInstance()->Draw( m_nImageID, nX, nY,
			fScale, fScale, &rLetter, 0, 0, 0, dwColor );

		// Increment the x position for the next letter
		nX += (int)( (m_cLetters[nId].m_nWidth + 3) * fScale );
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
// Function: “DrawWithDelay”
//////////////////////////////////////////////////////////////////////////////////////////////////////
void CFont::DrawWithDelay( const char* szText,  int nX, int nY, float fScale, DWORD dwColor, float fDelayTime,
						  int nDelayID )
{
	// If the timer has reached its delay time, increment the counter
	if( m_vElapsedTime[nDelayID].m_fTimer >= fDelayTime )
	{
		// Set Time to 0, update the counter
		m_vElapsedTime[nDelayID].m_fTimer = 0.0f;
		m_vElapsedTime[nDelayID].m_nCounter++;
	}

	//	Go through the string 1 char at a time
	int nLength = strlen(szText);

	// If the counter is greater than the length, set it to the length
	if(m_vElapsedTime[nDelayID].m_nCounter >= nLength )
		m_vElapsedTime[nDelayID].m_nCounter = nLength;

	// Keep track of the initial x
	int nOrigX = nX;

	for ( int i = 0; i < m_vElapsedTime[nDelayID].m_nCounter; i++ )
	{
		// 	Get the char
		char ch = szText[i];

		//	check for special chars
		if ( ch == ' ' )
		{
			// Increment the x position for the next letter
			nX += (int)( 6 * fScale );
			continue;
		}
		else if ( ch == '\n' )
		{
			// Reset the x position to the start of the line and increment the y position for the next line
			nX = nOrigX;
			nY += (int)(m_nCharHeight * fScale);
			continue;
		}

		//	Make the char an ID
		int nId = ch - m_nStartingChar;

		// Rect on the sprite sheet
		RECT rLetter = { m_cLetters[nId].m_nX, 0, rLetter.left + m_cLetters[nId].m_nWidth, rLetter.top + m_nCharHeight };

		// Render the rect
		CSGD_TextureManager::GetInstance()->Draw( m_nImageID, nX, nY,
			fScale, fScale, &rLetter, 0, 0, 0, dwColor );

		// Increment the x position for the next letter
		nX += (int)( (m_cLetters[nId].m_nWidth + 3) * fScale );
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
// Function: “DrawScrolling”
//////////////////////////////////////////////////////////////////////////////////////////////////////
void CFont::DrawScrolling( const char* szText, int nX, int nY, float fScale, DWORD dwColor,
						  int nMinDrawHeight, int nMaxDrawHeight, int nMinDrawWidth, int nMaxDrawWidth,
						  int nScrollingID )
{
	// The starting position of each line
	int nOrigX = nX;

	//	Go through the string 1 char at a time
	int nLength = strlen(szText);


	for ( int i = 0; i < nLength; i++ )
	{
		// 	Get the char
		char ch = szText[i];

		//	check for special chars
		if ( ch == ' ' )
		{
			// Increment the x position for the next letter
			nX += (int)( 6 * fScale );
			continue;
		}
		else if ( ch == '\n' )
		{
			// Reset the x position to the start of the line and increment the y position for the next line
			nX = nOrigX;
			nY += (int)(m_nCharHeight * fScale);
			continue;
		}

		//	Make the char an ID
		int nId = ch - m_nStartingChar;

		// Rect for the sprite sheet
		RECT rLetter = { m_cLetters[nId].m_nX, 0, rLetter.left + m_cLetters[nId].m_nWidth, rLetter.top + m_nCharHeight };

		// If the position is inside the boundaries render the rect
		int nXPosition =  nX + m_vScrolling[nScrollingID].m_nXOffset;
		int nYPosition =  nY + m_vScrolling[nScrollingID].m_nYOffset;
		if( nXPosition > nMinDrawWidth && nXPosition < nMaxDrawWidth && nYPosition > nMinDrawHeight && nYPosition < nMaxDrawHeight )		
			CSGD_TextureManager::GetInstance()->Draw( m_nImageID, nX + m_vScrolling[nScrollingID].m_nXOffset, nY + m_vScrolling[nScrollingID].m_nYOffset,
				fScale, fScale, &rLetter, 0, 0, 0, dwColor );

		// Increment the x position for the next letter
		nX += (int)( (m_cLetters[nId].m_nWidth + 3) * fScale );
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
// Function: “DrawScrollingWithDelay” 
//////////////////////////////////////////////////////////////////////////////////////////////////////
void CFont::DrawScrollingWithDelay( const char* szText, int nX, int nY, float fScale, DWORD dwColor,
								   int nMinDrawHeight, int nMaxDrawHeight, int nMinDrawWidth, int nMaxDrawWidth, float fDelayTime,
								   int nScrollingID, int nDelayID )
{
	// If the timer has reached its delay time, increment the counter
	if( m_vElapsedTime[nDelayID].m_fTimer >= fDelayTime )
	{
		// Update the counter and set elapsed time to 0
		m_vElapsedTime[nDelayID].m_fTimer = 0.0f;
		m_vElapsedTime[nDelayID].m_nCounter++;
	}

	//	Go through the string 1 char at a time
	int nLength = strlen(szText);

	// If the counter is greater than the length, set it to the length
	if( m_vElapsedTime[nDelayID].m_nCounter >= nLength )
		m_vElapsedTime[nDelayID].m_nCounter = nLength;

	// Keep track of the initial x
	int nOrigX = nX;

	for ( int i = 0; i < m_vElapsedTime[nDelayID].m_nCounter; i++ )
	{
		// 	Get the char
		char ch = szText[i];

		//	check for special chars
		if ( ch == ' ' )
		{
			// Increment the x position for the next letter
			nX += (int)( 6 * fScale );
			continue;
		}
		else if ( ch == '\n' )
		{

			// Reset the x position to the start of the line and increment the y position for the next line
			nX = nOrigX;
			nY += (int)(m_nCharHeight * fScale);
			continue;
		}

		//	Make the char an ID
		int nId = ch - m_nStartingChar;

		// Rect for the sprite sheet
		RECT rLetter = { m_cLetters[nId].m_nX, 0, rLetter.left + m_cLetters[nId].m_nWidth, rLetter.top + m_nCharHeight };

		// If the position is inside the boundaries render the rect
		int nXPosition =  nX + m_vScrolling[nScrollingID].m_nXOffset;
		int nYPosition =  nY + m_vScrolling[nScrollingID].m_nYOffset;
		if( nXPosition > nMinDrawWidth && nXPosition < nMaxDrawWidth && nYPosition > nMinDrawHeight && nYPosition < nMaxDrawHeight )
			CSGD_TextureManager::GetInstance()->Draw( m_nImageID, nXPosition, nYPosition,
				fScale, fScale, &rLetter, 0, 0, 0, dwColor );

		// Increment the x position for the next letter
		nX += (int)( (m_cLetters[nId].m_nWidth + 3) * fScale );
	}
}