//////////////////////////////////////////////////////////////////////////////////////////////////////
// File: “CAnimationLoad.cpp”
//
// Author: Anthony Muccio(AM)
//
// Purpose: This file primaraly holds the function bodies for CAnimations, but also hold the bodies
//			for CAnimation, CFrame, and CPoint.
//////////////////////////////////////////////////////////////////////////////////////////////////////
// SGD Wrappers
#include "SGD Wrappers/CSGD_TextureManager.h"
#include "SGD Wrappers/CSGD_Direct3D.h"

// My Includes
#include <fstream>
#include "CAnimationLoad.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////
// Function: “CPoint”
//////////////////////////////////////////////////////////////////////////////////////////////////////	
CPoint::CPoint( )
{
	m_nX = 0;
	m_nY = 0;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
// Function: “CFrame”
//////////////////////////////////////////////////////////////////////////////////////////////////////	
CFrame::CFrame(  )
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
// Function: “Render”
//////////////////////////////////////////////////////////////////////////////////////////////////////	
void CFrame::Render( )
{
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
// Function: “RectRender”
//////////////////////////////////////////////////////////////////////////////////////////////////////	
/*
void CFrame::RectRender()
{ 
	//DirectX singletons
	CSGD_Direct3D* pD3D = CSGD_Direct3D::GetInstance();

	// Draw GetFrame()
	pD3D->DrawLine(m_rFrame.left, m_rFrame.top, m_rFrame.right, m_rFrame.top, 255, 0, 0);
	pD3D->DrawLine(m_rFrame.right, m_rFrame.top, m_rFrame.right, m_rFrame.bottom, 255, 0, 0);
	pD3D->DrawLine(m_rFrame.right, m_rFrame.bottom, m_rFrame.left, m_rFrame.bottom, 255, 0, 0);
	pD3D->DrawLine(m_rFrame.left, m_rFrame.bottom, m_rFrame.left, m_rFrame.top, 255, 0, 0);

	// Draw Anchor
	pD3D->DrawLine(m_ptAnchor.X - 2, m_ptAnchor.Y - 2, m_ptAnchor.X + 2, m_ptAnchor.Y + 2, 255, 0, 255);
	pD3D->DrawLine(m_ptAnchor.X + 2, m_ptAnchor.Y - 2, m_ptAnchor.X - 2, m_ptAnchor.Y + 2, 255, 0, 255);

	// Draw Hit Rects
	for (int i = 0; i < m_nTotalHitRects; ++i)
	{
		pD3D->DrawLine(m_vHitRects[i].left, m_vHitRects[i].top, m_vHitRects[i].right, m_vHitRects[i].top, 0, 255, 0);
		pD3D->DrawLine(m_vHitRects[i].right, m_vHitRects[i].top, m_vHitRects[i].right, m_vHitRects[i].bottom, 0, 255, 0);
		pD3D->DrawLine(m_vHitRects[i].right, m_vHitRects[i].bottom, m_vHitRects[i].left, m_vHitRects[i].bottom, 0, 255, 0);
		pD3D->DrawLine(m_vHitRects[i].left, m_vHitRects[i].bottom, m_vHitRects[i].left, m_vHitRects[i].top, 0, 255, 0);
	}

	// Draw Collision Rects
	for (int i = 0; i < m_nTotalCollisionRects; ++i)
	{
		pD3D->DrawLine(m_vCollisionRects[i].left, m_vCollisionRects[i].top, m_vCollisionRects[i].right, m_vCollisionRects[i].top, 0, 0, 255);
		pD3D->DrawLine(m_vCollisionRects[i].right, m_vCollisionRects[i].top, m_vCollisionRects[i].right, m_vCollisionRects[i].bottom, 0, 0, 255);
		pD3D->DrawLine(m_vCollisionRects[i].right, m_vCollisionRects[i].bottom, m_vCollisionRects[i].left, m_vCollisionRects[i].bottom, 0, 0, 255);
		pD3D->DrawLine(m_vCollisionRects[i].left, m_vCollisionRects[i].bottom, m_vCollisionRects[i].left, m_vCollisionRects[i].top, 0, 0, 255);
	}
}
*/

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
	m_szFilename            = "\0";
	m_vAnimations.clear();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
// Function: “Render”
//////////////////////////////////////////////////////////////////////////////////////////////////////	
void CAnimations::Render( void )
{
	m_vAnimations[ m_nCurrentAnimation ].m_vFrames[ m_vAnimations[m_nCurrentAnimation].m_nCurrentFrame ].Render();
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
				in.read( (char*)&_anchor.m_nX, sizeof(int) );
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
		return true;
	}
	return false;
}