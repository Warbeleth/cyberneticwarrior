#include "PrecompiledHeader.h"

#include "CFlame.h"
#include "CGame.h"
#include "CSinglePlayerState.h"
#include "CPlayer.h"

CFlame::CFlame(void)
{
	this->SetType(OBJ_FLAME);
	this->SetImageID(CSinglePlayerState::GetInstance()->GetWeaponID());
	this->SetRotation(CSinglePlayerState::GetInstance()->GetPlayerPointer()->GetRotation());
	this->m_fLifeTime = 0.1f;
}

CFlame::~CFlame(void)
{
}

void CFlame::Update(float fElapsedTime)
{
	CBase::Update(fElapsedTime);

	

	static float fAge = 0.0f;
	fAge += fElapsedTime;
	if(fAge > this->m_fLifeTime)
	{
		CGame::GetInstance()->GetMessageSystemPointer()->SendMsg( new CDestroyFlameMessage( this, this->GetOwner()) );
		fAge = 0.0f;
	}
}

void CFlame::Render(void)
{
	RECT rRender;
	rRender.top = 472;
	rRender.left = 556;
	rRender.bottom = 540;
	rRender.right = 762;

	CSGD_TextureManager::GetInstance()->Draw( GetImageID(), 
		(int)(((GetPosX() - (GetWidth()/2.0f) ) - CCamera::GetInstance()->GetOffsetX()) * CCamera::GetInstance()->GetScale()), 
		(int)(((GetPosY() - (GetHeight()/2.0f)) - CCamera::GetInstance()->GetOffsetY()) * CCamera::GetInstance()->GetScale()), 
		CCamera::GetInstance()->GetScale(), 
		CCamera::GetInstance()->GetScale(), 
		&rRender, (GetWidth()/2.0f), (GetHeight()/2.0f),
		this->GetRotation() );

	CSGD_Direct3D::GetInstance()->DrawLine( GetPosX()-4, GetPosY()-4, GetPosX()+4, GetPosY()+4, 255, 0, 0 );
	CSGD_Direct3D::GetInstance()->DrawLine( GetPosX()-4, GetPosY()+4, GetPosX()+4, GetPosY()-4, 255, 0, 0 );
}

RECT CFlame::GetRect(void) const
{
	RECT rCollision;
	rCollision.top = (LONG)( GetPosY() );
	rCollision.left = (LONG)( GetPosX() );
	rCollision.bottom = (LONG)( rCollision.top + GetHeight() );
	rCollision.right = rCollision.left + GetWidth();

	return rCollision;
}

bool CFlame::CheckCollision(CBase *pBase)
{
	RECT rIntersect;
	if( IntersectRect(&rIntersect, &GetRect(), &pBase->GetRect()) )
	{
		if(this->GetOwner()->GetType() == OBJ_PLAYER)
		{
			if( pBase->GetType() != OBJ_PLAYER && pBase->GetType() != OBJ_FLAME && pBase->GetType() != OBJ_SPAWNER)
			{
				// Destroy the bullet
				//CGame::GetInstance()->GetMessageSystemPointer()->SendMsg( new CDestroyFlameMessage( this, this->GetOwner()) );
			}
		}
		else if(this->GetOwner()->GetType() == OBJ_ENEMY)
		{
		}

		return 1;
	}
	else
		return 0;
}