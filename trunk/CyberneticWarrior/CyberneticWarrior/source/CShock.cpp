#include "PrecompiledHeader.h"

#include "CShock.h"
#include "CGame.h"
#include "CSinglePlayerState.h"
#include "CPlayer.h"

CShock::CShock(void)
{
	this->SetType(OBJ_SHOCK);
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

CShock::~CShock(void)
{
}

void CShock::Update(float fElapsedTime)
{
	CBase::Update(fElapsedTime);

	
	static tVector2D vScreenDimensions;
	vScreenDimensions.fX = (float)CGame::GetInstance()->GetScreenWidth();
	vScreenDimensions.fY = (float)CGame::GetInstance()->GetScreenHeight();
	if(((this->GetPosX() + this->GetWidth()/2.0f) <= -20 
		|| ((this->GetPosX() - this->GetWidth()/2.0f) >= (CCamera::GetInstance()->GetOffsetX() + vScreenDimensions.fX + 20))
		|| (this->GetPosY() + (this->GetHeight()/2.0f)) <= -20)
		|| (this->GetPosY() - (this->GetHeight()/2.0f) >= (vScreenDimensions.fY+20)))
	{
		// destroy
		CGame::GetInstance()->GetMessageSystemPointer()->SendMsg(new CDestroyShockMessage(this, CSinglePlayerState::GetInstance()->GetPlayerPointer()));
	}
}

void CShock::Render(void)
{
	RECT rRender;
	rRender.top = 160;
	rRender.left = 680;
	rRender.right = 730;
	rRender.bottom = 280;
	CSGD_TextureManager::GetInstance()->Draw( GetImageID(), 
		(int)(((GetPosX() + (GetWidth()/2.0f) ) - CCamera::GetInstance()->GetOffsetX()) * CCamera::GetInstance()->GetScale()), 
		(int)(((GetPosY() - (GetHeight()/2.0f)) - CCamera::GetInstance()->GetOffsetY()) * CCamera::GetInstance()->GetScale()), 
		this->m_fDirection * CCamera::GetInstance()->GetScale(), 
		1.0f * CCamera::GetInstance()->GetScale(), 
		&rRender, (GetWidth()/2.0f), (GetHeight()/2.0f),
		this->GetRotation() );


}

RECT CShock::GetRect(void) const
{
	RECT rCollision;
	rCollision.top = (LONG)( GetPosY() + (GetWidth()/2.0f)  );
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
		if( pBase->GetType() != OBJ_PLAYER && pBase->GetType() != OBJ_SHOCK)
		{
			// Destroy the bullet
			CGame::GetInstance()->GetMessageSystemPointer()->SendMsg( new CDestroyShockMessage( this, CSinglePlayerState::GetInstance()->GetPlayerPointer()) );
		}

		return 1;
	}
	else
		return 0;
}