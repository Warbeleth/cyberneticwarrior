#include "PrecompiledHeader.h"

#include "CIce.h"
#include "CGame.h"
#include "CSinglePlayerState.h"
#include "CPlayer.h"

CIce::CIce(void)
{
	this->SetType(OBJ_ICE);
	this->SetImageID(CSinglePlayerState::GetInstance()->GetWeaponID());
	this->SetRotation(CSinglePlayerState::GetInstance()->GetPlayerPointer()->GetRotation());
	this->m_fLifeTime = 1.0f;
}

CIce::~CIce(void)
{
}

void CIce::Update(float fElapsedTime)
{
	CBase::Update(fElapsedTime);

	static float fAge = 0.0f;
	if(fAge > this->m_fLifeTime)
	{
		//CGame::GetInstance()->GetMessageSystemPointer()->SendMsg( new CDestroyIceMessage( this, CSinglePlayerState::GetInstance()->GetPlayerPointer()) );
		fAge = 0.0f;
	}
}

void CIce::Render(void)
{
	RECT rRender;
	rRender.top = 621;
	rRender.left = 819;
	rRender.right = 868;
	rRender.bottom = 648;
	CSGD_TextureManager::GetInstance()->Draw( GetImageID(), 
		(int)(((GetPosX() - (GetWidth()/2.0f) ) - CCamera::GetInstance()->GetOffsetX()) * CCamera::GetInstance()->GetScale()), 
		(int)(((GetPosY() - (GetHeight()/2.0f)) - CCamera::GetInstance()->GetOffsetY()) * CCamera::GetInstance()->GetScale()), 
		1.0f * CCamera::GetInstance()->GetScale(), 
		1.0f * CCamera::GetInstance()->GetScale(), 
		&rRender, (GetWidth()/2.0f), (GetHeight()/2.0f),
		this->GetRotation() );

	CSGD_Direct3D::GetInstance()->DrawLine( GetPosX()-4, GetPosY()-4, GetPosX()+4, GetPosY()+4, 255, 0, 0 );
	CSGD_Direct3D::GetInstance()->DrawLine( GetPosX()-4, GetPosY()+4, GetPosX()+4, GetPosY()-4, 255, 0, 0 );
}

RECT CIce::GetRect(void) const
{
	RECT rCollision;
	rCollision.top = (LONG)( GetPosY() );
	rCollision.left = (LONG)( GetPosX() );
	rCollision.bottom = (LONG)( rCollision.top + GetHeight() );
	rCollision.right = rCollision.left + GetWidth();

	return rCollision;
}

bool CIce::CheckCollision(CBase *pBase)
{
	RECT rIntersect;
	if( IntersectRect(&rIntersect, &GetRect(), &pBase->GetRect()) )
	{
		if( pBase->GetType() != OBJ_PLAYER )
		{
			// Destroy the bullet
			//CGame::GetInstance()->GetMessageSystemPointer()->SendMsg( new CDestroyIceMessage( this, CSinglePlayerState::GetInstance()->GetPlayerPointer()) );
		}

		return 1;
	}
	else
		return 0;
}