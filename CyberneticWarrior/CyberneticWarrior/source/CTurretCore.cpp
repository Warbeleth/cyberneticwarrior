#include "PrecompiledHeader.h"

#include "CTurretCore.h"
#include "CCamera.h"
#include "CMapLoad.h"

#include "CGame.h"

CTurretCore::CTurretCore()
{
}
CTurretCore::CTurretCore(int nImageID, float PosX, float PosY, int nGlobalType, int Width, int Height, int nState,
				int nMaxHP, int nCurrentHP, int nSightRange, int nAttackRange, float fRateOfFire, 
				float fSpeed) : CIdleEnemy(nState, nGlobalType, nImageID, nMaxHP, nCurrentHP, nSightRange, 
				nAttackRange, fRateOfFire, fSpeed, PosX, PosY, Width, Height)
{

	SetAnimations(CMapLoad::GetInstance()->CreateAnimation(nGlobalType));
	GetAnimations()->SetCurrentAnimation(0);
}
CTurretCore::~CTurretCore()
{

}

void CTurretCore::Update(float fElapsedTime)
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

void CTurretCore::Render()
{
	CIdleEnemy::Render();
}