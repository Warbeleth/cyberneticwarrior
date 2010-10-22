#ifndef _CANIMATIONLOAD_H_
#define _CANIMATIONLOAD_H_

#include <vector>
using std::vector;
#include <string>
using std::string;

class CRect
{
public:
	int  left;
	int  top;
	int  right;
	int  bottom;

	CRect(  );
};

class Point
{
public:
	int X;
	int Y;

	Point( );
};

class CFrame
{
private:
	CRect        m_rFrame;;
	Point       m_pAnchor;
	int         m_nTotalHitCRects;
	int         m_nTotalCollisionCRects;
	int         m_nTrigger;
	vector<CRect>  m_lstHitCRects;
	vector<CRect>  m_lstCollisionCRects;

	friend class CAnimation;

public:
	CFrame(  );

	// CFrame Methods
	void Render();
	void AddCollisionRect( CRect rRect ) { m_lstCollisionCRects.push_back( rRect ); }
	void AddHitRect( CRect rRect ) { m_lstHitCRects.push_back( rRect ); }
	//void CRectRender();

	// Accessor / Mutator
	void SetTotalCollisionRects( int nTotalCollisionRects ) { m_nTotalCollisionCRects = nTotalCollisionRects; }
	void SetTotalHitRects( int nTotalHitRects ) { m_nTotalHitCRects = nTotalHitRects; }
	void SetTrigger( int nTrigger ) { m_nTrigger = nTrigger; }
	void SetFrame( CRect rRect ) { m_rFrame.left = rRect.left;
								   m_rFrame.top = rRect.top;
								   m_rFrame.right = rRect.right;
								   m_rFrame.bottom = rRect.bottom; }
	void SetAnchor( Point ptAnchor ) { m_pAnchor.X = ptAnchor.X;
									   m_pAnchor.Y = ptAnchor.Y; }
};


class CAnimation
{
private:
	int         m_nTotalFrames;
	int         m_nCurrentFrame;
	float       m_nTimeBetweenFrames;
	float       m_nTotalAnimationTime;
	vector<CFrame> m_lstFrames;
	string     m_szAnimationName;

	friend class CFrame;
	friend class CAnimations;
public:
	CAnimation(  );
};

class CAnimations
{
	int                 m_nTotalAnimations;
	int					m_nCurrentAnimation;
	string              m_szFilename;
	vector<CAnimation>    m_lstAnimations;

	friend class CAnimation;

public:

	// Default Constructor
	CAnimations( );

	// Methods
	void Update( float fElapsedTime );
	void Render( void );

	// CAnimation Methods
	bool LoadBinary( char* szFilename );
};
#endif