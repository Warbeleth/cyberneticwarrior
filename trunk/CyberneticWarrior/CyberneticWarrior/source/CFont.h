//////////////////////////////////////////////////////////////////////////////////////////////////////
// File: “CFont.h”
//
// Author: Anthony Muccio(AM)
//
// Purpose: This header primaraly holds the CFont class, however it also holds the CDelay, CScrolling,
//			and CLetter classes.
//			CFont		- The class used to create fonts, supports kerning, scrolling, and typewriter
//							rendering.
//			CLetter		- The class used inside of CFont that holds the positions and widths of each
//							letter and symbol on the font spritesheet.5
//			CDelay		- The class used inside of CFont that is required for each "Delay" render call.
//			CScrolling	- The class used inside of CFont that is required for each "Scrolling" render
//							call.
//
//			ALL LETTERS AND SYMBOLS MUST BE ON ONE LINE AND DEFAULT STARTING CHAR BE !
//////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _CFONT_H_
#define _CFONT_H_

#include <windows.h>
// Might be replaced for speed purposes. . .

#include <vector>

//////////////////////////////////////////////////////////////////////////////////////////////////////
//						******************* CLASS - CDelay *******************
//////////////////////////////////////////////////////////////////////////////////////////////////////
class CDelay
{
public:
	int		m_nCounter;			// The counter to increment for a delay call
	float	m_fTimer;			// The timer to increment in update

	//////////////////////////////////////////////////////////////////////////////////////////////////////
	// Function: “CDelay”
	//
	// Purpose: The default constructor for CDelay. Will initialize values.
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	CDelay( void );
};



//////////////////////////////////////////////////////////////////////////////////////////////////////
//						******************* CLASS - CScrolling *******************
//////////////////////////////////////////////////////////////////////////////////////////////////////
class CScrolling
{
public:
	int			m_nXOffset;		// Holds the amount to offset the X by, for DrawScrolling and DrawScrollingWithDelay
	int			m_nYOffset;		// Holds the amount to offset the Y by, for DrawScrolling and DrawScrollingWithDelay
	int			m_nXScrolling;	// X Scrolling direction (-) left (+) right
	int			m_nYScrolling;	// Y Scrolling direction (-) up (+) up

	////////////////////////////////////////////?//////////////////////////////////////////////////////////
	// Function: “CScrolling”
	//
	// Purpose: The constructor for CScrolling. Will initialize values and set the scrolling direction.
	//			nXScrolling - For setting X scroll direction
	//			nYScrolling - For setting Y scroll direction
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	CScrolling( int nXScrolling, int nYScrolling );
};



//////////////////////////////////////////////////////////////////////////////////////////////////////
//						******************* CLASS - CLetter *******************
//////////////////////////////////////////////////////////////////////////////////////////////////////
class CLetter
{
public:
	int		m_nX;				// The starting X position for each letter
	int		m_nWidth;			// The width of each letter

	//////////////////////////////////////////////////////////////////////////////////////////////////////
	// Function: “CLetter”
	//
	// Purpose: The default constructor for CLetter. Will initialize values.
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	CLetter( void );
};



//////////////////////////////////////////////////////////////////////////////////////////////////////
//						******************* CLASS - CFont *******************
//////////////////////////////////////////////////////////////////////////////////////////////////////
class CFont
{
private:
	int					m_nImageID;				// ID for the spritesheet

	// Font - Constants(  m_cLetters will change per spritesheet, but is constant for the rest )
	CLetter				m_cLetters[90];			// Holds the positions and widths of each letter
	int					m_nStartingChar;		// The starting character value
	int					m_nCharHeight;			// The line height so all characters are on the same line

	// Font - Updates
	std::vector<CDelay>		m_vElapsedTime;			// Holds the time progressed, for DrawWithDelay and DrawScrollingWithDelay
	std::vector<CScrolling>	m_vScrolling;			// Holds the offset for each scrolling call

	//////////////////////////////////////////////////////////////////////////////////////////////////////
	// Function: “CFont”
	//
	// Purpose: The copy constructor for CFont. This does nothing and has an empty body.
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	CFont( CFont& );

