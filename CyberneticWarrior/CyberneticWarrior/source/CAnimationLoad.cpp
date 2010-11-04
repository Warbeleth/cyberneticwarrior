\//////////////////////////////////////////////////////////////////////////////////////////////////////
// File: “CAnimationLoad.cpp”
//
// Author: Anthony Muccio(AM)
//
// Purpose: This file primaraly holds the function bodies for CAnimations, but also hold the bodies
//			for CAnimation, CFrame, and CPoint.
//////////////////////////////////////////////////////////////////////////////////////////////////////
#include "PrecompiledHeader.h"

// My Includes
#include "CAnimationLoad.h"
#include "CBase.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////
// Function: “CPoint”
//////////////////////////////////////////////////////////////////////////////////////////////////////	
CPoint::CPoint( void )
{
	m_nX = 0;
	m_nY = 0;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
// Function: “CFrame”
//////////////////////////////////////////////////////////////////////////////////////////////////////	
CFrame::CFrame( void )
{
	m_rFrame.left            = 0;
	m_rFrame.top             = 0;
	m_rFrame.right           = 0;
	m_rFrame.bottom          = 0;
	m_ptAnchor.m_nX           = 0;
	m_ptAnchor.m_nY           = 0;
	m_nTotalHitRects         = 0;
	m_nTotalCollisionRects   = 0;
	m_nTrigger               = 0;
	m_vHitRects.clear();
	m_vCollisionRects.clear();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
// Function: “RectRender”
//////////////////////////////////////////////////////////////////////////////////////////////////////	
void CFrame::RectRender( int nX, int nY, float fScale )
{ 
	//DirectX singletons
	CSGD_Direct3D* pD3D = CSGD_Direct3D::GetInstance();
		
	// Draw GetFrame()
	RECT rFrame = m_rFrame;
	rFrame.right -= rFrame.left;
	rFrame.left = 0;
	rFrame.bottom -= rFrame.top;
	rFrame.top = 0;

	pD3D->DrawLine(rFrame.left + nX, rFrame.top + nY, rFrame.right + nX, rFrame.top + nY, 255, 0, 0);
	pD3D->DrawLine(rFrame.right + nX, rFrame.top + nY, rFrame.right + nX, rFrame.bottom + nY, 255, 0, 0);
	pD3D->DrawLine(rFrame.right + nX, rFrame.bottom + nY, rFrame.left + nX, rFrame.bottom + nY, 255, 0, 0);
	pD3D->DrawLine(rFrame.left + nX, rFrame.bottom + nY, rFrame.left + nX, rFrame.top + nY, 255, 0, 0);
	
	// Draw Anchor
	CPoint pAnchor;
	pAnchor.m_nX = m_ptAnchor.m_nX - m_rFrame.left;
	pAnchor.m_nY = m_ptAnchor.m_nY - m_rFrame.top;

	pD3D->DrawLine(pAnchor.m_nX + nX - 2, pAnchor.m_nY + nY - 2, pAnchor.m_nX + nX + 2, pAnchor.m_nY + nY + 2, 255, 0, 255);
	pD3D->DrawLine(pAnchor.m_nX + nX + 2, pAnchor.m_nY + nY - 2, pAnchor.m_nX + nX - 2, pAnchor.m_nY + nY + 2, 255, 0, 255);

	if(fScale == 1)
	{
		// Draw Hit Rects
		for (int i = 0; i < m_nTotalHitRects; ++i)
		{
			RECT rHitFrame = m_vHitRects[i];
			rHitFrame.left -= m_rFrame.left;
			rHitFrame.right -= m_rFrame.left;
			rHitFrame.top -= m_rFrame.top;
			rHitFrame.bottom -= m_rFrame.top;

			pD3D->DrawLine(rHitFrame.left + nX, rHitFrame.top + nY, rHitFrame.right + nX, rHitFrame.top + nY, 0, 255, 0);
			pD3D->DrawLine(rHitFrame.right + nX, rHitFrame.top + nY, rHitFrame.right + nX, rHitFrame.bottom + nY, 0, 255, 0);
			pD3D->DrawLine(rHitFrame.right + nX, rHitFrame.bottom + nY, rHitFrame.left + nX, rHitFrame.bottom + nY, 0, 255, 0);
			pD3D->DrawLine(rHitFrame.left + nX, rHitFrame.bottom + nY, rHitFrame.left + nX, rHitFrame.top + nY, 0, 255, 0);
		}

		// Draw Collision Rects
		for (int i = 0; i < m_nTotalCollisionRects; ++i)
		{
			RECT rHitFrame = m_vCollisionRects[i];
			rHitFrame.left -= m_rFrame.left;
			rHitFrame.right -= m_rFrame.left;
			rHitFrame.top -= m_rFrame.top;
			rHitFrame.bottom -= m_rFrame.top;

			pD3D->DrawLine(rHitFrame.left + nX, rHitFrame.top + nY, rHitFrame.right + nX, rHitFrame.top + nY, 0, 0, 255);
			pD3D->DrawLine(rHitFrame.right + nX, rHitFrame.top + nY, rHitFrame.right + nX, rHitFrame.bottom + nY, 0, 0, 255);
			pD3D->DrawLine(rHitFrame.right + nX, rHitFrame.bottom + nY, rHitFrame.left + nX, rHitFrame.bottom + nY, 0, 0, 255);
			pD3D->DrawLine(rHitFrame.left + nX, rHitFrame.bottom + nY, rHitFrame.left + nX, rHitFrame.top + nY, 0, 0, 255);
		}
	}
	else if( fScale == -1 )
	{
		pD3D->DrawLine(pAnchor.m_nX + nX - 2, pAnchor.m_nY + nY - 2, pAnchor.m_nX + nX + 2, pAnchor.m_nY + nY + 2, 255, 0, 255);
		pD3D->DrawLine(pAnchor.m_nX + nX + 2, pAnchor.m_nY + nY - 2, pAnchor.m_nX + nX - 2, pAnchor.m_nY + nY + 2, 255, 0, 255);

		// Draw Hit Rects
		for (int i = 0; i < m_nTotalHitRects; ++i)
		{
			int nOffset;
			RECT rHitFrame = m_vHitRects[i];
			nOffset = m_rFrame.right - rHitFrame.right;
			rHitFrame.left -= m_rFrame.left + nOffset;
			rHitFrame.right -= m_rFrame.left;
			rHitFrame.top -= m_rFrame.top;
			rHitFrame.bottom -= m_rFrame.top;

			pD3D->DrawLine(rHitFrame.left + nX, rHitFrame.top + nY, rHitFrame.right + nX, rHitFrame.top + nY, 0, 255, 0);
			pD3D->DrawLine(rHitFrame.right + nX, rHitFrame.top + nY, rHitFrame.right + nX, rHitFrame.bottom + nY, 0, 255, 0);
			pD3D->DrawLine(rHitFrame.right + nX, rHitFrame.bottom + nY, rHitFrame.left + nX, rHitFrame.bottom + nY, 0, 255, 0);
			pD3D->DrawLine(rHitFrame.left + nX, rHitFrame.bottom + nY, rHitFrame.left + nX, rHitFrame.top + nY, 0, 255, 0);
		}

		// Draw Collision Rects
		for (int i = 0; i < m_nTotalCollisionRects; ++i)
		{
			int nOffset;
			RECT rHitFrame = m_vCollisionRects[i];
			nOffset = m_rFrame.right - rHitFrame.right;
			rHitFrame.right -= rHitFrame.left;
			rHitFrame.right += nOffset;
			rHitFrame.left = nOffset;
			rHitFrame.top -= m_rFrame.top;
			rHitFrame.bottom -= m_rFrame.top;

			pD3D->DrawLine(rHitFrame.left + nX, rHitFrame.top + nY, rHitFrame.right + nX, rHitFrame.top + nY, 0, 0, 255);
			pD3D->DrawLine(rHitFrame.right + nX, rHitFrame.top + nY, rHitFrame.right + nX, rHitFrame.bottom + nY, 0, 0, 255);
			pD3D->DrawLine(rHitFrame.right + nX, rHitFrame.bottom + nY, rHitFrame.left + nX, rHitFrame.bottom + nY, 0, 0, 255);
			pD3D->DrawLine(rHitFrame.left + nX, rHitFrame.bottom + nY, rHitFrame.left + nX, rHitFrame.top + nY, 0, 0, 255);
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
// Function: “CAnimation”
//////////////////////////////////////////////////////////////////////////////////////////////////////	
CAnimation::CAnimation( )
{
	m_nTotalFrames      = 0;
	m_nCurrentFrame     = 0;
	m_fTimeBetweenFrames = 0.0f;
	m_vFrames.clear();
	m_szAnimationName.clear();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
// Function: “CAnimations”
//////////////////////////////////////////////////////////////////////////////////////////////////////	
CAnimations::CAnimations()
{
	m_nTotalAnimations      = 0;
	m_nCurrentAnimation		= 0;	
	m_nOffset.m_nX			= 0;
	m_nOffset.m_nY			= 0;
	m_nId					= -1;
	m_fTotalElapsedTime		= 0.0f;
	m_szFilename            = "\0";
	m_bJustLoaded			= false;
	m_vAnimations.clear();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
// Function: “~CAnimations”
//
// Purpose: The destructor for CAnimations. Will clean-up values.
//////////////////////////////////////////////////////////////////////////////////////////////////////
CAnimations::~CAnimations( )
{

	//if(m_nId != -1)
	//	CSGD_TextureManager::GetInstance()->UnloadTexture(m_nId);


}

CAnimations& CAnimations::operator=( const CAnimations& Animation )
{
	if( this != &Animation )
	{
		// spritesheet filename
		this->m_szFilename = Animation.m_szFilename;

		// total animations
		this->m_nTotalAnimations = Animation.m_nTotalAnimations;

		// each animation
		for ( int i = 0; i < m_nTotalAnimations; ++i)
		{
			// add the animation
			this->m_vAnimations.push_back( Animation.m_vAnimations[i] );
		}

		if( m_nId != -1 )
		{
			CSGD_TextureManager::GetInstance()->UnloadTexture(m_nId);
			m_nId = -1;
		}

		string RelativeFile = "resource/graphics/" + m_szFilename;
		m_nId = CSGD_TextureManager::GetInstance()->LoadTexture(RelativeFile.c_str());
	}

	return *this;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
// Function: “Update”
//////////////////////////////////////////////////////////////////////////////////////////////////////
void CAnimations::Update( float fElapsedTime )
{
	m_fTotalElapsedTime += fElapsedTime;

	if( m_fTotalElapsedTime >= m_vAnimations[m_nCurrentAnimation].m_fTimeBetweenFrames)
	{
		m_fTotalElapsedTime = 0.0f;
		m_vAnimations[m_nCurrentAnimation].m_nCurrentFrame++;

		if( m_vAnimations[m_nCurrentAnimation].m_nCurrentFrame >= m_vAnimations[m_nCurrentAnimation].m_nTotalFrames )
			m_vAnimations[m_nCurrentAnimation].m_nCurrentFrame = 0;

		m_nOffset.m_nX = m_vAnimations[m_nCurrentAnimation].m_vFrames[m_vAnimations[m_nCurrentAnimation].m_nCurrentFrame].GetAnchor().m_nX -
			m_vAnimations[ m_nCurrentAnimation ].m_vFrames[ m_vAnimations[m_nCurrentAnimation].m_nCurrentFrame ].GetFrame().left;
		m_nOffset.m_nY = m_vAnimations[m_nCurrentAnimation].m_vFrames[m_vAnimations[m_nCurrentAnimation].m_nCurrentFrame].GetAnchor().m_nY -
			m_vAnimations[ m_nCurrentAnimation ].m_vFrames[ m_vAnimations[m_nCurrentAnimation].m_nCurrentFrame ].GetFrame().top;	
	}
	else if( m_bJustLoaded )
	{

		m_nOffset.m_nX = m_vAnimations[m_nCurrentAnimation].m_vFrames[m_vAnimations[m_nCurrentAnimation].m_nCurrentFrame].GetAnchor().m_nX -
			m_vAnimations[ m_nCurrentAnimation ].m_vFrames[ m_vAnimations[m_nCurrentAnimation].m_nCurrentFrame ].GetFrame().left;
		m_nOffset.m_nY = m_vAnimations[m_nCurrentAnimation].m_vFrames[m_vAnimations[m_nCurrentAnimation].m_nCurrentFrame].GetAnchor().m_nY -
			m_vAnimations[ m_nCurrentAnimation ].m_vFrames[ m_vAnimations[m_nCurrentAnimation].m_nCurrentFrame ].GetFrame().top;	

		m_bJustLoaded = false;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
// Function: “Render”
//////////////////////////////////////////////////////////////////////////////////////////////////////	
void CAnimations::Render( int nPosX, int nPosY, float fScale )
{
	int nXOffset = - m_nOffset.m_nX - CCamera::GetInstance()->GetOffsetX();
	int nYOffset = - m_nOffset.m_nY - CCamera::GetInstance()->GetOffsetY();
	
	CSGD_TextureManager::GetInstance()->Draw(m_nId, 
		int(CCamera::GetInstance()->GetScale() * (nPosX + nXOffset)), 
		int(CCamera::GetInstance()->GetScale() * (nPosY + nYOffset)), 
		CCamera::GetInstance()->GetScale() * fScale, 
		CCamera::GetInstance()->GetScale(), 
		&m_vAnimations[ m_nCurrentAnimation ].m_vFrames[ m_vAnimations[m_nCurrentAnimation].m_nCurrentFrame ].GetFrame() );

	if(fScale == 1)
		m_vAnimations[ m_nCurrentAnimation ].m_vFrames[ m_vAnimations[m_nCurrentAnimation].m_nCurrentFrame ].RectRender( nPosX-CCamera::GetInstance()->GetOffsetX()-GetFrameWidth()/2, nPosY-CCamera::GetInstance()->GetOffsetY()-GetFrameHeight(), fScale );
	else if(fScale == -1 )
		m_vAnimations[ m_nCurrentAnimation ].m_vFrames[ m_vAnimations[m_nCurrentAnimation].m_nCurrentFrame ].RectRender( nPosX-CCamera::GetInstance()->GetOffsetX()-3*GetFrameWidth()/2, nPosY-CCamera::GetInstance()->GetOffsetY()-GetFrameHeight(), fScale );
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
// Function: “LoadBinary”
//////////////////////////////////////////////////////////////////////////////////////////////////////	
bool CAnimations::LoadBinary( char* szFilename )
{
	std::ifstream in;
	in.clear();

	in.open( szFilename, std::ios_base::in | std::ios_base::binary);

	if(in.is_open())
	{
		char cStringLength;
		in.read( (char*)&cStringLength, sizeof(char));
		int nStringLength = (int)cStringLength;

		// spritesheet filename
		char* buffer = new char[nStringLength+1];
		in.read( (char*)buffer, nStringLength);
		buffer[nStringLength] = '\0';
		m_szFilename = buffer;
		delete buffer;

		// total animations
		in.read((char*)&m_nTotalAnimations, sizeof(int));

		// each animation
		for ( int i = 0; i < m_nTotalAnimations; ++i)
		{
			// add the animation
			CAnimation _animation;
			m_vAnimations.push_back( _animation );

			// time between frames
			float nTimeBetweenFrames;
			in.read( (char*)&nTimeBetweenFrames, sizeof(float) );
			m_vAnimations[i].m_fTimeBetweenFrames = nTimeBetweenFrames;

			// Animation Name
			m_vAnimations[i].m_szAnimationName = "Default";

			// total frames
			int nTotalFrames;
			in.read( (char*)&nTotalFrames, sizeof(int) );
			m_vAnimations[i].m_nTotalFrames = nTotalFrames;

			// each frame
			for ( int x = 0; x < nTotalFrames; ++x)
			{
				// add the frame
				CFrame _frame;
				m_vAnimations[i].m_vFrames.push_back(_frame);

				// anchor
				CPoint _anchor;
				in.read( (char*)&_anchor.m_nX, sizeof(int) );
				in.read( (char*)&_anchor.m_nY, sizeof(int) );
				m_vAnimations[i].m_vFrames[x].SetAnchor( _anchor );

				// frame
				RECT _frameRect;
				in.read( (char*)&_frameRect.left, sizeof(int) );
				in.read( (char*)&_frameRect.top, sizeof(int) );
				in.read( (char*)&_frameRect.right, sizeof(int) );
				in.read( (char*)&_frameRect.bottom, sizeof(int) );
				m_vAnimations[i].m_vFrames[x].SetFrame( _frameRect );

				// trigger
				int nTrigger;
				in.read( (char*)&nTrigger, sizeof(int) );
				m_vAnimations[i].m_vFrames[x].SetTrigger( nTrigger );

				// total hit rects
				int nTotalHitRects;
				in.read( (char*)&nTotalHitRects, sizeof(int) );
				m_vAnimations[i].m_vFrames[x].SetTotalHitRects( nTotalHitRects );

				// each hit rect
				for ( int y = 0; y < nTotalHitRects; ++y)
				{
					RECT _hitRect;
					in.read( (char*)&_hitRect.left, sizeof(int) );
					in.read( (char*)&_hitRect.top, sizeof(int) );
					in.read( (char*)&_hitRect.right, sizeof(int) );
					in.read( (char*)&_hitRect.bottom, sizeof(int) );
					m_vAnimations[i].m_vFrames[x].AddHitRect( _hitRect );
				}

				// total collision rects
				int nTotalCollisionRects;
				in.read( (char*)&nTotalCollisionRects, sizeof(int) );
				m_vAnimations[i].m_vFrames[x].SetTotalCollisionRects( nTotalCollisionRects );

				// each collision rect
				for ( int z = 0; z < nTotalCollisionRects; ++z)
				{
					RECT _collisionRect;
					in.read( (char*)&_collisionRect.left, sizeof(int) );
					in.read( (char*)&_collisionRect.top, sizeof(int) );
					in.read( (char*)&_collisionRect.right, sizeof(int) );
					in.read( (char*)&_collisionRect.bottom, sizeof(int) );
					m_vAnimations[i].m_vFrames[x].AddCollisionRect( _collisionRect );
				}
			}
		}
		in.close();

		if( m_nId != -1 )
		{
			CSGD_TextureManager::GetInstance()->UnloadTexture(m_nId);
			m_nId = -1;
		}

		string RelativeFile = "resource/graphics/" + m_szFilename;
		m_nId = CSGD_TextureManager::GetInstance()->LoadTexture(RelativeFile.c_str());

		m_bJustLoaded = true;
		return true;
	}
	return false;
}

bool CFrame::CheckCollision( CBase* pBase, int nPosX, int nPosY )
{
	if(pBase->GetAnimations())
	{
		RECT rCollision;

		vector<RECT> vBaseCollisionRects = (*pBase->GetAnimations()->GetCollisionRects());
				
		for( int i = 0; i < m_nTotalCollisionRects; ++i )
		{
			RECT rThisRect = m_vCollisionRects[i];		
			rThisRect.left -= m_rFrame.left;
			rThisRect.right -= m_rFrame.left;
			rThisRect.left += nPosX;
			rThisRect.right += nPosX;

			rThisRect.top -= m_rFrame.top;
			rThisRect.bottom -= m_rFrame.top;
			rThisRect.top += nPosY;
			rThisRect.bottom += nPosY;

			for( unsigned int x = 0; x < pBase->GetAnimations()->GetCollisionRects()->size(); ++x )
			{
				RECT rBaseRect = vBaseCollisionRects[x];
				RECT rFrame = pBase->GetAnimations()->GetFrameOffset();

				rBaseRect.left -= rFrame.left;
				rBaseRect.right -= rFrame.left;
				rBaseRect.left += pBase->GetPosX();
				rBaseRect.right += pBase->GetPosX();

				rBaseRect.top -= rFrame.top;
				rBaseRect.bottom -= rFrame.top;
				rBaseRect.top += pBase->GetPosY();
				rBaseRect.bottom += pBase->GetPosY();

				if(IntersectRect(&rCollision, &rThisRect, &rBaseRect ))
					return true;
			}
		}
	}
	else
	{		
		RECT rCollision;			

		for( int i = 0; i < m_nTotalCollisionRects; ++i )
		{
			RECT rThisRect = m_vCollisionRects[i];		
			rThisRect.left -= m_rFrame.left;
			rThisRect.right -= m_rFrame.left;
			rThisRect.left += nPosX;
			rThisRect.right += nPosX;

			rThisRect.top -= m_rFrame.top;
			rThisRect.bottom -= m_rFrame.top;
			rThisRect.top += nPosY;
			rThisRect.bottom += nPosY;

			if(IntersectRect(&rCollision, &rThisRect, &pBase->GetRect()))
				return true;
		}
	}

	return false;
}

bool CAnimations::CheckCollision( CBase* pBase, int nPosX, int nPosY )
{
	return (m_vAnimations[m_nCurrentAnimation].m_vFrames[m_vAnimations[m_nCurrentAnimation].m_nCurrentFrame].CheckCollision( pBase, nPosX, nPosY ));
}

bool CFrame::CheckHit( CBase* pBase, int nPosX, int nPosY )
{
	if(pBase->GetAnimations())
	{
		RECT rHit;

		vector<RECT> vBaseCollisionRects = (*pBase->GetAnimations()->GetCollisionRects());
				
		for( int i = 0; i < m_nTotalHitRects; ++i )
		{
			RECT rThisRect = m_vHitRects[i];		
			rThisRect.left -= m_rFrame.left;
			rThisRect.right -= m_rFrame.left;
			rThisRect.left += nPosX;
			rThisRect.right += nPosX;

			rThisRect.top -= m_rFrame.top;
			rThisRect.bottom -= m_rFrame.top;
			rThisRect.top += nPosY;
			rThisRect.bottom += nPosY;

			for( unsigned int x = 0; x < pBase->GetAnimations()->GetCollisionRects()->size(); ++x )
			{
				RECT rBaseRect = vBaseCollisionRects[x];
				RECT rFrame = pBase->GetAnimations()->GetFrameOffset();

				rBaseRect.left -= rFrame.left;
				rBaseRect.right -= rFrame.left;
				rBaseRect.left += pBase->GetPosX();
				rBaseRect.right += pBase->GetPosX();

				rBaseRect.top -= rFrame.top;
				rBaseRect.bottom -= rFrame.top;
				rBaseRect.top += pBase->GetPosY();
				rBaseRect.bottom += pBase->GetPosY();

				if(IntersectRect(&rHit, &rThisRect, &rBaseRect ))
					return true;
			}
		}
	}

	return false;
}

bool CAnimations::CheckHit( CBase* pBase, int nPosX, int nPosY )
{
	return (m_vAnimations[m_nCurrentAnimation].m_vFrames[m_vAnimations[m_nCurrentAnimation].m_nCurrentFrame].CheckHit( pBase, nPosX, nPosY ));
}
	
RECT CAnimations::GetCollisionFrame( int nPosX, int nPosY ) 
{ 
	RECT rFrame = m_vAnimations[m_nCurrentAnimation].m_vFrames[m_vAnimations[m_nCurrentAnimation].m_nCurrentFrame].GetFrame();
	rFrame.right -= rFrame.left;
	rFrame.left = 0;
	rFrame.bottom -= rFrame.top;
	rFrame.top = 0;

	rFrame.top += nPosY;
	rFrame.bottom += nPosY;
	rFrame.left += nPosX;
	rFrame.right += nPosX;

	return rFrame;
}

RECT CAnimations::GetFrame( void ) 
{ 
	RECT rFrame = m_vAnimations[m_nCurrentAnimation].m_vFrames[m_vAnimations[m_nCurrentAnimation].m_nCurrentFrame].GetFrame();
	rFrame.right -= rFrame.left;
	rFrame.left = 0;
	rFrame.bottom -= rFrame.top;
	rFrame.top = 0;

	return rFrame;
}

int CAnimations::GetFrameWidth( void )
{
	RECT rFrame = m_vAnimations[m_nCurrentAnimation].m_vFrames[m_vAnimations[m_nCurrentAnimation].m_nCurrentFrame].GetFrame();
	return (rFrame.right - rFrame.left);
}

int CAnimations::GetFrameHeight( void )
{	
	RECT rFrame = m_vAnimations[m_nCurrentAnimation].m_vFrames[m_vAnimations[m_nCurrentAnimation].m_nCurrentFrame].GetFrame();
	return (rFrame.bottom - rFrame.top);
}