#include "PrecompiledHeader.h"

#include "CBullet.h"
#include "CGame.h"
#include "CSinglePlayerState.h"
#include "CPlayer.h"

CBullet::CBullet(void)
{
	this->SetType(OBJ_BULLET);
	this->SetImageID(CSinglePlayerState::GetInstance()->GetWeaponID());
	this->SetRotation(0.0f);
	if(CSinglePlayerState::GetInstance()->GetPlayerPointer()->GetForward())
	{
		this->m_fDirection = 1.0f;
	}
	else
	{
		this->m_fDirection = -1.0f;
	}
}

CBullet::~CBullet(void)
{
}

void CBullet::Update(float fElapsedTime)
{
	CBase::Update(fElapsedTime);


	

	static tVector2D vScreenDimensions;
	vScreenDimensions.fX = (float)CGame::GetInstance()->GetScreenWidth();
	vScreenDimensions.fY = (float)CGame::GetInstance()->GetScreenHeight();
	if(((this->GetPosX() + this->GetWidth()/2.0f) <= -20 
		|| ((this->GetPosX() - this->GetWidth()/2.0f) >= (CCamera::GetInstance()->GetOffsetX() + vScreenDimensions.fX + 20))
		|| (this->GetPosY() + (this->GetHeight()/2.0f)) <= -20))
//		|| (this->GetPosY() - (this->GetHeight()/2.0f) >= (vScreenDimensions.fY+20)))
	{
		// destroy
		CGame::GetInstance()->GetMessageSystemPointer()->SendMsg(new CDestroyBulletMessage(this, CSinglePlayerState::GetInstance()->GetPlayerPointer()));
	}
}

void CBullet::Render(void)
{
	RECT rRender;
	rRender.top = 240;
	rRender.left = 975;
	rRender.right = rRender.left + 32;
	rRender.bottom = rRender.top + 32;
	CSGD_TextureManager::GetInstance()->Draw( GetImageID(), 
		(int)(((GetPosX() + (GetWidth()/2.0f) ) - CCamera::GetInstance()->GetOffsetX()) * CCamera::GetInstance()->GetScale()), 
		(int)(((GetPosY() - (GetHeight()/2.0f)) - CCamera::GetInstance()->GetOffsetY()) * CCamera::GetInstance()->GetScale()), 
		this->m_fDirection * CCamera::GetInstance()->GetScale(), 
		1.0f * CCamera::GetInstance()->GetScale(), 
		&rRender, (GetWidth()/2.0f), (GetHeight()/2.0f),
		this->GetRotation() );
}

RECT CBullet::GetRect(void) const
{
	RECT rCollision;
	rCollision.top = (LONG)( GetPosY() );
	rCollision.left = (LONG)( GetPosX() );
	rCollision.bottom = (LONG)( rCollision.top + GetHeight() );
	rCollision.right = rCollision.left + GetWidth();

	return rCollision;
}

bool CBullet::CheckCollision(CBase *pBase)
{
	RECT rIntersect;
	if( IntersectRect(&rIntersect, &GetRect(), &pBase->GetRect()) )
	{
		if(this->GetOwnerType() == OBJ_PLAYER)
		{
			if( pBase->GetType() != OBJ_PLAYER && pBase->GetType() != OBJ_SPAWNER)
			{
				// Destroy the bullet
				CGame::GetInstance()->GetMessageSystemPointer()->SendMsg( new CDestroyBulletMessage( this, CSinglePlayerState::GetInstance()->GetPlayerPointer()) );
			}
		}
		else if(this->GetOwnerType() == OBJ_ENEMY)
		{
		}

		return 1;
	}
	else
		return 0;
}