#include "PrecompiledHeader.h"

#include "CIce.h"
#include "CGame.h"
#include "CSinglePlayerState.h"

CIce::CIce(void)
{
	this->SetType(OBJ_ICE);
	this->SetImageID(CSinglePlayerState::GetInstance()->GetWeaponID());
	this->m_fLifeTime = 1.0f;
	m_rRender.top = 621;
	m_rRender.left = 819;
	m_rRender.right = 868;
	m_rRender.bottom = 648;
}

CIce::~CIce(void)
{
}

void CIce::Update(float fElapsedTime)
{
	CBaseProjectile::Update( fElapsedTime );

	if( m_bDead )
		CGame::GetInstance()->GetMessageSystemPointer()->SendMsg( new CDestroyIceMessage( this, this->GetOwner()) );
		
}

bool CIce::CheckCollision(CBase *pBase)
{
	if(CBaseProjectile::CheckCollision( pBase ))
	{				
		// Destroy the bullet
		if(this->GetOwner()->GetType() == OBJ_ENEMY)
		{
			if(pBase->GetType() != OBJ_ENEMY)
				CGame::GetInstance()->GetMessageSystemPointer()->SendMsg( new CDestroyIceMessage( this, this->GetOwner()) );
		}
		else
		{
			CGame::GetInstance()->GetMessageSystemPointer()->SendMsg( new CDestroyIceMessage( this, this->GetOwner()) );
		}
		return true;
	}
	else
		return false;
}



