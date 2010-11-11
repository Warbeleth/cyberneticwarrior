#include "PrecompiledHeader.h"

#include "CBullet.h"
#include "CGame.h"
#include "CSinglePlayerState.h"

CBullet::CBullet(void)
{
	this->SetType(OBJ_BULLET);
	this->SetImageID(CSinglePlayerState::GetInstance()->GetWeaponID());
	this->SetDamage(100);
	rRender.top = 239;
	rRender.left = 976;
	rRender.right = 985;
	rRender.bottom = 245;
}

CBullet::~CBullet(void)
{
}

void CBullet::Update(float fElapsedTime)
{
	CBaseProjectile::Update( fElapsedTime );

	static tVector2D vScreenDimensions;
	vScreenDimensions.fX = (float)CGame::GetInstance()->GetScreenWidth();
	vScreenDimensions.fY = (float)CGame::GetInstance()->GetScreenHeight();
	if(((this->GetPosX() + this->GetWidth()/2.0f) <= -20 
		|| ((this->GetPosX() - this->GetWidth()/2.0f) >= (CCamera::GetInstance()->GetOffsetX() + vScreenDimensions.fX + 20))
		|| (this->GetPosY() + (this->GetHeight()/2.0f)) <= -20))
//		|| (this->GetPosY() - (this->GetHeight()/2.0f) >= (vScreenDimensions.fY+20)))
	{
		// destroy
		CGame::GetInstance()->GetMessageSystemPointer()->SendMsg(new CDestroyBulletMessage(this, this->GetOwner()));
	}
}

bool CBullet::CheckCollision(CBase *pBase)
{
	if(CBaseProjectile::CheckCollision( pBase ))
	{				
		// Destroy the bullet
		if(this->GetOwner()->GetType() == OBJ_ENEMY)
		{
			if(pBase->GetType() != OBJ_ENEMY)
				CGame::GetInstance()->GetMessageSystemPointer()->SendMsg( new CDestroyBulletMessage( this, this->GetOwner()) );
		}
		else
		{
			CGame::GetInstance()->GetMessageSystemPointer()->SendMsg( new CDestroyBulletMessage( this, this->GetOwner()) );
		}
		return true;
	}
	else
		return false;
}