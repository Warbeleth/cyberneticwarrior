#include "PrecompiledHeader.h"

#include "CGrenade.h"
#include "CGame.h"
#include "CSinglePlayerState.h"
#include "CBlock.h"
#include "CBaseEnemy.h"

CGrenade::CGrenade(void)
{
	this->SetType(OBJ_GRENADE);
	this->SetImageID(CSinglePlayerState::GetInstance()->GetWeaponID());
	this->m_nBounceCount = 0;
	this->m_fBoomTime = 5.0f;
	this->m_vVelocity.fY = 500.0f;
	
	rRender.top = 161;
	rRender.left = 970;
	rRender.right = 995;
	rRender.bottom = 186;
}

CGrenade::~CGrenade(void)
{
}

void CGrenade::Update(float fElapsedTime)
{
	CBaseProjectile::Update( fElapsedTime );

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
			if(pBase->GetType() == OBJ_PLAYER 
				&& pBase->GetType() != OBJ_SPAWNER 
				&& pBase->GetType() != OBJ_ENEMY)
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