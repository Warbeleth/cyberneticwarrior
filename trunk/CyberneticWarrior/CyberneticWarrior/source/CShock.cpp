#include "PrecompiledHeader.h"

#include "CShock.h"
#include "CGame.h"
#include "CSinglePlayerState.h"
#include "CPlayer.h"

CShock::CShock(void)
{
	this->SetType(OBJ_SHOCK);
	this->SetImageID(CSinglePlayerState::GetInstance()->GetWeaponID());
	SetDamage((int)CSinglePlayerState::GetInstance()->GetPlayerPointer()->GetCharge()*500);
	CSinglePlayerState::GetInstance()->GetPlayerPointer()->SetCharge(0);
	m_rRender.top = 167;
	m_rRender.left = 677;
	m_rRender.right = 722;
	m_rRender.bottom = 279;
}

CShock::~CShock(void)
{
}

void CShock::Update(float fElapsedTime)
{
	CBaseProjectile::Update( fElapsedTime );
	
	if( m_bDead )
		CGame::GetInstance()->GetMessageSystemPointer()->SendMsg( new CDestroyShockMessage( this, this->GetOwner()) );
}

bool CShock::CheckCollision(CBase *pBase)
{
	if(CBaseProjectile::CheckCollision( pBase ))
	{	
		// Destroy the bullet
		if(this->GetOwner()->GetType() == OBJ_ENEMY)
		{
			if(pBase->GetType() != OBJ_ENEMY)
				CGame::GetInstance()->GetMessageSystemPointer()->SendMsg( new CDestroyShockMessage( this, this->GetOwner()) );
		}
		else
		{
			CGame::GetInstance()->GetMessageSystemPointer()->SendMsg( new CDestroyShockMessage( this, this->GetOwner()) );
		}
		return true;
	}
	else
		return false;
}