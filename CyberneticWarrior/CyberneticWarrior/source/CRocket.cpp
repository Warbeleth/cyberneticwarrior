#include "PrecompiledHeader.h"

#include "CRocket.h"
#include "CGame.h"
#include "CSinglePlayerState.h"
#include "CPlayer.h"

CRocket::CRocket( void )
{
	m_fDeathTimer	= 0.0f;
	m_nRocketState	= ROCKET_DIRECTIONAL;
	SetType( OBJ_ROCKET );
	SetImageID(CSinglePlayerState::GetInstance()->GetWeaponID());
	this->SetRotation(CSinglePlayerState::GetInstance()->GetPlayerPointer()->GetRotation());
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
			tVector2D vecRocketRotation;
			vecRocketRotation.fX = 0.0f;
			vecRocketRotation.fY = -1.0f;

			tVector2D vecMousePosition;
			vecMousePosition.fX = (float)(CSGD_DirectInput::GetInstance()->MouseGetPosX() + CCamera::GetInstance()->GetOffsetX());
			vecMousePosition.fY = (float)(CSGD_DirectInput::GetInstance()->MouseGetPosY() + CCamera::GetInstance()->GetOffsetY());

			SetRotation( CSinglePlayerState::GetInstance()->GetPlayerPointer()->GetRotation() );

			if( CSGD_DirectInput::GetInstance()->MouseGetPosX() < GetPosX()  - CCamera::GetInstance()->GetOffsetX() )
				SetRotation( 2*SGD_PI - CSinglePlayerState::GetInstance()->GetPlayerPointer()->GetRotation() );
	
			
			SetBaseVelX( vecMousePosition.fX - 8 - GetPosX() );
			SetBaseVelY( vecMousePosition.fY + 8 - GetPosY() );

			break;
		}
	}


	static tVector2D vScreenDimensions;
	vScreenDimensions.fX = (float)CGame::GetInstance()->GetScreenWidth();
	vScreenDimensions.fY = (float)CGame::GetInstance()->GetScreenHeight();
	if(((this->GetPosX() + this->GetWidth()/2.0f) <= -20 
		|| ((this->GetPosX() - this->GetWidth()/2.0f) >= (CCamera::GetInstance()->GetOffsetX() + vScreenDimensions.fX + 20))
		|| (this->GetPosY() + (this->GetHeight()/2.0f)) <= -20)
		)//|| (this->GetPosY() - (this->GetHeight()/2.0f) >= (vScreenDimensions.fY+20)))
	{
		// destroy
		CGame::GetInstance()->GetMessageSystemPointer()->SendMsg(new CDestroyRocketMessage(this, this->GetOwner()));
	}

	CBase::Update( fElapsedTime );
}

void CRocket::Render( void )
{
	static RECT rRender;
	
	rRender.top = 372;
	rRender.left = 538;
	rRender.bottom = 402;
	rRender.right = 638;
	CSGD_TextureManager::GetInstance()->Draw( GetImageID(), 
		(int)(((GetPosX() - (GetWidth()/2.0f) ) - CCamera::GetInstance()->GetOffsetX()) * CCamera::GetInstance()->GetScale()), 
		(int)(((GetPosY() - (GetHeight()/2.0f)) - CCamera::GetInstance()->GetOffsetY()) * CCamera::GetInstance()->GetScale()), 
		CCamera::GetInstance()->GetScale(), 
		CCamera::GetInstance()->GetScale(), 
		&rRender, (GetWidth()/2.0f), (GetHeight()/2.0f),
		this->GetRotation() );
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
		if(this->GetOwner()->GetType() == OBJ_PLAYER)
		{
			if( pBase->GetType() != OBJ_PLAYER && pBase->GetType() != OBJ_SPAWNER )
			{
				// Destroy the rocket
				CGame::GetInstance()->GetMessageSystemPointer()->SendMsg( new CDestroyRocketMessage( this, this->GetOwner()) );
			}
		}
		else if(this->GetOwner()->GetType() == OBJ_ENEMY)
		{
			if( pBase->GetType() != OBJ_ENEMY && pBase->GetType() != OBJ_SPAWNER )
			{
				// Destroy the rocket
				CGame::GetInstance()->GetMessageSystemPointer()->SendMsg( new CDestroyRocketMessage( this, this->GetOwner()) );
			}
		}

		return 1;
	}
	else
		return 0;
}

void CRocket::HandleEvent( CEvent* pEvent )
{
}