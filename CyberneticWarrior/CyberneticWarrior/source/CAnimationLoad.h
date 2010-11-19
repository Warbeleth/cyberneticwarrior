//////////////////////////////////////////////////////////////////////////////////////////////////////
// File: �CAnimationLoad.h�
//
// Author: Anthony Muccio(AM)
//
// Purpose: Primaraly the holder of the CAnimations class that will load in any amount of animations
//			from a binary file.  These animations must reside on the same spritesheet as the others
//			being loaded in. This also holds CAnimation, which is a particular animation, CFrame, 
//			which is the holder of each frame and their collsion / hit rects. Lastly it holds CPoint
//			which is just an x, y position.
//////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _CANIMATIONLOAD_H_
#define _CANIMATIONLOAD_H_

// My Includes
#include <d3d9.h>
#include <vector>
using std::vector;
#include <string>
using std::string;
class CBase;

//////////////////////////////////////////////////////////////////////////////////////////////////////
//						******************* CLASS - CPoint *******************
//////////////////////////////////////////////////////////////////////////////////////////////////////
class CPoint
{
public:
	int m_nX;
	int m_nY;
	
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	// Function: �CPoint�
	//
	// Purpose: The default constructor for CPoint. Will initialize values.
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	CPoint( );
};

//////////////////////////////////////////////////////////////////////////////////////////////////////
//						******************* CLASS - CFrame *******************
//////////////////////////////////////////////////////////////////////////////////////////////////////
class CFrame
{
private:
	RECT			m_rFrame;				// Frame of the image being displayed
	CPoint			m_ptAnchor;				// Anchor to align th eimages on
	CPoint			m_ptPivotPoint;			// The arm pivot point for an animation
	int				m_nTotalHitRects;		// Total hit rects for melee weapon collision
	int				m_nTotalCollisionRects;	// Total collision rects for the character
	int				m_nTrigger;				// Trigger for an event to occur
	vector<RECT>	m_vHitRects;			// The actual hit rects data
	vector<RECT>	m_vCollisionRects;		// The actual collision rects data

	// Friends with CAnimation
	friend class CAnimation;

public:
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	// Function: �CFrame�
	//
	// Purpose: The default constructor for CFrame. Will initialize values.
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	CFrame(  );

	//////////////////////////////////////////////////////////////////////////////////////////////////////
	// Function: �AddCollisionRect�
	//
	// Purpose: Will add a collision rect to this particular frame in the animation.
	//			rRect - The rect to add.
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	void AddCollisionRect( RECT rRect ) { m_vCollisionRects.push_back( rRect ); }
		
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	// Function: �AddHitRect�
	//
	// Purpose: Will add a hit rect to this particular frame in the animation.
	//			rRect - The rect to add.
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	void AddHitRect( RECT rRect ) { m_vHitRects.push_back( rRect ); }


	bool CheckHit( CBase* pBase, int nPosX, int nPosY );
	bool CheckCollision( CBase* pBase, int nPosX, int nPosY );
		
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	// Function: �RectRender�
	//
	// Purpose: This is for debugging purposes and should be removed in later versions. This function will
	//			render this particular frame in the animation, but in this case it wont be an image, it 
	//			will be a Rectangle. This will also display this frame's hit / collision rectangles.
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	void RectRender( int nX, int nY, float fScaleX = 1.0f);

	///////////////////////////////////////////////////////////////////////////////////////////////////////
	// Function: Accessors 
	// 
	// Purpose: Accesses the specified type. 
	///////////////////////////////////////////////////////////////////////////////////////////////////////
	CPoint GetAnchor( void ) { return m_ptAnchor; }
	CPoint GetPivot( void ) { return m_ptPivotPoint; }
	RECT GetFrame( void ) { return m_rFrame; }
	vector<RECT>* GetCollisionRects( void ) { return &m_vCollisionRects; }
	int GetTrigger( void ) { return m_nTrigger; }

	///////////////////////////////////////////////////////////////////////////////////////////////////////
	// Function: Modifiers 
	// 
	// Purpose: Modifies the specified type. 
	///////////////////////////////////////////////////////////////////////////////////////////////////////
	void SetTotalCollisionRects( int nTotalCollisionRects ) { m_nTotalCollisionRects = nTotalCollisionRects; }
	void SetTotalHitRects( int nTotalHitRects ) { m_nTotalHitRects = nTotalHitRects; }
	void SetTrigger( int nTrigger ) { m_nTrigger = nTrigger; }
	void SetFrame( RECT rRect ) { m_rFrame.left = rRect.left;
								   m_rFrame.top = rRect.top;
								   m_rFrame.right = rRect.right;
								   m_rFrame.bottom = rRect.bottom; }
	void SetAnchor( CPoint ptAnchor ) { m_ptAnchor.m_nX = ptAnchor.m_nX;
									    m_ptAnchor.m_nY = ptAnchor.m_nY; }
	void SetPivot( CPoint ptPivot ) { m_ptPivotPoint.m_nX = ptPivot.m_nX; 
									  m_ptPivotPoint.m_nY = ptPivot.m_nY; }
};

