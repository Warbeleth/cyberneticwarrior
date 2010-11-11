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
	rRender.top = 372;
	rRender.left = 538;
	rRender.bottom = 402;
	rRender.right = 638;
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
			vecMousePosition.fX -= GetPosX();
			vecMousePosition.fY -= GetPosY();
			
			// Find Initial rotation
			float fAngle = AngleBetweenVectors( vecRocketRotation, vecMousePosition ) - SGD_PI/2;

			// Calculate final rotation
			if( CSGD_DirectInput::GetInstance()->MouseGetPosX() < GetPosX() - CCamera::GetInstance()->GetOffsetX() )
				fAngle = SGD_PI - fAngle ;

			SetRotation(fAngle);
			SetBaseVelX( vecMousePosition.fX);
			SetBaseVelY( vecMousePosition.fY);

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
}

bool CRocket::CheckCollision( CBase* pBase )
{	
	if(CBaseProjectile::CheckCollision( pBase ))
	{				
		// Destroy the rocket
		if(this->GetOwner()->GetType() == OBJ_ENEMY)
		{
			if(pBase->GetType() != OBJ_ENEMY)
				CGame::GetInstance()->GetMessageSystemPointer()->SendMsg( new CDestroyRocketMessage( this, this->GetOwner()) );
		}
		else
		{
			CGame::GetInstance()->GetMessageSystemPointer()->SendMsg( new CDestroyRocketMessage( this, this->GetOwner()) );
		}
		return true;
	}
	else
		return false;
}