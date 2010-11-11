#include "PrecompiledHeader.h"

#include "CShock.h"
#include "CGame.h"
#include "CSinglePlayerState.h"
#include "CPlayer.h"

CShock::CShock(void)
{
	this->SetType(OBJ_SHOCK);
	this->SetImageID(CSinglePlayerState::GetInstance()->GetWeaponID());
	this->SetRotation(CSinglePlayerState::GetInstance()->GetPlayerPointer()->GetRotation());
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
		)//|| (this->GetPosY() - (this->GetHeight()/2.0f) >= (vScreenDimensions.fY+20)))
	{
		// destroy
		CGame::GetInstance()->GetMessageSystemPointer()->SendMsg(new CDestroyShockMessage(this, this->GetOwner()));
	}
}

void CShock::Render(void)
{
	RECT rRender;
	rRender.top = 167;
	rRender.left = 677;
	rRender.right = 722;
	rRender.bottom = 279;
	CSGD_TextureManager::GetInstance()->Draw( GetImageID(), 
		(int)(((GetPosX() - (GetWidth()/2.0f) ) - CCamera::GetInstance()->GetOffsetX()) * CCamera::GetInstance()->GetScale()), 
		(int)(((GetPosY() - (GetHeight()/2.0f)) - CCamera::GetInstance()->GetOffsetY()) * CCamera::GetInstance()->GetScale()), 
		CCamera::GetInstance()->GetScale(), 
		CCamera::GetInstance()->GetScale(), 
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
		if(this->GetOwner()->GetType() == OBJ_PLAYER)
		{
			if( pBase->GetType() != OBJ_PLAYER && pBase->GetType() != OBJ_SHOCK && pBase->GetType() != OBJ_SPAWNER)
			{
				// Destroy the bullet
				CGame::GetInstance()->GetMessageSystemPointer()->SendMsg( new CDestroyShockMessage( this, this->GetOwner()) );
			}
		}
		else if(this->GetOwner()->GetType() == OBJ_ENEMY)
		{
			if( pBase->GetType() != OBJ_ENEMY && pBase->GetType() != OBJ_SHOCK && pBase->GetType() != OBJ_SPAWNER)
			{
				// Destroy the bullet
				CGame::GetInstance()->GetMessageSystemPointer()->SendMsg( new CDestroyShockMessage( this, this->GetOwner()) );
			}
		}

		return 1;
	}
	else
		return 0;
}