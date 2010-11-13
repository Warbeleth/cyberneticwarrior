#include "PrecompiledHeader.h"

#include "CAppleMech.h"
#include "CCamera.h"

#include "CGame.h"
#include "CMapLoad.h"

CAppleMech::CAppleMech()
{
}
CAppleMech::CAppleMech(int nImageID, float PosX, float PosY,int Width, int Height, int nState,
				int nMaxHP, int nCurrentHP, int nSightRange, int nAttackRange, int nGlobalType, float fRateOfFire, 
				float fSpeed) : CIdleEnemy(nState, nGlobalType, nImageID, nMaxHP, nCurrentHP, nSightRange, 
				nAttackRange, fRateOfFire, fSpeed, PosX, PosY, Width, Height)
{
	SetAnimations(CMapLoad::GetInstance()->CreateAnimation(Boss_Apple));
	GetAnimations()->SetCurrentAnimation(1);
	this->SetShotDelay(0.0f);
}
CAppleMech::~CAppleMech()
{

}

void CAppleMech::Update(float fElapsedTime)
{
	CIdleEnemy::Update(fElapsedTime);

	switch(ReturnAIState())
	{
	case Idle:
		GetAnimations()->SetCurrentAnimation(1);
		break;
	case iActive:
		GetAnimations()->SetCurrentAnimation(0);
		if(!GetAnimations()->SameFrame() > GetAnimations()->GetTrigger() != 0)
		{
			CGame::GetInstance()->GetMessageSystemPointer()->SendMsg(new CCreatePlasmaMessage(this));
		}
		break;
	case iDead:
		CGame::GetInstance()->GetMessageSystemPointer()->SendMsg(new CDestroyEnemyMessage((CBaseEnemy*)this));
		break;
	};
}

void CAppleMech::Render()
{
	CIdleEnemy::Render();
}