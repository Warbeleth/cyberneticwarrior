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
	CSGD_TextureManager::GetInstance()->Draw( GetImageID(), 
		(int)(((GetPosX() - (GetWidth()/2.0f) ) - CCamera::GetInstance()->GetOffsetX()) * CCamera::GetInstance()->GetScale()), 
		(int)(((GetPosY() - (GetHeight()/2.0f)) - CCamera::GetInstance()->GetOffsetY()) * CCamera::GetInstance()->GetScale()), 
		CCamera::GetInstance()->GetScale(),
		CCamera::GetInstance()->GetScale(), 
		&m_rRender, (GetWidth()/2.0f), (GetHeight()/2.0f),
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
	if( GetType() == pBase->GetType() || GetOwner()->GetType() == pBase->GetType() )
		return false;

	/*if( pBase->GetType() == OBJ_ENEMY)
	{
		RECT rcollision;
		 vector<RECT> basecollision = (*pBase->GetAnimations()->GetCollisionRects());
		 RECT test = GetRect();
		for( unsigned int i = 0; i < pBase->GetAnimations()->GetCollisionRects()->size(); ++i )
		{
			RECT rect = basecollision[i];
			rect.left -= pBase->GetAnimations()->GetFrame().left;
			rect.right -= pBase->GetAnimations()->GetFrame().left;
			rect.top -= pBase->GetAnimations()->GetFrame().top;
			rect.bottom -= pBase->GetAnimations()->GetFrame().top;

			rect.left += test.left;
			rect.right += test.left;
			rect.top += test.top;
			rect.bottom += test.top;

			if(IntersectRect( &rcollision, &rect, &GetRect()))
				return true;
		}
	}
	return false;*/
		/*if(pBase->GetAnimations() && pBase->GetType() != OBJ_SPAWNER )
		{
			if(pBase->GetAnimations()->CheckCollision(this, pBase->GetPosX(), pBase->GetPosY()))
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
		}
		else */
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
						(*pPlayer).DecrementHealth(m_nDamage);
					}
				}

				return true;
			}
		}
	return false;
}