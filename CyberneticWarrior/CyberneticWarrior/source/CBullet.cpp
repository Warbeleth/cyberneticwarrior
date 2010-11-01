#include "PrecompiledHeader.h"

#include "CBullet.h"
#include "CGame.h"
#include "CSinglePlayerState.h"
#include "CPlayer.h"

CBullet::CBullet(void)
{
	this->SetType(OBJ_BULLET);
//	this->SetImageID(CSinglePlayerState::GetInstance()->GetBulletID());
	this->SetRotation(0.0f);
}

CBullet::~CBullet(void)
{
}

void CBullet::Update(float fElapsedTime)
{
	CBase::Update(fElapsedTime);
}

void CBullet::Render(void)
{
	RECT rRender;
	rRender.top = 235;
	rRender.left = 970;
	rRender.right = 990;
	rRender.bottom = 250;
	CSGD_TextureManager::GetInstance()->Draw( GetImageID(), 
		(int)(((GetPosX() + (GetWidth()/2.0f) ) - CCamera::GetInstance()->GetOffsetX()) * CCamera::GetInstance()->GetScale()), 
		(int)(((GetPosY() - (GetHeight()/2.0f)) - CCamera::GetInstance()->GetOffsetY()) * CCamera::GetInstance()->GetScale()), 
		1.0f * CCamera::GetInstance()->GetScale(), 
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
		if( pBase->GetType() != OBJ_PLAYER )
		{
			// Destroy the bullet
			CGame::GetInstance()->GetMessageSystemPointer()->SendMsg( new CDestroyBulletMessage( this, CSinglePlayerState::GetInstance()->GetPlayerPointer()) );
		}

		return 1;
	}
	else
		return 0;
}