//////////////////////////////////////////////////////////////////////////////////////////////////////
//						******************* CLASS - CAnimation *******************
//////////////////////////////////////////////////////////////////////////////////////////////////////
class CAnimation
{
private:
	int					m_nTotalFrames;			// Total frames for this particular animation
	int					m_nCurrentFrame;		// Current frame this animation is set to render
	float				m_fTimeBetweenFrames;	// Time between each frame change
	vector<CFrame>		m_vFrames;			// The actual frames, their Hit Rects, etc.
	string				m_szAnimationName;		// The animation name, can be used in future builds to
												// select an animation
	int					m_nPrevFrame;

	// Friends with CAnimations as well as CFrame
	friend class CFrame;
	friend class CAnimations;

public:	
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	// Function: �CAnimation�
	//
	// Purpose: The default constructor for CAnimaiton. Will initialize values.
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	CAnimation(  );
	bool SameFrame( void );
};

//////////////////////////////////////////////////////////////////////////////////////////////////////
//						******************* CLASS - CAnimations *******************
//////////////////////////////////////////////////////////////////////////////////////////////////////
class CAnimations
{
	int						m_nTotalAnimations;		// Total animations that this character has
	int						m_nCurrentAnimation;	// Current animation to be rendered
	CPoint					m_nOffset;
	int						m_nId;
	float					m_fTotalElapsedTime;
	bool					m_bJustLoaded;
	string					m_szFilename;			// Filename of the spritesheet for this character
	vector<CAnimation>		m_vAnimations;		// Animations, their frames, etc.
	bool					m_bDone; // Animation over?

	// Friends with CAnimation
	friend class CAnimation;

public:
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	// Function: �CAnimations�
	//
	// Purpose: The default constructor for CAnimations. Will initialize values.
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	CAnimations( );	
	
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	// Function: �CAnimations�
	//
	// Purpose: The destructor for CAnimations. Will clean-up values.
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	~CAnimations( );

	CAnimations& operator=( const CAnimations& Animation );

	//////////////////////////////////////////////////////////////////////////////////////////////////////
	// Function: �Update�
	//
	// Purpose: Update will take in a delta time and add it to the currently elapsed time. After this it
	//			checks it against the animations m_fTimeBetweenFrame. If it is greater than that time it
	//			will reset the elapsed time and increment the current animation frame.
	//			fElapsedTime - The delta time to be added to the overall elapsed time
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	void Update( float fElapsedTime );
	
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	// Function: �Render�
	//
	// Purpose: This will render the current animation to the screen based on the current frame and
	//			animation values inside the CAnimations object.
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	void Render( int nPosX, int nPosY, float fScaleX = 1.0f, DWORD dwColor = -1 );

	//////////////////////////////////////////////////////////////////////////////////////////////////////
	// Function: �LoadBinary�
	//
	// Purpose: Load in a binary file generated by the Animation Editor.
	//			Return - True if it succeeds, False if it fails to open the file
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	bool LoadBinary( char* szFilename );

	bool CheckCollision( CBase* pBase, int nPosX, int nPosY );

	bool CheckHit( CBase* pBase, int nPosX, int nPosY );

	void SetCurrentAnimation( int nCurrentAnimation ) { m_nCurrentAnimation = nCurrentAnimation; m_bJustLoaded = true; }

	vector<RECT>* GetCollisionRects( void ) { return m_vAnimations[m_nCurrentAnimation].m_vFrames[m_vAnimations[m_nCurrentAnimation].m_nCurrentFrame].GetCollisionRects(); }
	RECT GetCollisionFrame( int nPosX, int nPosY );
	RECT GetFrame( void );
	int GetFrameWidth( void ); 
	int GetFrameHeight( void ) ;
	RECT GetFrameOffset( void ) { return (m_vAnimations[m_nCurrentAnimation].m_vFrames[m_vAnimations[m_nCurrentAnimation].m_nCurrentFrame].GetFrame()); }
	CPoint GetPivotPoint( void ) { return (m_vAnimations[m_nCurrentAnimation].m_vFrames[m_vAnimations[m_nCurrentAnimation].m_nCurrentFrame].GetPivot()); }
	int GetTrigger( void ) { return (m_vAnimations[m_nCurrentAnimation].m_vFrames[m_vAnimations[m_nCurrentAnimation].m_nCurrentFrame].GetTrigger()); }
	bool SameFrame( void ) { return (m_vAnimations[m_nCurrentAnimation].SameFrame()); }
	bool GetDone( void ) { return m_bDone; }
};
#endif