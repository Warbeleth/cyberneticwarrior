#include "PrecompiledHeader.h"

#include "CFlame.h"
#include "CGame.h"
#include "CSinglePlayerState.h"

CFlame::CFlame(void)
{
	this->SetType(OBJ_FLAME);
	this->SetImageID(CSinglePlayerState::GetInstance()->GetWeaponID());
	this->m_fLifeTime = 0.1f;
	rRender.top = 472;
	rRender.left = 556;
	rRender.bottom = 540;
	rRender.right = 762;
}

CFlame::~CFlame(void)
{
}

void CFlame::Update(float fElapsedTime)
{
	CBaseProjectile::Update( fElapsedTime );

	static float fAge = 0.0f;
	fAge += fElapsedTime;
	if(fAge > this->m_fLifeTime)
	{
		CGame::GetInstance()->GetMessageSystemPointer()->SendMsg( new CDestroyFlameMessage( this, this->GetOwner()) );
		fAge = 0.0f;
	}
}

bool CFlame::CheckCollision(CBase *pBase)
{
	if(CBaseProjectile::CheckCollision( pBase ))
	{	
		// Destroy the bullet
		//CGame::GetInstance()->GetMessageSystemPointer()->SendMsg( new CDestroyFlameMessage( this, this->GetOwner()) );
		if(this->GetOwner()->GetType() == OBJ_ENEMY)
		{
			if(pBase->GetType() != OBJ_ENEMY)
				CGame::GetInstance()->GetMessageSystemPointer()->SendMsg( new CDestroyFlameMessage( this, this->GetOwner()) );
		}
		else
		{
			CGame::GetInstance()->GetMessageSystemPointer()->SendMsg( new CDestroyFlameMessage( this, this->GetOwner()) );
		}
		return true;
	}
	else
		return false;
				
}