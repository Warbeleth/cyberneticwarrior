#include "PrecompiledHeader.h"

#include "CRocket.h"
#include "CGame.h"
#include "CSinglePlayerState.h"
#include "CPlayer.h"

CRocket::CRocket( void )
{
	m_nRocketState	= ROCKET_DIRECTIONAL;
	SetType( OBJ_ROCKET );
	SetImageID(CSinglePlayerState::GetInstance()->GetWeaponID());
	SetDamage(120);
	m_rRender.top = 372;
	m_rRender.left = 538;
	m_rRender.bottom = 402;
	m_rRender.right = 638;
}

CRocket::~CRocket( void )
{
}

void CRocket::Update( float fElapsedTime)
{
	CBaseProjectile::Update( fElapsedTime );

	if(CSinglePlayerState::GetInstance()->GetPlayerPointer()->GetHoming())
	{
		m_nRocketState = ROCKET_HOMING;
	}
	else
		m_nRocketState = ROCKET_DIRECTIONAL;

	if( m_bDead )
	{
		CGame::GetInstance()->GetMessageSystemPointer()->SendMsg( new CCreateExplosionMessage( GetPosX(), GetPosY()) );
		CGame::GetInstance()->GetMessageSystemPointer()->SendMsg( new CDestroyRocketMessage( this, CSinglePlayerState::GetInstance()->GetPlayerPointer()) );
	}

	switch(m_nRocketState)
	{
	case ROCKET_DIRECTIONAL:
		{
			break;
		}
	case ROCKET_HOMING:
		{
			tVector2D vecRocketRotation;
			vecRocketRotation.fX = 0.0f;
			vecRocketRotation.fY = -1.0f;

			tVector2D vecMousePosition;
			vecMousePosition.fX = (float)(CSGD_DirectInput::GetInstance()->MouseGetPosX() + CCamera::GetInstance()->GetOffsetX());
			vecMousePosition.fY = (float)(CSGD_DirectInput::GetInstance()->MouseGetPosY() + CCamera::GetInstance()->GetOffsetY());
			vecMousePosition.fX -= GetPosX();
			vecMousePosition.fY -= GetPosY();
			
			// Find Initial rotation
			float fAngle = AngleBetweenVectors( vecRocketRotation, vecMousePosition ) - SGD_PI/2;

			// Calculate final rotation
			if( CSGD_DirectInput::GetInstance()->MouseGetPosX() < GetPosX() - CCamera::GetInstance()->GetOffsetX() )
				fAngle = SGD_PI - fAngle;

			SetRotation(fAngle);
			SetBaseVelX( vecMousePosition.fX);
			SetBaseVelY( vecMousePosition.fY);

			break;
		}
	}
}

bool CRocket::CheckCollision( CBase* pBase )
{	
	if(CBaseProjectile::CheckCollision( pBase ))
	{				
		// Destroy the rocket
		CSGD_WaveManager::GetInstance()->Play(CSinglePlayerState::GetInstance()->GetSFX(EXPLOSION));
		CGame::GetInstance()->GetMessageSystemPointer()->SendMsg( new CCreateExplosionMessage(GetPosX(), GetPosY()));
		CGame::GetInstance()->GetMessageSystemPointer()->SendMsg( new CDestroyRocketMessage( this, this->GetOwner()));
		return true;
	}
	else
		return false;
}