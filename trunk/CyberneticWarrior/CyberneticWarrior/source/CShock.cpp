#include "PrecompiledHeader.h"

#include "CShock.h"
#include "CGame.h"
#include "CSinglePlayerState.h"

CShock::CShock(void)
{
	this->SetType(OBJ_SHOCK);
	this->SetImageID(CSinglePlayerState::GetInstance()->GetWeaponID());
	
	rRender.top = 167;
	rRender.left = 677;
	rRender.right = 722;
	rRender.bottom = 279;
}

CShock::~CShock(void)
{
}

void CShock::Update(float fElapsedTime)
{
	CBaseProjectile::Update( fElapsedTime );

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

bool CShock::CheckCollision(CBase *pBase)
{
	if(CBaseProjectile::CheckCollision( pBase ))
	{	
		// Destroy the bullet
		if(this->GetOwner()->GetType() == OBJ_ENEMY)
		{
			if(pBase->GetType() != OBJ_ENEMY)
				CGame::GetInstance()->GetMessageSystemPointer()->SendMsg( new CDestroyShockMessage( this, this->GetOwner()) );
		}
		else
		{
			CGame::GetInstance()->GetMessageSystemPointer()->SendMsg( new CDestroyShockMessage( this, this->GetOwner()) );
		}
		return true;
	}
	else
		return false;
}