	//////////////////////////////////////////////////////////////////////////////////////////////////////
	// Function: “CFont& operator=”
	//
	// Purpose: The assignment operator does nothing and has an empty body.
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	CFont& operator=( CFont& );

public:
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	// Function: “CFont”
	//
	// Purpose: The default constructor for CFont. Will initialize values.
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	CFont( void );

	//////////////////////////////////////////////////////////////////////////////////////////////////////
	// Function: “~CFont”
	//
	// Purpose: The destructor for CFont. Will clean up the ImageID.
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	~CFont( void );

	//////////////////////////////////////////////////////////////////////////////////////////////////////
	// Function: “AddDelay”
	//
	// Purpose: This delay variable is necessary for each call to a delay function.
	//
	//			RETURNS - ID to the delay variable
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	int AddDelay( void );

	//////////////////////////////////////////////////////////////////////////////////////////////////////
	// Function: “AddScrolling”
	//
	// Purpose: This scrolling variable is necessary for each call to the scrolling functions.
	//			nXScrolling - For setting X scroll direction
	//			nYScrolling - for setting Y scroll direction
	//
	//			RETURNS - ID to the scrolling variable
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	int AddScrolling( int nXScrolling, int nYScrolling );

	//////////////////////////////////////////////////////////////////////////////////////////////////////
	// Function: “InitFont”
	//
	// Purpose: This will take in two strings, one for the sprite sheet and the other for the binary file
	//			which holds the widths/positions of the letters. This function will unload a texture if
	//			the image ID is pre-existing to another spritesheet. The binary file must contain data for
	//			180 intergers. This is the 90 positions and 90 widths for the 90 symbols.
	//			szSpriteSheetFilename - The filename for the image the font will be using
	//			szBinaryFilename - The filename for the binary file holding each letter and symbol's 
	//							   position and width.
	//			
	//			RETURNS - (true) if it succeeded, (false) if it has failed to open the file
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	bool InitFont( const char* szSpriteSheetFilename, const char* szBinaryFilename );

	//////////////////////////////////////////////////////////////////////////////////////////////////////
	// Function: “ShutdownFont”
	//
	// Purpose: This function will unload the texture being saved by the image ID and clear any delay or
	//			scrolling variables.  This will essentially start the font variable out fresh and will
	//			need to be reinitialized.
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	void ShutdownFont( void );

	//////////////////////////////////////////////////////////////////////////////////////////////////////
	// Function: “Input”
	//
	// Purpose: This function takes in a direct input key value and uses that key to check whether the user
	//			has decided to "speed up" the delay / scrolling renders.
	//			ucKey - The key passed in that determines whether the user wants to "speed up" the delay / 
	//					scrolling renders
	//			fDeltaTime - The elapsed time since the last call of this function
	//			fDeltaDelaySpeed - This is a percentage based speed increase, if fDeltaDelaySpeed = 1 then
	//							   the speed will remain the same
	//			fDeltaScrollingSpeed - This is a percentage based speed increase, if fDeltaScrollingspeed
	//								   = 1 then the speed will remain the same
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	bool Input( unsigned char ucKey, float fDeltaTime, float fDeltaDelaySpeed, float fDeltaScrollingSpeed );

	//////////////////////////////////////////////////////////////////////////////////////////////////////
	// Function: “Update”
	//
	// Purpose: Update will take care of updating the elapsed time of each delay variable and will update
	//			each scrolling variable.
	//			fDeltaTime - The elapsed time since the last call of this function
	//
	//			NECESSARY FOR ANY DELAY OR SCROLLING CALLS
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	void Update( float fDeltaTime );

	//////////////////////////////////////////////////////////////////////////////////////////////////////
	// Function: “Draw”
	//
	// Purpose: Draw takes in a string and does a normal render to the screen at the specified position, 
	//			scale and color.
	//			szText - The string to be displayed to the screen
	//			nX - X position
	//			nY - Y position
	//			fScale - Scale of the image being displayed, 1 being the normal size, less than one being
	//					 smaller, greater than 1 being larger
	//			dwColor - The color to shade the image being displayed
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	void Draw( const char* szText, int nX, int nY, float fScale, DWORD dwColor );

