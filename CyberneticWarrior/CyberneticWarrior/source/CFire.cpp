#include "PrecompiledHeader.h"

#include "CFire.h"
#include "CGame.h"
#include "CSinglePlayerState.h"

CFire::CFire(void)
{
	this->SetType(OBJ_FIRE);
	this->SetImageID(CSinglePlayerState::GetInstance()->GetWeaponID());
	m_rRender.top = 610;
	m_rRender.left = 550;
	m_rRender.right = 610;
	m_rRender.bottom = 550;
}

CFire::~CFire(void)
{
}

void CFire::Update(float fElapsedTime)
{
	CBaseProjectile::Update( fElapsedTime );
	
	if( m_bDead )
		CGame::GetInstance()->GetMessageSystemPointer()->SendMsg( new CDestroyFireMessage( this, this->GetOwner()) );
}

bool CFire::CheckCollision(CBase *pBase)
{
	if(CBaseProjectile::CheckCollision( pBase ))
	{				
		// Destroy the bullet
		//CGame::GetInstance()->GetMessageSystemPointer()->SendMsg( new CDestroyFireMessage( this, CSinglePlayerState::GetInstance()->GetPlayerPointer()) );
		if(this->GetOwner()->GetType() == OBJ_ENEMY)
		{
			if(pBase->GetType() != OBJ_ENEMY)
				CGame::GetInstance()->GetMessageSystemPointer()->SendMsg( new CDestroyFireMessage( this, this->GetOwner()) );
		}
		else
		{
			CGame::GetInstance()->GetMessageSystemPointer()->SendMsg( new CDestroyFireMessage( this, this->GetOwner()) );
		}
		return true;
	}
	else
		return false;
}

