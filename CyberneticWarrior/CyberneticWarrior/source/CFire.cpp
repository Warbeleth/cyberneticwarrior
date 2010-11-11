#include "PrecompiledHeader.h"

#include "CFire.h"
#include "CGame.h"
#include "CSinglePlayerState.h"
#include "CPlayer.h"

CFire::CFire(void)
{
	this->SetType(OBJ_FIRE);
	this->SetImageID(CSinglePlayerState::GetInstance()->GetWeaponID());
	this->SetRotation(CSinglePlayerState::GetInstance()->GetPlayerPointer()->GetRotation());
}

CFire::~CFire(void)
{
}

void CFire::Update(float fElapsedTime)
{
	CBase::Update(fElapsedTime);
}

void CFire::Render(void)
{
	RECT rRender;
	rRender.top = 610;
	rRender.left = 550;
	rRender.right = 610;
	rRender.bottom = 550;
	CSGD_TextureManager::GetInstance()->Draw( GetImageID(), 
		(int)(((GetPosX() - (GetWidth()/2.0f) ) - CCamera::GetInstance()->GetOffsetX()) * CCamera::GetInstance()->GetScale()), 
		(int)(((GetPosY() - (GetHeight()/2.0f)) - CCamera::GetInstance()->GetOffsetY()) * CCamera::GetInstance()->GetScale()), 
		CCamera::GetInstance()->GetScale(), 
		CCamera::GetInstance()->GetScale(), 
		&rRender, (GetWidth()/2.0f), (GetHeight()/2.0f),
		this->GetRotation() );
		
	CSGD_Direct3D::GetInstance()->DrawLine( GetPosX()-4, GetPosY()-4, GetPosX()+4, GetPosY()+4, 255, 0, 0 );
	CSGD_Direct3D::GetInstance()->DrawLine( GetPosX()-4, GetPosY()+4, GetPosX()+4, GetPosY()-4, 255, 0, 0 );
}

RECT CFire::GetRect(void) const
{
	RECT rCollision;
	rCollision.top = (LONG)( GetPosY() );
	rCollision.left = (LONG)( GetPosX() );
	rCollision.bottom = (LONG)( rCollision.top + GetHeight() );
	rCollision.right = rCollision.left + GetWidth();

	return rCollision;
}

bool CFire::CheckCollision(CBase *pBase)
{
	RECT rIntersect;
	if( IntersectRect(&rIntersect, &GetRect(), &pBase->GetRect()) )
	{
		if( pBase->GetType() != OBJ_PLAYER )
		{
			// Destroy the bullet
			//CGame::GetInstance()->GetMessageSystemPointer()->SendMsg( new CDestroyFireMessage( this, CSinglePlayerState::GetInstance()->GetPlayerPointer()) );
		}

		return 1;
	}
	else
		return 0;
}