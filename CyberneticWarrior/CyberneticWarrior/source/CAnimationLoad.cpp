#include "CAnimationLoad.h"
#include "SGD Wrappers/CSGD_TextureManager.h"
#include "SGD Wrappers/CSGD_Direct3D.h"
#include <fstream>
using namespace std;

CAnimation::CAnimation( )
{
	m_nTotalFrames      = 0;
	m_nCurrentFrame     = 0;
	m_nTimeBetweenFrames = 0.0f;
	m_nTotalAnimationTime = 0.0f;
	m_lstFrames.clear();
	m_szAnimationName.clear();
}

CRect::CRect(  )
{
	left = 0;
	top = 0;
	right = 0;
	bottom = 0;
}

Point::Point( )
{
	X = 0;
	Y = 0;
}

CFrame::CFrame(  )
{
	m_rFrame.left            = 0;
	m_rFrame.top             = 0;
	m_rFrame.right           = 0;
	m_rFrame.bottom          = 0;
	m_pAnchor.X              = 0;
	m_pAnchor.Y              = 0;
	m_nTotalHitCRects         = 0;
	m_nTotalCollisionCRects   = 0;
	m_nTrigger               = 0;
	m_lstHitCRects.clear();
	m_lstCollisionCRects.clear();
}

void CFrame::Render( )
{
}
/*
void CFrame::CRectRender()
{ 
	//DiCRectX singletons
	CSGD_Direct3D* pD3D = CSGD_Direct3D::GetInstance();

	// Draw GetFrame()
	pD3D->DrawLine(m_rFrame.left, m_rFrame.top, m_rFrame.right, m_rFrame.top, 255, 0, 0);
	pD3D->DrawLine(m_rFrame.right, m_rFrame.top, m_rFrame.right, m_rFrame.bottom, 255, 0, 0);
	pD3D->DrawLine(m_rFrame.right, m_rFrame.bottom, m_rFrame.left, m_rFrame.bottom, 255, 0, 0);
	pD3D->DrawLine(m_rFrame.left, m_rFrame.bottom, m_rFrame.left, m_rFrame.top, 255, 0, 0);

	// Draw Anchor
	pD3D->DrawLine(m_pAnchor.X - 2, m_pAnchor.Y - 2, m_pAnchor.X + 2, m_pAnchor.Y + 2, 255, 0, 255);
	pD3D->DrawLine(m_pAnchor.X + 2, m_pAnchor.Y - 2, m_pAnchor.X - 2, m_pAnchor.Y + 2, 255, 0, 255);

	// Draw Hit CRects
	for (int i = 0; i < m_nTotalHitCRects; ++i)
	{
		pD3D->DrawLine(m_lstHitCRects[i].left, m_lstHitCRects[i].top, m_lstHitCRects[i].right, m_lstHitCRects[i].top, 0, 255, 0);
		pD3D->DrawLine(m_lstHitCRects[i].right, m_lstHitCRects[i].top, m_lstHitCRects[i].right, m_lstHitCRects[i].bottom, 0, 255, 0);
		pD3D->DrawLine(m_lstHitCRects[i].right, m_lstHitCRects[i].bottom, m_lstHitCRects[i].left, m_lstHitCRects[i].bottom, 0, 255, 0);
		pD3D->DrawLine(m_lstHitCRects[i].left, m_lstHitCRects[i].bottom, m_lstHitCRects[i].left, m_lstHitCRects[i].top, 0, 255, 0);
	}

	// Draw Collision CRects
	for (int i = 0; i < m_nTotalCollisionCRects; ++i)
	{
		pD3D->DrawLine(m_lstCollisionCRects[i].left, m_lstCollisionCRects[i].top, m_lstCollisionCRects[i].right, m_lstCollisionCRects[i].top, 0, 0, 255);
		pD3D->DrawLine(m_lstCollisionCRects[i].right, m_lstCollisionCRects[i].top, m_lstCollisionCRects[i].right, m_lstCollisionCRects[i].bottom, 0, 0, 255);
		pD3D->DrawLine(m_lstCollisionCRects[i].right, m_lstCollisionCRects[i].bottom, m_lstCollisionCRects[i].left, m_lstCollisionCRects[i].bottom, 0, 0, 255);
		pD3D->DrawLine(m_lstCollisionCRects[i].left, m_lstCollisionCRects[i].bottom, m_lstCollisionCRects[i].left, m_lstCollisionCRects[i].top, 0, 0, 255);
	}
}
*/

