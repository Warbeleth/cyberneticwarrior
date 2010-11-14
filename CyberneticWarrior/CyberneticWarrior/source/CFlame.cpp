#include "PrecompiledHeader.h"

#include "CFlame.h"
#include "CGame.h"
#include "CSinglePlayerState.h"

CFlame::CFlame(void)
{
	this->SetType(OBJ_FLAME);
	this->SetImageID(CSinglePlayerState::GetInstance()->GetWeaponID());
	SetTime(0.25f);
	m_rRender.top = 472;
	m_rRender.left = 556;
	m_rRender.bottom = 540;
	m_rRender.right = 762;
}

CFlame::~CFlame(void)
{
}

void CFlame::Update(float fElapsedTime)
{
	CBaseProjectile::Update( fElapsedTime );

	if( m_bDead )
		CGame::GetInstance()->GetMessageSystemPointer()->SendMsg( new CDestroyFlameMessage( this, this->GetOwner()) );

}

bool CFlame::CheckCollision(CBase *pBase)
{
	if(CBaseProjectile::CheckCollision( pBase ))
	{	
		// Destroy the bullet
		//CGame::GetInstance()->GetMessageSystemPointer()->SendMsg( new CDestroyFlameMessage( this, this->GetOwner()) );
		if(this->GetOwner()->GetType() == OBJ_ENEMY)
		{
			//if(pBase->GetType() != OBJ_ENEMY)
			//	CGame::GetInstance()->GetMessageSystemPointer()->SendMsg( new CDestroyFlameMessage( this, this->GetOwner()) );
		}
		else
		{
			//CGame::GetInstance()->GetMessageSystemPointer()->SendMsg( new CDestroyFlameMessage( this, this->GetOwner()) );
		}
		return true;
	}
	else
		return false;
				
}