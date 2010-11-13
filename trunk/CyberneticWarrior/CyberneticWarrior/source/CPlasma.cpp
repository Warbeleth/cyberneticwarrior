#include "PrecompiledHeader.h"

#include "CPlasma.h"
#include "CGame.h"
#include "CSinglePlayerState.h"

CPlasma::CPlasma(void)
{
	this->SetType(OBJ_PLASMA);
	this->SetImageID(CSinglePlayerState::GetInstance()->GetWeaponID());
	m_rRender.top = 52;
	m_rRender.left = 671;
	m_rRender.right = 713;
	m_rRender.bottom = 100;
}

CPlasma::~CPlasma(void)
{
}

void CPlasma::Update(float fElapsedTime)
{
	CBaseProjectile::Update( fElapsedTime );

	if( m_bDead )
		CGame::GetInstance()->GetMessageSystemPointer()->SendMsg( new CDestroyPlasmaMessage( this, this->GetOwner()) );
		
}

bool CPlasma::CheckCollision(CBase *pBase)
{	
	if(CBaseProjectile::CheckCollision( pBase ))
	{				
		// Destroy the bullet
		if(this->GetOwner()->GetType() == OBJ_ENEMY)
		{
			if(pBase->GetType() != OBJ_ENEMY)
				CGame::GetInstance()->GetMessageSystemPointer()->SendMsg( new CDestroyPlasmaMessage( this, this->GetOwner()) );
		}
		else
		{
			CGame::GetInstance()->GetMessageSystemPointer()->SendMsg( new CDestroyPlasmaMessage( this, this->GetOwner()) );
		}
		return true;
	}
	else
		return false;
}