#include "PrecompiledHeader.h"

#include "CGrenade.h"
#include "CGame.h"
#include "CSinglePlayerState.h"
#include "CPlayer.h"
#include "CBlock.h"

CGrenade::CGrenade(void)
{
	this->SetType(OBJ_GRENADE);
	this->SetImageID(CSinglePlayerState::GetInstance()->GetWeaponID());
	this->SetRotation(0.0f);
	this->m_nBounceCount = 0;
	this->m_fBoomTime = 5.0f;
	this->m_vVelocity.fY = 500.0f;
}

CGrenade::~CGrenade(void)
{
}

void CGrenade::Update(float fElapsedTime)
{
	CBase::Update(fElapsedTime);

	static float fAge = 0.0f;
	fAge += fElapsedTime;


	if(fAge > this->m_fBoomTime)
	{
		CGame::GetInstance()->GetMessageSystemPointer()->SendMsg( new CDestroyGrenadeMessage( this, CSinglePlayerState::GetInstance()->GetPlayerPointer()) );
		fAge = 0.0f;
	}
	
	if(this->m_vVelocity.fY != 0.0f)
	{
		this->m_vVelocity.fY += 300*fElapsedTime;
		this->SetBaseVelY(this->m_vVelocity.fY);
	}
	
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
		if( pBase->GetType() == OBJ_BLOCK )
		{
			CBlock* BLOCK = (CBlock*)pBase;
			this->m_nBounceCount++;
			if(this->GetPosY() < BLOCK->GetPosY())
			{
				this->SetPosY(this->GetPosY()-2.0f);
			}
			if(this->GetPosY() > BLOCK->GetPosY())
			{
				this->SetPosY(this->GetPosY()+2.0f);
			}

			this->m_vVelocity.fY = -this->m_vVelocity.fY;

			// Destroy the bullet
			if(this->m_nBounceCount > 2)
			{
				//CGame::GetInstance()->GetMessageSystemPointer()->SendMsg( new CDestroyGrenadeMessage( this, CSinglePlayerState::GetInstance()->GetPlayerPointer()) );
				this->m_nBounceCount = 0;
			}
		}

		return 1;
	}
	else
		return 0;
}