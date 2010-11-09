#include "PrecompiledHeader.h"

#include "CGrenade.h"
#include "CGame.h"
#include "CSinglePlayerState.h"
#include "CPlayer.h"
#include "CBlock.h"
#include "CBaseEnemy.h"

CGrenade::CGrenade(void)
{
	this->SetType(OBJ_GRENADE);
	this->SetImageID(CSinglePlayerState::GetInstance()->GetWeaponID());
	this->SetRotation(0.0f);
	this->m_nBounceCount = 0;
	this->m_fBoomTime = 5.0f;
	this->m_vVelocity.fY = 500.0f;
	this->m_fDirection = 1.0f;
}

CGrenade::~CGrenade(void)
{
}

void CGrenade::Update(float fElapsedTime)
{

	static float fAge = 0.0f;
	fAge += fElapsedTime;


	if(fAge > this->m_fBoomTime)
	{
		CGame::GetInstance()->GetMessageSystemPointer()->SendMsg( new CDestroyGrenadeMessage( this, this->GetOwner()) );
		fAge = 0.0f;
	}
	
	if(this->m_vVelocity.fY != 0.0f)
	{
		this->m_vVelocity.fY += 300*fElapsedTime;
		this->SetBaseVelY(this->m_vVelocity.fY);
	}

	static tVector2D vScreenDimensions;
	vScreenDimensions.fX = (float)CGame::GetInstance()->GetScreenWidth();
	vScreenDimensions.fY = (float)CGame::GetInstance()->GetScreenHeight();
	if(((this->GetPosX() + this->GetWidth()/2.0f) <= -20 
		|| ((this->GetPosX() - this->GetWidth()/2.0f) >= (CCamera::GetInstance()->GetOffsetX() + vScreenDimensions.fX + 20))
		|| (this->GetPosY() + (this->GetHeight()/2.0f)) <= -20)
		)//|| (this->GetPosY() - (this->GetHeight()/2.0f) >= (vScreenDimensions.fY+20)))
	{
		// destroy
		CGame::GetInstance()->GetMessageSystemPointer()->SendMsg(new CDestroyGrenadeMessage(this, this->GetOwner()));
	}
	
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
		this->m_fDirection * CCamera::GetInstance()->GetScale(), 
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
		if( pBase->GetType() == OBJ_BLOCK )
		{
			CBlock* BLOCK = (CBlock*)pBase;
			this->m_nBounceCount++;
			if(this->GetPosY() < BLOCK->GetPosY())
			{
				this->SetPosY(BLOCK->GetPosY()-this->GetHeight()-0.1f);
			}
			if(this->GetPosY() > BLOCK->GetPosY())
			{
				this->SetPosY(this->GetPosY()+this->GetHeight()+0.1f);
			}

			this->m_vVelocity.fY = -this->m_vVelocity.fY;

			// Destroy the bullet
			if(this->m_nBounceCount > 2)
			{
				CGame::GetInstance()->GetMessageSystemPointer()->SendMsg( new CDestroyGrenadeMessage( this, this->GetOwner()) );
				this->m_nBounceCount = 0;
			}
		}
		if(this->GetOwner()->GetType() == OBJ_PLAYER)
		{
			if(pBase->GetType() == OBJ_ENEMY && pBase->GetType() != OBJ_SPAWNER)
			{
				CBaseEnemy* pEnemy = (CBaseEnemy*)pBase; 
				this->SetPosX(pEnemy->GetPosX());
				this->SetPosY(pEnemy->GetPosY());
				this->SetBaseVelX(0.0f);
				this->SetBaseVelY(0.0f);
				this->m_vVelocity.fY = 0.0f;
			}
		}
		else if(this->GetOwner()->GetType() == OBJ_ENEMY)
		{
			if(pBase->GetType() == OBJ_PLAYER && pBase->GetType() != OBJ_SPAWNER)
			{
				this->SetPosX(pBase->GetPosX());
				this->SetPosY(pBase->GetPosY());
				this->SetBaseVelX(0.0f);
				this->SetBaseVelY(0.0f);
				this->m_vVelocity.fY = 0.0f;
			}
		}

		return 1;
	}
	else
		return 0;
}