	//////////////////////////////////////////////////////////////////////////////////////////////////////
	// Function: “DrawWithDelay”
	//
	// Purpose: DrawWithDelay renders a string in a typewriter fashion to the screen with a specified 
	//			delay time, position, scale and color. Make sure that you only use one delay time per 
	//			font variable.
	//			szText - The string to be displayed to the screen
	//			nX - X position
	//			nY - Y position
	//			fScale - Scale of the image being displayed, 1 being the normal size, less than one being
	//					 smaller, greater than 1 being larger
	//			dwColor - The color to shade the image being displayed
	//			fDelayTime - The time to wait before displaying the next letter in the string
	//			nDelayID - The ID to the delay timer used for this particular call to DrawWithDelay, this
	//					   is returned through the AddDelay function
	//
	//			THIS FUNCTION NEEDS A DELAY ID 
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	void DrawWithDelay( const char* szText,  int nX, int nY, float fScale, DWORD dwColor, float fDelayTime,
		int nDelayID );

	//////////////////////////////////////////////////////////////////////////////////////////////////////
	// Function: “DrawScrolling”
	//
	// Purpose: DrawScrolling renders a string to the screen and scrolls in the direction that it is givin.
	//			nXScrolling and nYScrolling represent the direction. (+) for right and down. (-) for left
	//			and up.  The nMin / nMax Height and Width determine if the rendered text wants to be full
	//			screen or just a section.
	//			szText - The string to be displayed to the screen
	//			nX - X position
	//			nY - Y position
	//			fScale - Scale of the image being displayed, 1 being the normal size, less than one being
	//					 smaller, greater than 1 being larger
	//			dwColor - The color to shade the image being displayed
	//			nMinDrawHeight - The minimum height that text will be displayed to the screen (top)
	//			nMaxDrawHeight - The maximum height that the text will be displayed to the screen (bottom)
	//			nMinDrawWidth - The minimum width that the text will be displayed to the screen (left)
	//			nMaxDrawWidth - The maximum width that the text till be displayed to the screen (right)
	//			nScrollingID - The ID to the scrolling offsets for the x and y position used for this 
	//						   particular call to DrawScrolling, this is returned through the AddScrolling 
	//						   function
	//
	//			THIS FUNCTION NEEDS A SCROLLING ID
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	void DrawScrolling( const char* szText, int nX, int nY, float fScale, DWORD dwColor,
		int nMinDrawHeight, int nMaxDrawHeight, int nMinDrawWidth, int nMaxDrawWidth, 
		int nScrollingID );

	//////////////////////////////////////////////////////////////////////////////////////////////////////
	// Function: “DrawScrollingWithDelay”
	//
	// Purpose: DrawScrollingWithDelay renders a string to the screen and scrolls in the direction that it 
	//			is givin. nXScrolling and nYScrolling represent the direction. (+) for right and down. 
	//			(-) for left and up.  The nMin / nMax Height and Width determine if the rendered text wants
	//			to be full screen or just a section. This Draw also does a typewriter like fashion, as seen
	//			in DrawWithDelay.
	//			szText - The string to be displayed to the screen
	//			nX - X position
	//			nY - Y position
	//			fScale - Scale of the image being displayed, 1 being the normal size, less than one being
	//					 smaller, greater than 1 being larger
	//			dwColor - The color to shade the image being displayed
	//			nMinDrawHeight - The minimum height that text will be displayed to the screen (top)
	//			nMaxDrawHeight - The maximum height that the text will be displayed to the screen (bottom)
	//			nMinDrawWidth - The minimum width that the text will be displayed to the screen (left)
	//			nMaxDrawWidth - The maximum width that the text till be displayed to the screen (right)
	//			fDelayTime - The time to wait before displaying the next letter in the string
	//			nScrollingID - The ID to the scrolling offsets for the x and y position used for this 
	//						   particular call to DrawScrolling, this is returned through the AddScrolling 
	//						   function
	//			nDelayID - The ID to the delay timer used for this particular call to DrawWithDelay, this
	//					   is returned through the AddDelay function
	//
	//			THIS FUNCTION NEEDS A DELAY ID AND SCROLLING ID
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	void DrawScrollingWithDelay( const char* szText, int nX, int nY, float fScale, DWORD dwColor,
		int nMinDrawHeight, int nMaxDrawHeight, int nMinDrawWidth, int nMaxDrawWidth, float fDelayTime,
		int nScrollingID, int nDelayID);
};

#endif