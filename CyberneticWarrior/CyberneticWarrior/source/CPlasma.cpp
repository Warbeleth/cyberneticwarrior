#include "PrecompiledHeader.h"

#include "CPlasma.h"
#include "CGame.h"
#include "CSinglePlayerState.h"
#include "CPlayer.h"

CPlasma::CPlasma(void)
{
	this->SetType(OBJ_PLASMA);
	this->SetImageID(CSinglePlayerState::GetInstance()->GetPlasmaID());
	this->SetRotation(0.0f);
}

CPlasma::~CPlasma(void)
{
}

void CPlasma::Update(float fElapsedTime)
{
	CBase::Update(fElapsedTime);
}

void CPlasma::Render(void)
{
	RECT rRender;
	rRender.top = 48;
	rRender.left = 660;
	rRender.right = 720;
	rRender.bottom = 105;
	CSGD_TextureManager::GetInstance()->Draw( GetImageID(), 
		(int)(((GetPosX() + (GetWidth()/2.0f) ) - CCamera::GetInstance()->GetOffsetX()) * CCamera::GetInstance()->GetScale()), 
		(int)(((GetPosY() - (GetHeight()/2.0f)) - CCamera::GetInstance()->GetOffsetY()) * CCamera::GetInstance()->GetScale()), 
		1.0f * CCamera::GetInstance()->GetScale(), 
		1.0f * CCamera::GetInstance()->GetScale(), 
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
		if( pBase->GetType() != OBJ_PLAYER )
		{
			// Destroy the bullet
			CGame::GetInstance()->GetMessageSystemPointer()->SendMsg( new CDestroyPlasmaMessage( this, CSinglePlayerState::GetInstance()->GetPlayerPointer()) );
		}

		return 1;
	}
	else
		return 0;
}