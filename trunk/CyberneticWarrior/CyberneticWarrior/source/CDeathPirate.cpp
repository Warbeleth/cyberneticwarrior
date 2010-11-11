#include "PrecompiledHeader.h"

#include "CDeathPirate.h"
#include "CCamera.h"

#include "CGame.h"
#include "CMapLoad.h"

CDeathPirate::CDeathPirate()
{
}
CDeathPirate::CDeathPirate(int nImageID, float PosX, float PosY,int Width, int Height, int nState,
				int nMaxHP, int nCurrentHP, int nSightRange, int nAttackRange, int nGlobalType, float fRateOfFire, 
				float fSpeed) : CIdleEnemy(nState, nGlobalType, nImageID, nMaxHP, nCurrentHP, nSightRange, 
				nAttackRange, fRateOfFire, fSpeed, PosX, PosY, Width, Height)
{
	SetAnimations(CMapLoad::GetInstance()->CreateAnimation(Boss_Pirate));
	GetAnimations()->SetCurrentAnimation(0);
}
CDeathPirate::~CDeathPirate()
{

}

void CDeathPirate::Update(float fElapsedTime)
{
	CIdleEnemy::Update(fElapsedTime);

	switch(ReturnAIState())
	{
	case Idle:
		GetAnimations()->SetCurrentAnimation(0);
		break;
	case iActive:
		GetAnimations()->SetCurrentAnimation(1);
		break;
	case iDead:
		CGame::GetInstance()->GetMessageSystemPointer()->SendMsg(new CDestroyEnemyMessage((CBaseEnemy*)this));
		break;
	};
}

void CDeathPirate::Render()
{
	CIdleEnemy::Render();
}