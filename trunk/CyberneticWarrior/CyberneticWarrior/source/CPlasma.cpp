#include "PrecompiledHeader.h"

#include "CPlasma.h"
#include "CGame.h"
#include "CSinglePlayerState.h"
#include "CPlayer.h"

CPlasma::CPlasma(void)
{
	this->SetType(OBJ_PLASMA);
	this->SetImageID(CSinglePlayerState::GetInstance()->GetWeaponID());
	this->SetRotation(CSinglePlayerState::GetInstance()->GetPlayerPointer()->GetRotation());
}

CPlasma::~CPlasma(void)
{
}

void CPlasma::Update(float fElapsedTime)
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
		CGame::GetInstance()->GetMessageSystemPointer()->SendMsg(new CDestroyPlasmaMessage(this, this->GetOwner()));
	}
}

void CPlasma::Render(void)
{
	RECT rRender;
	rRender.top = 52;
	rRender.left = 671;
	rRender.right = 713;
	rRender.bottom = 100;
	CSGD_TextureManager::GetInstance()->Draw( GetImageID(), 
		(int)(((GetPosX() - (GetWidth()/2.0f) ) - CCamera::GetInstance()->GetOffsetX()) * CCamera::GetInstance()->GetScale()), 
		(int)(((GetPosY() - (GetHeight()/2.0f)) - CCamera::GetInstance()->GetOffsetY()) * CCamera::GetInstance()->GetScale()), 
		CCamera::GetInstance()->GetScale(), 
		CCamera::GetInstance()->GetScale(), 
		&rRender, (GetWidth()/2.0f), (GetHeight()/2.0f),
		this->GetRotation() );
}

RECT CPlasma::GetRect(void) const
{
	RECT rCollision;
	rCollision.top = (LONG)( GetPosY() );
	rCollision.left = (LONG)( GetPosX() );
	rCollision.bottom = (LONG)( rCollision.top + GetHeight() );
	rCollision.right = rCollision.left + GetWidth();

	return rCollision;
}

bool CPlasma::CheckCollision(CBase *pBase)
{
	RECT rIntersect;
	if( IntersectRect(&rIntersect, &GetRect(), &pBase->GetRect()) )
	{
		if(this->GetOwner()->GetType() == OBJ_PLAYER)
		{
			if( pBase->GetType() != OBJ_PLAYER && pBase->GetType() != OBJ_PLASMA  && pBase->GetType() != OBJ_SPAWNER)
			{
				// Destroy the bullet
				CGame::GetInstance()->GetMessageSystemPointer()->SendMsg( new CDestroyPlasmaMessage( this, this->GetOwner()) );
			}
		}
		else if(this->GetOwner()->GetType() == OBJ_ENEMY)
		{
			if( pBase->GetType() != OBJ_ENEMY && pBase->GetType() != OBJ_PLASMA  && pBase->GetType() != OBJ_SPAWNER)
			{
				// Destroy the bullet
				CGame::GetInstance()->GetMessageSystemPointer()->SendMsg( new CDestroyPlasmaMessage( this, this->GetOwner()) );
			}
		}

		return 1;
	}
	else
		return 0;
}