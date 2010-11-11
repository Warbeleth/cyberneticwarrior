#include "PrecompiledHeader.h"

#include "CPlasma.h"
#include "CGame.h"
#include "CSinglePlayerState.h"

CPlasma::CPlasma(void)
{
	this->SetType(OBJ_PLASMA);
	this->SetImageID(CSinglePlayerState::GetInstance()->GetWeaponID());
	rRender.top = 52;
	rRender.left = 671;
	rRender.right = 713;
	rRender.bottom = 100;
}

CPlasma::~CPlasma(void)
{
}

void CPlasma::Update(float fElapsedTime)
{
	CBaseProjectile::Update( fElapsedTime );

	static tVector2D vScreenDimensions;
	vScreenDimensions.fX = (float)CGame::GetInstance()->GetScreenWidth();
	vScreenDimensions.fY = (float)CGame::GetInstance()->GetScreenHeight();
	if(((this->GetPosX() + this->GetWidth()/2.0f) <= -20 
		|| ((this->GetPosX() - this->GetWidth()/2.0f) >= (CCamera::GetInstance()->GetOffsetX() + vScreenDimensions.fX + 20))
		|| (this->GetPosY() + (this->GetHeight()/2.0f)) <= -20)
		)//|| (this->GetPosY() - (this->GetHeight()/2.0f) >= (vScreenDimensions.fY+20)))
	{
		// destroy
		CGame::GetInstance()->GetMessageSystemPointer()->SendMsg(new CDestroyPlasmaMessage(this, this->GetOwner()));
	}
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