CAnimations::CAnimations()
{
	m_nTotalAnimations      = 0;
	m_nCurrentAnimation		= 0;
	m_szFilename            = "\0";
	m_lstAnimations.clear();
}

void CAnimations::Render( void )
{
	m_lstAnimations[ m_nCurrentAnimation ].m_lstFrames[ m_lstAnimations[m_nCurrentAnimation].m_nCurrentFrame ].Render();
}

bool CAnimations::LoadBinary( char* szFilename )
{
	ifstream in;
	in.clear();

	in.open( szFilename, ios_base::in | ios_base::binary);

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
			m_lstAnimations.push_back( _animation );

			// time between frames
			float nTimeBetweenFrames;
			in.read( (char*)&nTimeBetweenFrames, sizeof(float) );
			m_lstAnimations[i].m_nTimeBetweenFrames = nTimeBetweenFrames;

			// Animation Name
			m_lstAnimations[i].m_szAnimationName = "Default";

			// total frames
			int nTotalFrames;
			in.read( (char*)&nTotalFrames, sizeof(int) );
			m_lstAnimations[i].m_nTotalFrames = nTotalFrames;

			// each frame
			for ( int x = 0; x < nTotalFrames; ++x)
			{
				// add the frame
				CFrame _frame;
				m_lstAnimations[i].m_lstFrames.push_back(_frame);

				// anchor
				Point _anchor;
				in.read( (char*)&_anchor.X, sizeof(int) );
				in.read( (char*)&_anchor.Y, sizeof(int) );
				m_lstAnimations[i].m_lstFrames[x].SetAnchor( _anchor );

				// frame
				CRect _frameRect;
				in.read( (char*)&_frameRect.left, sizeof(int) );
				in.read( (char*)&_frameRect.top, sizeof(int) );
				in.read( (char*)&_frameRect.right, sizeof(int) );
				in.read( (char*)&_frameRect.bottom, sizeof(int) );
				m_lstAnimations[i].m_lstFrames[x].SetFrame( _frameRect );

				// trigger
				int nTrigger;
				in.read( (char*)&nTrigger, sizeof(int) );
				m_lstAnimations[i].m_lstFrames[x].SetTrigger( nTrigger );

				// total hit rects
				int nTotalHitRects;
				in.read( (char*)&nTotalHitRects, sizeof(int) );
				m_lstAnimations[i].m_lstFrames[x].SetTotalHitRects( nTotalHitRects );

				// each hit rect
				for ( int y = 0; y < nTotalHitRects; ++y)
				{
					CRect _hitRect;
					in.read( (char*)&_hitRect.left, sizeof(int) );
					in.read( (char*)&_hitRect.top, sizeof(int) );
					in.read( (char*)&_hitRect.right, sizeof(int) );
					in.read( (char*)&_hitRect.bottom, sizeof(int) );
					m_lstAnimations[i].m_lstFrames[x].AddHitRect( _hitRect );
				}

				// total collision rects
				int nTotalCollisionRects;
				in.read( (char*)&nTotalCollisionRects, sizeof(int) );
				m_lstAnimations[i].m_lstFrames[x].SetTotalCollisionRects( nTotalCollisionRects );

				// each collision rect
				for ( int z = 0; z < nTotalCollisionRects; ++z)
				{
					CRect _collisionRect;
					in.read( (char*)&_collisionRect.left, sizeof(int) );
					in.read( (char*)&_collisionRect.top, sizeof(int) );
					in.read( (char*)&_collisionRect.right, sizeof(int) );
					in.read( (char*)&_collisionRect.bottom, sizeof(int) );
					m_lstAnimations[i].m_lstFrames[x].AddCollisionRect( _collisionRect );
				}
			}
		}
		in.close();
		return true;
	}
	return false;
}