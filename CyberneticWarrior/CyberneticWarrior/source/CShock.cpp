#include "PrecompiledHeader.h"

#include "CShock.h"
#include "CGame.h"
#include "CSinglePlayerState.h"
#include "CPlayer.h"

CShock::CShock(void)
{
	this->SetType(OBJ_BULLET);
	this->SetImageID(CSinglePlayerState::GetInstance()->GetShockID());
	this->SetRotation(0.0f);
}

CShock::~CShock(void)
{
}

void CShock::Update(float fElapsedTime)
{
	CBase::Update(fElapsedTime);
}

void CShock::Render(void)
{
	RECT rRender;
	rRender.top = 160;
	rRender.left = 670;
	rRender.right = 730;
	rRender.bottom = 290;
	CSGD_TextureManager::GetInstance()->Draw( GetImageID(), 
		(int)(((GetPosX() + (GetWidth()/2.0f) ) - CCamera::GetInstance()->GetOffsetX()) * CCamera::GetInstance()->GetScale()), 
		(int)(((GetPosY() - (GetHeight()/2.0f)) - CCamera::GetInstance()->GetOffsetY()) * CCamera::GetInstance()->GetScale()), 
		1.0f * CCamera::GetInstance()->GetScale(), 
		1.0f * CCamera::GetInstance()->GetScale(), 
		&rRender, (GetWidth()/2.0f), (GetHeight()/2.0f),
		this->GetRotation() );
}

RECT CShock::GetRect(void) const
{
	RECT rCollision;
	rCollision.top = (LONG)( GetPosY() );
	rCollision.left = (LONG)( GetPosX() );
	rCollision.bottom = (LONG)( rCollision.top + GetHeight() );
	rCollision.right = rCollision.left + GetWidth();

	return rCollision;
}

bool CShock::CheckCollision(CBase *pBase)
{
	RECT rIntersect;
	if( IntersectRect(&rIntersect, &GetRect(), &pBase->GetRect()) )
	{
		if( pBase->GetType() != OBJ_PLAYER )
		{
			// Destroy the bullet
			CGame::GetInstance()->GetMessageSystemPointer()->SendMsg( new CDestroyShockMessage( this, CSinglePlayerState::GetInstance()->GetPlayerPointer()) );
		}

		return 1;
	}
	else
		return 0;
}