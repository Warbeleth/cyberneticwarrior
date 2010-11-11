#include "PrecompiledHeader.h"

#include "CIce.h"
#include "CGame.h"
#include "CSinglePlayerState.h"

CIce::CIce(void)
{
	this->SetType(OBJ_ICE);
	this->SetImageID(CSinglePlayerState::GetInstance()->GetWeaponID());
	this->m_fLifeTime = 1.0f;
	rRender.top = 621;
	rRender.left = 819;
	rRender.right = 868;
	rRender.bottom = 648;
}

CIce::~CIce(void)
{
}

void CIce::Update(float fElapsedTime)
{
	CBaseProjectile::Update( fElapsedTime );

	static float fAge = 0.0f;
	if(fAge > this->m_fLifeTime)
	{
		//CGame::GetInstance()->GetMessageSystemPointer()->SendMsg( new CDestroyIceMessage( this, CSinglePlayerState::GetInstance()->GetPlayerPointer()) );
		fAge = 0.0f;
	}
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