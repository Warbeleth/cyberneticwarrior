#include "PrecompiledHeader.h"

#include "CFlame.h"
#include "CGame.h"
#include "CSinglePlayerState.h"
#include "CPlayer.h"

CFlame::CFlame(void)
{
	this->SetType(OBJ_FLAME);
	this->SetImageID(CSinglePlayerState::GetInstance()->GetWeaponID());
	this->SetRotation(0.0f);
	this->m_fLifeTime = 0.1f;
	if(CSinglePlayerState::GetInstance()->GetPlayerPointer()->GetForward())
	{
		this->m_fDirection = 1.0f;
	}
	else
	{
		this->m_fDirection = -1.0f;
	}
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
		CGame::GetInstance()->GetMessageSystemPointer()->SendMsg( new CDestroyFlameMessage( this, CSinglePlayerState::GetInstance()->GetPlayerPointer()) );
		fAge = 0.0f;
	}
}

void CFlame::Render(void)
{
	RECT rRender;
	rRender.top = 464;
	rRender.left = 550;
	rRender.bottom = 545;
	rRender.right = 765;
	CSGD_TextureManager::GetInstance()->Draw( GetImageID(), 
		(int)(((GetPosX() + this->m_fDirection*(GetWidth()/2.0f) ) - CCamera::GetInstance()->GetOffsetX()) * CCamera::GetInstance()->GetScale()), 
		(int)(((GetPosY() - (GetHeight()/2.0f)) - CCamera::GetInstance()->GetOffsetY()) * CCamera::GetInstance()->GetScale()), 
		this->m_fDirection * CCamera::GetInstance()->GetScale(), 
		1.0f * CCamera::GetInstance()->GetScale(), 
		&rRender, 45.0f, 10.0f, this->GetRotation() );


	/*RECT rLALA;
	rLALA.top = (LONG)GetPosY()- CCamera::GetInstance()->GetOffsetY();
	rLALA.left = (LONG)GetPosX()- CCamera::GetInstance()->GetOffsetX();
	rLALA.bottom = rLALA.top + GetHeight();
	rLALA.right = rLALA.left + GetWidth();
	CSGD_Direct3D::GetInstance()->GetSprite()->Flush();
	CSGD_Direct3D::GetInstance()->DrawRect(rLALA,255,255,255);*/
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
		if( pBase->GetType() != OBJ_PLAYER && pBase->GetType() != OBJ_FLAME)
		{
			// Destroy the bullet
			CGame::GetInstance()->GetMessageSystemPointer()->SendMsg( new CDestroyFlameMessage( this, CSinglePlayerState::GetInstance()->GetPlayerPointer()) );
		}

		return 1;
	}
	else
		return 0;
}