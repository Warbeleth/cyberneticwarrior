#include "PrecompiledHeader.h"
#include "CBaseProjectile.h"
#include "CPlayer.h"
#include "CSinglePlayerState.h"
#include "CBaseEnemy.h"

CBaseProjectile::CBaseProjectile( void )
{
	this->SetRotation(CSinglePlayerState::GetInstance()->GetPlayerPointer()->GetRotation());
	m_nDamage = 0;
	m_pOwner = NULL;
}

CBaseProjectile::~CBaseProjectile( void )
{
}

void CBaseProjectile::Update( float fElapsedTime )
{
	CBase::Update( fElapsedTime );
}

void CBaseProjectile::Render( void )
{
	CSGD_TextureManager::GetInstance()->Draw( GetImageID(), 
		(int)(((GetPosX() - (GetWidth()/2.0f) ) - CCamera::GetInstance()->GetOffsetX()) * CCamera::GetInstance()->GetScale()), 
		(int)(((GetPosY() - (GetHeight()/2.0f)) - CCamera::GetInstance()->GetOffsetY()) * CCamera::GetInstance()->GetScale()), 
		CCamera::GetInstance()->GetScale(),
		CCamera::GetInstance()->GetScale(), 
		&rRender, (GetWidth()/2.0f), (GetHeight()/2.0f),
		GetRotation() );
}

RECT CBaseProjectile::GetRect( void )
{
	RECT rCollision;
	rCollision.top = (LONG)( GetPosY() );
	rCollision.left = (LONG)( GetPosX() );
	rCollision.bottom = (LONG)( rCollision.top + GetHeight() );
	rCollision.right = rCollision.left + GetWidth();

	return rCollision;
}

bool CBaseProjectile::CheckCollision(CBase *pBase)
{
	RECT rIntersect;
	if( IntersectRect(&rIntersect, &GetRect(), &pBase->GetRect()) && pBase->GetType() != OBJ_SPAWNER )
	{
		if(this->GetOwner()->GetType() == OBJ_PLAYER)
		{
			if(pBase->GetType() == OBJ_ENEMY)
			{
				CBaseEnemy* pEnemy = (CBaseEnemy*)pBase;
				(*pEnemy).SetCurrentHP( pEnemy->GetCurrentHP() - m_nDamage );
			}
		}
		else if(this->GetOwner()->GetType() == OBJ_ENEMY)
		{
			if(pBase->GetType() == OBJ_PLAYER)
			{
				CPlayer* pPlayer= (CPlayer*)pBase;
				(*pPlayer).DecrementHealth(m_nDamage);
			}
		}

		return true;
	}
		return false;
}