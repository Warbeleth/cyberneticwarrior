#include "PrecompiledHeader.h"

#include "CGrenade.h"
#include "CGame.h"
#include "CSinglePlayerState.h"
#include "CPlayer.h"

CGrenade::CGrenade(void)
{
	this->SetType(OBJ_BULLET);
	this->SetImageID(CSinglePlayerState::GetInstance()->GetGrenadeID());
	this->SetRotation(0.0f);
}

CGrenade::~CGrenade(void)
{
}

void CGrenade::Update(float fElapsedTime)
{
	CBase::Update(fElapsedTime);
}

void CGrenade::Render(void)
{
	RECT rRender;
	rRender.top = 160;
	rRender.left = 970;
	rRender.right = 1000;
	rRender.bottom = 190;
	CSGD_TextureManager::GetInstance()->Draw( GetImageID(), 
		(int)(((GetPosX() + (GetWidth()/2.0f) ) - CCamera::GetInstance()->GetOffsetX()) * CCamera::GetInstance()->GetScale()), 
		(int)(((GetPosY() - (GetHeight()/2.0f)) - CCamera::GetInstance()->GetOffsetY()) * CCamera::GetInstance()->GetScale()), 
		1.0f * CCamera::GetInstance()->GetScale(), 
		1.0f * CCamera::GetInstance()->GetScale(), 
		&rRender, (GetWidth()/2.0f), (GetHeight()/2.0f),
		this->GetRotation() );
}

RECT CGrenade::GetRect(void) const
{
	RECT rCollision;
	rCollision.top = (LONG)( GetPosY() );
	rCollision.left = (LONG)( GetPosX() );
	rCollision.bottom = (LONG)( rCollision.top + GetHeight() );
	rCollision.right = rCollision.left + GetWidth();

	return rCollision;
}

bool CGrenade::CheckCollision(CBase *pBase)
{
	RECT rIntersect;
	if( IntersectRect(&rIntersect, &GetRect(), &pBase->GetRect()) )
	{
		if( pBase->GetType() != OBJ_PLAYER )
		{
			// Destroy the bullet
			CGame::GetInstance()->GetMessageSystemPointer()->SendMsg( new CDestroyGrenadeMessage( this, CSinglePlayerState::GetInstance()->GetPlayerPointer()) );
		}

		return 1;
	}
	else
		return 0;
}