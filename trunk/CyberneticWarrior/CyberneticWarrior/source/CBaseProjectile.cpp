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
	SetAnimations(NULL);
	m_fAge = 0.0f;
	m_fDeathTime = 7.0f;
	m_bDead = false;
}

CBaseProjectile::~CBaseProjectile( void )
{
}

void CBaseProjectile::Update( float fElapsedTime )
{
	CBase::Update( fElapsedTime );
	
	m_fAge += fElapsedTime;

	if( m_fAge > m_fDeathTime )
		m_bDead = true;
}

void CBaseProjectile::Render( void )
{
	if(GetAnimations())
	{
		GetAnimations()->Render( (int)(((GetPosX() - (GetWidth()/2.0f) ) - CCamera::GetInstance()->GetOffsetX()) * CCamera::GetInstance()->GetScale()), 
			(int)(((GetPosY() - (GetHeight()/2.0f)) - CCamera::GetInstance()->GetOffsetY()) * CCamera::GetInstance()->GetScale()));
	}
	else
	{
		CSGD_TextureManager::GetInstance()->Draw( GetImageID(), 
			(int)(((GetPosX() - (GetWidth()/2.0f) ) - CCamera::GetInstance()->GetOffsetX()) * CCamera::GetInstance()->GetScale()), 
			(int)(((GetPosY() - (GetHeight()/2.0f)) - CCamera::GetInstance()->GetOffsetY()) * CCamera::GetInstance()->GetScale()), 
			CCamera::GetInstance()->GetScale(),
			CCamera::GetInstance()->GetScale(), 
			&m_rRender, (GetWidth()/2.0f), (GetHeight()/2.0f),
			GetRotation() );
	}
}

RECT CBaseProjectile::GetRect( void )
{
	RECT rCollision;
	rCollision.top = (LONG)( GetPosY() - GetHeight()/2 );
	rCollision.left = (LONG)( GetPosX() - GetWidth()/2 );
	rCollision.bottom = (LONG)( rCollision.top + GetHeight() );
	rCollision.right = rCollision.left + GetWidth();

	return rCollision;
}

bool CBaseProjectile::CheckCollision(CBase *pBase)
{
	if( (this->GetType() != OBJ_ROCKET && GetType() == pBase->GetType()) || GetOwner()->GetType() == pBase->GetType() )
		return false;

	if( pBase->GetType() != OBJ_SPAWNER )
		{
			RECT rIntersect;
			if( IntersectRect(&rIntersect, &GetRect(), &pBase->GetRect()) )
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
						(*pPlayer).DecrementHealth((float)m_nDamage);
					}
				}

				return true;
			}
		}
	return false;
}