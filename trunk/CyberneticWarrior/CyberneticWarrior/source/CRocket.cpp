#include "PrecompiledHeader.h"

#include "CRocket.h"
#include "CGame.h"
#include "CSinglePlayerState.h"
#include "CPlayer.h"

//	RocketStates { ROCKET_DIRECTIONAL, ROCKET_HOMING, ROCKET_MAX };

CRocket::CRocket( void )
{
	m_fRotation		= 0.0f;
	m_fDeathTimer	= 0.0f;
	m_nRocketState	= ROCKET_DIRECTIONAL;
	SetType( OBJ_ROCKET );
	SetImageID(CSinglePlayerState::GetInstance()->GetRocketID());
}

CRocket::~CRocket( void )
{
}

void CRocket::Update( float fElapsedTime)
{

	if(CSinglePlayerState::GetInstance()->GetPlayerPointer()->GetHoming())
	{
		m_nRocketState = ROCKET_HOMING;
	}
	else
		m_nRocketState = ROCKET_DIRECTIONAL;

	m_fDeathTimer += fElapsedTime;

	if( m_fDeathTimer > DEATH_TIME )
		CGame::GetInstance()->GetMessageSystemPointer()->SendMsg( new CDestroyRocketMessage( this, CSinglePlayerState::GetInstance()->GetPlayerPointer()) );


	switch(m_nRocketState)
	{
	case ROCKET_DIRECTIONAL:
		{
			break;
		}
	case ROCKET_HOMING:
		{
			tVector2D vecMousePosition;
			vecMousePosition.fX = (float)(CSGD_DirectInput::GetInstance()->MouseGetPosX());
			vecMousePosition.fY = (float)(CSGD_DirectInput::GetInstance()->MouseGetPosY());

			SetBaseVelX( vecMousePosition.fX - 8 - GetPosX() );
			SetBaseVelY( vecMousePosition.fY + 8 - GetPosY() );

			break;
		}
	}

	CBase::Update( fElapsedTime );
}

void CRocket::Render( void )
{
	CSGD_TextureManager::GetInstance()->Draw( GetImageID(), (int)(GetPosX() + (GetWidth()/2.0f) ), 
		(int)(GetPosY() - (GetHeight()/2.0f)), 1.0f, 1.0f, 0, (GetWidth()/2.0f), (GetHeight()/2.0f),
		m_fRotation );
}

RECT CRocket::GetRect( void ) const
{
	RECT rCollision;
	rCollision.top = (LONG)( GetPosY() );
	rCollision.left = (LONG)( GetPosX() );
	rCollision.bottom = (LONG)( rCollision.top + GetHeight() );
	rCollision.right = rCollision.left + GetWidth();

	return rCollision;
}

bool CRocket::CheckCollision( CBase* pBase )
{
	RECT rIntersect;
	if( IntersectRect(&rIntersect, &GetRect(), &pBase->GetRect()) )
	{
		if( pBase->GetType() != OBJ_PLAYER )
		{
			// Destroy the rocket
			CGame::GetInstance()->GetMessageSystemPointer()->SendMsg( new CDestroyRocketMessage( this, CSinglePlayerState::GetInstance()->GetPlayerPointer()) );
		}

		return 1;
	}
	else
		return 0;
}

void CRocket::HandleEvent( CEvent* pEvent )
{
}