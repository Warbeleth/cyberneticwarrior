#include "PrecompiledHeader.h"
#include "CExplosion.h"
#include "CMapLoad.h"
#include "CGame.h"
#include "CStackStateMachine.h"
#include "CWinState.h"

CExplosion::CExplosion( void )
{
	SetAnimations(CMapLoad::GetInstance()->CreateAnimation(Anim_Explosion));
	m_dwColor = -1;
}

CExplosion::~CExplosion( void )
{
}

void CExplosion::Update( float fElapsedTime )
{
	if(GetAnimations())
		GetAnimations()->Update( fElapsedTime );

	if(GetAnimations())
	{
		if(GetAnimations()->GetDone())
		{
			// Destroy
			CGame::GetInstance()->GetMessageSystemPointer()->SendMsg( new CDestroyExplosionMessage(this) );
		}
	}
}

void CExplosion::Render( void )
{
	if(GetAnimations())
	{
		GetAnimations()->Render( (int)GetPosX(), (int)GetPosY(), 1, m_dwColor);
	}
}