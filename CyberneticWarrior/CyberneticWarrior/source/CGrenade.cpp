#include "PrecompiledHeader.h"

#include "CGrenade.h"
#include "CGame.h"
#include "CSinglePlayerState.h"
#include "CBlock.h"
#include "CBaseEnemy.h"

CGrenade::CGrenade(void)
{
	SetType(OBJ_GRENADE);
	SetImageID(CSinglePlayerState::GetInstance()->GetWeaponID());
	m_nBounceCount = 0;
	m_fBounceCooldownTimer = 0.0f;
	m_bJustBounced = false;
	m_bStuckToTarget = false;
	m_pTarget = NULL;
	SetDamage( 25 );

	m_rRender.top = 161;
	m_rRender.left = 970;
	m_rRender.right = 995;
	m_rRender.bottom = 186;
}

CGrenade::~CGrenade(void)
{
}

void CGrenade::Update(float fElapsedTime)
{
	CBaseProjectile::Update( fElapsedTime );

	if( m_bDead )
	{		
		if( m_pTarget )
		{
			if(m_pTarget->GetType() == OBJ_PLAYER)
			{
				CPlayer*  pPlayer = (CPlayer*)m_pTarget;
				(*pPlayer).DecrementHealth((float)GetDamage());
			}
			else if(m_pTarget->GetType() == OBJ_ENEMY)
			{
				CBaseEnemy*  pEnemy = (CBaseEnemy*)m_pTarget;
				(*pEnemy).SetCurrentHP( pEnemy->GetCurrentHP()-GetDamage() );
			}
		}

		CGame::GetInstance()->GetMessageSystemPointer()->SendMsg( new CDestroyGrenadeMessage( this, this->GetOwner()) );
	}
	
	if(!m_bStuckToTarget)
	{
		SetBaseVelY( GetBaseVelY() + 300*fElapsedTime );

		if(m_bJustBounced)
			m_fBounceCooldownTimer += fElapsedTime;

		if(m_fBounceCooldownTimer >= BOUNCE_COOLDOWN)
		{
			m_bJustBounced = false;
			m_fBounceCooldownTimer = 0.0f;
		}
		
	}
	else
	{
			if(m_pTarget->GetType() == OBJ_PLAYER)
			{
				CPlayer*  pPlayer = (CPlayer*)m_pTarget;
				
				if(pPlayer->GetHealth() <= 0)
					CGame::GetInstance()->GetMessageSystemPointer()->SendMsg( new CDestroyGrenadeMessage( this, this->GetOwner()) );

				SetBaseVelX( (*pPlayer).GetBaseVelX() );
				SetBaseVelY( (*pPlayer).GetBaseVelY() );
			}
			else if(m_pTarget->GetType() == OBJ_ENEMY)
			{
				CBaseEnemy*  pEnemy = (CBaseEnemy*)m_pTarget;
				
				if(pEnemy->GetCurrentHP() <= 0)
					CGame::GetInstance()->GetMessageSystemPointer()->SendMsg( new CDestroyGrenadeMessage( this, this->GetOwner()) );

				SetBaseVelX( (*pEnemy).GetSpeed() );
				SetBaseVelY( 0 );
			}
	}
}


bool CGrenade::CheckCollision(CBase *pBase)
{
	if( GetType() == pBase->GetType() || pBase->GetType() == GetOwner()->GetType() )
		return false;

	if(!m_bJustBounced && !m_bStuckToTarget)
	{
		RECT rIntersect;
		if( IntersectRect(&rIntersect, &GetRect(), &pBase->GetRect()) )
		{
			if( pBase->GetType() == OBJ_BLOCK )
			{
				m_nBounceCount++;
				m_bJustBounced = true;

				SetBaseVelY( -GetBaseVelY() );

				// Destroy the bullet
				if(this->m_nBounceCount > 3)
					CGame::GetInstance()->GetMessageSystemPointer()->SendMsg( new CDestroyGrenadeMessage( this, this->GetOwner()) );
			}
			else if( pBase->GetType() != OBJ_SPAWNER)
			{
				m_pTarget = pBase;
				m_bStuckToTarget = true;
			}
			return true;
		}
	}
	return false;
}